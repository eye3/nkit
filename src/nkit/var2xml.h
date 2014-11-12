/*
   Copyright 2010-2014 Boris T. Darchiev (boris.darchiev@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NKIT__XML2VAR__H__
#define NKIT__XML2VAR__H__

#include <stack>

#include "nkit/dynamic_getter.h"
#include "nkit/transcode.h"

namespace nkit
{
  //----------------------------------------------------------------------------
  struct Var2XmlOptions
  {
    static const size_t DEFAULT_FLOAT_PRECISION;
    static const std::string ITEM_NAME_DEFAULT;
    static const std::string BOOL_TRUE;
    static const std::string BOOL_FALSE;
    static const std::string ATTR_KEY_DEFAULT;
    static const std::string TEXT_KEY_DEFAULT;

    struct Pretty
    {
      std::string indent_;
      std::string newline_;
    };

    typedef NKIT_SHARED_PTR(Var2XmlOptions) Ptr;

    static Ptr Create(const Dynamic & options, std::string * error)
    {
      Ptr res(new Var2XmlOptions);
      DynamicGetter op(options);

      static const StringSet EMPTY_STRING_SET;

      std::string version, encoding, indent, nweline;
      bool standalone;
      op
        .Get(".rootname", &res->root_name_, S_EMPTY_)
        .Get(".itemname", &res->item_name_, ITEM_NAME_DEFAULT)
        .Get(".attrkey", &res->attr_key_, ATTR_KEY_DEFAULT)
        .Get(".textkey", &res->text_key_, TEXT_KEY_DEFAULT)
        .Get(".xmldec.version", &version, S_EMPTY_)
        .Get(".xmldec.encoding", &encoding, S_UTF_8_)
        .Get(".xmldec.standalone", &standalone, true)
        .Get(".pretty.indent", &res->pretty_.indent_, S_EMPTY_)
        .Get(".pretty.newline", &res->pretty_.newline_, S_EMPTY_)
        .Get(".cdata", &res->cdata_, EMPTY_STRING_SET)
        .Get(".float_precision", &res->float_precision_,
                DEFAULT_FLOAT_PRECISION)
        .Get(".date_time_format", &res->date_time_format_,
                S_DATE_TIME_DEFAULT_FORMAT_)
        .Get(".bool_true", &res->bool_true_, BOOL_TRUE)
        .Get(".bool_false", &res->bool_false_, BOOL_FALSE)
      ;

      if (!op.ok())
      {
        *error = op.error();
        return Ptr();
      }

      if (res->cdata_.empty())
      {
        op.Get(".+cdata", &res->cdata_, EMPTY_STRING_SET);
        if (!op.ok())
        {
          *error = op.error();
          return Ptr();
        }
      }

      if (res->cdata_.empty())
      {
        op.Get(".-cdata", &res->cdata_, EMPTY_STRING_SET);
        if (!op.ok())
        {
          *error = op.error();
          return Ptr();
        }

        if (!res->cdata_.empty())
          res->cdata_exclude_ = true;
      }

      if (!istrequal(encoding, S_UTF_8_))
      {
        res->transcoder_ = Transcoder::Find(encoding);
        if (!res->transcoder_)
        {
          *error = "'" + encoding + "' encoding doesn't supported";
          return Ptr();
        }
      }

      if (!version.empty())
      {
        res->xml_dec_ = "<?xml version=\"" + version +
            "\" encoding=\"" + encoding +
            "\" standalone=\"" + (standalone? "yes": "no") + "\"?>";
      }

      return res;
    }

    Var2XmlOptions()
      : transcoder_(NULL)
      , cdata_exclude_(false)
      , float_precision_(DEFAULT_FLOAT_PRECISION)
    {}

    const Transcoder * transcoder_;
    std::string root_name_;
    std::string item_name_;
    std::string attr_key_;
    std::string text_key_;
    std::string xml_dec_;
    Pretty pretty_;
    StringSet cdata_;
    bool cdata_exclude_;
    size_t float_precision_;
    std::string date_time_format_;
    std::string bool_true_;
    std::string bool_false_;
  };  // struct Var2XmlOptions

  //----------------------------------------------------------------------------
  template <typename T>
  class Var2XmlConverter
  {
    typedef typename T::type DataType;
    typedef typename T::DictConstIterator DictConstIterator;
    typedef typename T::ListConstIterator ListConstIterator;

  public:
    //--------------------------------------------------------------------------
    static bool Process(const std::string & options, const DataType & data,
        std::string * out, std::string * error)
    {
      Dynamic op = DynamicFromJson(options, error);
      if (!op && !error->empty())
        return false;
      return Process(op, data, out, error);
    }

    //--------------------------------------------------------------------------
    static bool Process(const Dynamic & options, const DataType & data,
        std::string * out, std::string * error)
    {
      Var2XmlOptions::Ptr op = Var2XmlOptions::Create(options, error);
      if (!op)
        return false;

      Var2XmlConverter builder(op);

      if (T::IsDict(data))
      {
        if (op->root_name_.empty())
          op->root_name_ = op->item_name_.empty() ? "root": op->item_name_;

        const std::string & item_name =
            op->item_name_.empty() ? op->root_name_: op->item_name_;

        builder.BeginElement(op->root_name_, data, out);

        if (!builder.Convert(item_name, data, builder, out, error))
          return false;

        builder.EndElement(out);
      }
      else if (T::IsList(data))
      {
        if (!op->root_name_.empty())
          builder.BeginElement(op->root_name_, data, out);

        if (!builder.Convert(op->item_name_, data, builder, out, error))
          return false;

        if (!op->root_name_.empty())
          builder.EndElement(out);
      }
      else
      {
        *error = "Variable MUST be object (dict) or list";
        return false;
      }

      return true;
    }

  private:
    //--------------------------------------------------------------------------
    Var2XmlConverter(Var2XmlOptions::Ptr options)
      : options_(options)
      , first_end_after_begin_(false)
      , begin_(true)
    {}

//    bool check_xml_name(const std::string & name)
//    {
//      static const char * NOT_IN = "!@#$%^&*()=+?/\\|'\"`~,; \t\r\n";
//      static const std::string NOT_FIRST = std::string("1234567890:-.") + NOT_IN;
//
//      if (unlikely(nkit::istarts_with(name, "xml")))
//        return false;
//      else if (unlikely(NOT_FIRST.find(name[0]) != NOT_FIRST.npos))
//        return false;
//      else if (unlikely(name.find(" ") != name.npos))
//        return false;
//
//      size_t size = name.size();
//      for (size_t i=1; i < size; ++i)
//      {
//        if (strchr(NOT_IN, name[i]) != NULL)
//          return false;
//      }
//
//      return true;
//    }
//
    //--------------------------------------------------------------------------
    bool Convert(const std::string & item_name, const DataType & data,
        Var2XmlConverter & builder, std::string * out, std::string * error)
    {
      if (T::IsDict(data))
      {
        DictConstIterator it = T::begin_d(data), end = T::end_d(data);
        for (; it != end; ++it)
        {
          std::string key(T::First(it));
          if (options_->attr_key_ == key || options_->text_key_ == key)
            continue;

//          if (!check_xml_name(key))
//          {
//            *error = "Wrong name for XML element: " + key;
//            return false;
//          }
//
          DataType v = T::Second(it);
          if (!T::IsList(v))
            builder.BeginElement(key, v, out);
          if (!Convert(key, v, builder, out, error))
            return false;
          if (!T::IsList(v))
            builder.EndElement(out);
        }

        // textkey option ('_')
        bool found(false);
        DataType text = T::GetByKey(data, options_->text_key_, &found);
        if (found)
          builder.PutText(text, true, out);
      }
      else if (T::IsList(data))
      {
        bool use_item_name = !item_name.empty();
        ListConstIterator it = T::begin_l(data), end = T::end_l(data);
        for (size_t counter = 0; it != end; ++it, ++counter)
        {
          builder.BeginElement(
                  use_item_name ? item_name :
                          options_->item_name_, // + string_cast(counter),
                  T::Value(it), out);
          if (!Convert("", T::Value(it), builder, out, error))
            return false;
          builder.EndElement(out);
        }
      }
      else
      {
        builder.PutText(data, false, out);
      }

      return true;
    }

    //--------------------------------------------------------------------------
    void BeginElement(const std::string & name, const DataType & data,
        std::string * out)
    {
      if (begin_)
      {
        begin_ = false;
        if (!options_->xml_dec_.empty() && !options_->root_name_.empty())
        {
          out->append(options_->xml_dec_);
          out->append(options_->pretty_.newline_);
        }
      }
      else
        out->append(options_->pretty_.newline_);

      path_.push(name);
      out->append(current_indent_);
      out->append("<");
      AppendTranscoded(name, out);

      // attrkey option ('$')
      if (T::IsDict(data))
      {
        bool found = false;
        DataType attrs = T::GetByKey(data, options_->attr_key_, &found);
        if (found && T::IsDict(attrs))
        {
          DictConstIterator pair = T::begin_d(attrs), end = T::end_d(attrs);
          for (; pair != end; ++pair)
          {
            (*out) += ' ';
            AppendTranscoded(T::First(pair), out);
            out->append("=\"");
            PutText(T::Second(pair), out);
            (*out) += '\"';

          }
        }
      }

      out->append(">");
      current_indent_ += options_->pretty_.indent_;
      first_end_after_begin_ = true;
    }

    //--------------------------------------------------------------------------
    void EndElement(std::string * out)
    {
      assert(!path_.empty());

      current_indent_.resize(
          current_indent_.size() - options_->pretty_.indent_.size());
      if (!first_end_after_begin_)
      {
        out->append(options_->pretty_.newline_);
        out->append(current_indent_);
      }
      first_end_after_begin_ = false;
      out->append("</");
      AppendTranscoded(path_.top(), out);
      out->append(">");
      path_.pop();
    }

    //--------------------------------------------------------------------------
    void AppendTranscoded(const std::string & text, std::string * out)
    {
      if (options_->transcoder_)
        options_->transcoder_->FromUtf8(text, out);
      else
        out->append(text);
    }

    //--------------------------------------------------------------------------
    void PutText(const DataType & data, bool newline, std::string * out)
    {
      // TODO: optimize by member string
      if (T::IsDateTime(data))
        PutText(T::GetStringAsDateTime(data, options_->date_time_format_),
                newline, out);
      else if (T::IsFloat(data))
        PutText(T::GetStringAsFloat(data, options_->float_precision_),
                newline, out);
      else if (T::IsBool(data))
        PutText(T::GetStringAsBool(data,
                  options_->bool_true_,
                  options_->bool_false_),
                newline, out);
      else
        PutText(T::GetString(data), newline, out);
    }

    //--------------------------------------------------------------------------
    void PutText(const DataType & data, std::string * out)
    {
      // TODO: optimize by member string
      if (T::IsDateTime(data))
        PutText(T::GetStringAsDateTime(data, options_->date_time_format_), out);
      else if (T::IsFloat(data))
        PutText(T::GetStringAsFloat(data, options_->float_precision_), out);
      else if (T::IsBool(data))
        PutText(T::GetStringAsBool(data,
                  options_->bool_true_,
                  options_->bool_false_),
                out);
      else
        PutText(T::GetString(data), out);
    }

    //--------------------------------------------------------------------------
    void PutText(const std::string & text, bool newline, std::string * out)
    {
      if (text.empty())
        return;

      if (newline)
      {
        out->append(options_->pretty_.newline_);
        out->append(current_indent_);
      }

      if (options_->cdata_.empty())
      {
        PutText(text, out);
      }
      else
      {
        bool found =
                options_->cdata_.find(path_.top()) != options_->cdata_.end();
        if (( found && !options_->cdata_exclude_) ||
            (!found &&  options_->cdata_exclude_))
          PutCdata(text, out);
        else
          PutText(text, out);
      }
    }

    //--------------------------------------------------------------------------
    void PutText(const std::string & text, std::string * out)
    {
      if (options_->transcoder_)
      {
        options_->transcoder_->FromUtf8(text, SpetialCharCallback, out);
      }
      else
      {
        size_t count = text.size();
        for (size_t i=0; i < count; ++i)
        {
          char ch = text[i];
          if (SpetialCharCallback(ch, out))
            continue;
          (*out) += ch;
        }
      }
    }

    static bool SpetialCharCallback(char ch, std::string * out)
    {
      switch (ch)
      {
      case '<':
        out->append("&lt;");
        return true;
      case '>':
        out->append("&gt;");
        return true;
      case '&':
        out->append("&amp;");
        return true;
      case '"':
        out->append("&quot;");
        return true;
      case '\'':
        out->append("&apos;");
        return true;
      default:
        break;
      }
      return false;
    }

    //--------------------------------------------------------------------------
    void PutCdata(const std::string & cdata, std::string * out)
    {
      out->append("<![CDATA[");
      AppendTranscoded(cdata, out);
      out->append("]]>");
    }

  private:
    Var2XmlOptions::Ptr options_;
    std::stack<std::string> path_;
    std::string current_indent_;
    bool first_end_after_begin_;
    bool begin_;
  };  // Var2XmlConverter

}  // namespace nkit

#endif  // NKIT__XML2VAR__H__
