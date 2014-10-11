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

#include "nkit/dynamic_getter.h"

namespace nkit
{
  //----------------------------------------------------------------------------
  struct Var2XmlOptions
  {
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

      std::string version, encoding, indent, nweline;
      bool standalone;
      op
        .Get(".rootname", &res->root_name_, S_EMPTY_)
        .Get(".itemname", &res->item_name_, S_EMPTY_)
        .Get(".attrkey", &res->attr_key_, S_EMPTY_)
        .Get(".charkey", &res->char_key_, S_EMPTY_)
        .Get(".xmldec.version", &version, S_EMPTY_)
        .Get(".xmldec.encoding", &encoding, S_UTF_8_)
        .Get(".xmldec.standalone", &standalone, true)
        .Get(".pretty.indent", &res->pretty_.indent_, S_EMPTY_)
        .Get(".pretty.newline", &res->pretty_.newline_, S_EMPTY_)
      ;

      if (!op.ok())
      {
        *error = op.error();
        return Ptr();
      }

      if (!version.empty())
        res->xml_dec_ = "<?xml version=\"" + version +
            "\" encoding=\"" + encoding +
            "\" standalone=\"" + (standalone? "yes": "no") + "\"?>";

      return res;
    }

    std::string root_name_;
    std::string item_name_;
    std::string attr_key_;
    std::string char_key_;
    std::string xml_dec_;
    Pretty pretty_;
  };  // struct Var2XmlOptions

  //----------------------------------------------------------------------------
  class Var2XmlConverter
  {
  public:
    Var2XmlConverter(Var2XmlOptions::Ptr options)
      : options_(options)
      , first_end_after_begin_(false)
      , begin_(true)
    {}

    //----------------------------------------------------------------------------
    static bool Run(const Dynamic & options, const Dynamic & data,
        std::string * out, std::string * error)
    {
      Var2XmlOptions::Ptr op = Var2XmlOptions::Create(options, error);
      if (!op)
        return false;

      Var2XmlConverter builder(op);

      if (data.IsDict())
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
      else if (data.IsList())
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
    //----------------------------------------------------------------------------
    bool Convert(const std::string & item_name, const Dynamic & data,
        Var2XmlConverter & builder, std::string * out, std::string * error)
    {
      if (data.IsDict())
      {
        Dynamic::HashConstIterator it = data.begin_h(), end = data.end_h();
        for (; it != end; ++it)
        {
          const std::string & key = it->first;
          if (options_->attr_key_ == key
              || options_->char_key_ == key)
            continue;
          const Dynamic & v = it->second;
          if (!v.IsList())
            builder.BeginElement(key, v, out);
          if (!Convert(key, v, builder, out, error))
            return false;
          if (!v.IsList())
            builder.EndElement(out);
        }

        // charkey option ('_')
        const Dynamic * text = NULL;
        if (data.Get(options_->char_key_, &text))
          builder.PutText(
              text->IsString() ? text->GetConstString(): text->GetString(),
              true, out);
      }
      else if (data.IsList())
      {
        bool use_item_name = !item_name.empty();
        Dynamic::ArrayConstIterator it = data.begin_l(), end = data.end_l();
        for (size_t counter = 0; it != end; ++it, ++counter)
        {
          builder.BeginElement(use_item_name ? item_name : string_cast(counter),
              *it, out);
          if (!Convert("", *it, builder, out, error))
            return false;
          builder.EndElement(out);
        }
      }
      else
      {
        builder.PutText(
            data.IsString() ? data.GetConstString(): data.GetString(), false, out);
      }

      return true;
    }

    //----------------------------------------------------------------------------
    void BeginElement(const std::string & name, const Dynamic & data,
        std::string * out)
    {
      std::string & out_ = *out;
      if (begin_)
      {
        begin_ = false;
        if (!options_->xml_dec_.empty() && !options_->root_name_.empty())
        {
          out_.append(options_->xml_dec_);
          out_.append(options_->pretty_.newline_);
        }
      }
      else
        out_.append(options_->pretty_.newline_);

      path_.push(name);
      out_.append(current_indent_);
      out_.append("<");
      out_.append(name);

      // attrkey option ('$')
      const Dynamic & attrs = data[options_->attr_key_];
      if (attrs.IsDict())
      {
        DDICT_FOREACH(pair, attrs)
        {
          out_ += ' ';
          out_.append(pair->first);
          out_.append("=\"");
          PutText(pair->second.IsString() ? pair->second.GetConstString():
              pair->second.GetString(), out);
          out_ += '\"';

        }
      }
      out_.append(">");
      current_indent_ += options_->pretty_.indent_;
      first_end_after_begin_ = true;
    }

    //----------------------------------------------------------------------------
    void EndElement(std::string * out)
    {
      assert(!path_.empty());

      std::string & out_ = *out;
      current_indent_.resize(
          current_indent_.size() - options_->pretty_.indent_.size());
      if (!first_end_after_begin_)
      {
        out_.append(options_->pretty_.newline_);
        out_.append(current_indent_);
      }
      first_end_after_begin_ = false;
      out_.append("</");
      out_.append(path_.top());
      out_.append(">");
      path_.pop();
    }

    //----------------------------------------------------------------------------
    void PutText(const std::string & text, bool newline, std::string * out)
    {
      if (text.empty())
        return;

      std::string & out_ = *out;

      if (newline)
      {
        out_.append(options_->pretty_.newline_);
        out_.append(current_indent_);
      }

      size_t count = text.size();
      for (size_t i=0; i < count; ++i)
      {
        char c = text[i];
        switch (c)
        {
        case '<':
          out_.append("&lt;");
          break;
        case '>':
          out_.append("&gt;");
          break;
        case '&':
          out_.append("&amp;");
          break;
        case '"':
          out_.append("&quot;");
          break;
        case '\'':
          out_.append("&apos;");
          break;
        default:
          out_ += c;
          break;
        }
      }
    }

    void PutText(const std::string & text, std::string * out)
    {
      std::string & out_ = *out;

      size_t count = text.size();
      for (size_t i=0; i < count; ++i)
      {
        char c = text[i];
        switch (c)
        {
        case '<':
          out_.append("&lt;");
          break;
        case '>':
          out_.append("&gt;");
          break;
        case '&':
          out_.append("&amp;");
          break;
        case '"':
          out_.append("&quot;");
          break;
        case '\'':
          out_.append("&apos;");
          break;
        default:
          out_ += c;
          break;
        }
      }
    }

  private:
    //----------------------------------------------------------------------------
    void PutCdata(const std::string & cdata, std::string * out)
    {
      std::string & out_ = *out;
      out_.append("<![CDATA[");
      out_.append(cdata);
      out_.append("]]>");
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
