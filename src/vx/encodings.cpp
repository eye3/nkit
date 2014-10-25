#include "nkit/tools.h"
#include "nkit/detail/encodings.h"

#ifdef max
#undef max
#endif

namespace nkit
{
  static const size_t ALL_CHARS_LEN = 0x100;

  struct Encoding
  {
    const uint16_t codepage_id;
    const int map[ALL_CHARS_LEN];
  };

  struct Lang
  {
    const char * name;
    const uint16_t codepage_id;
  };

#include "vx/langs_inc.cpp"

  static const size_t LANGS_COUNT = sizeof(langs)/sizeof(Lang);

#include "vx/encodings_inc.cpp"

  static const size_t ENCODINGS_COUNT = sizeof(encodings)/sizeof(Encoding);

  //----------------------------------------------------------------------------
  bool find_encoding(const char * name, XML_Encoding * info)
  {
    for (size_t l = 0; l < LANGS_COUNT; ++l)
    {
      if (NKIT_STRCASECMP(langs[l].name, name) == 0)
      {
        for (size_t e = 0; e < ENCODINGS_COUNT; ++e)
        {
          const Encoding & encoding = encodings[e];
          if (encoding.codepage_id == langs[l].codepage_id)
          {
            for (size_t i = 0; i < ALL_CHARS_LEN; ++i)
              info->map[i] = encoding.map[i];
            return true;
          }
        }
        break;
      }
    }
    
    return false;
  }

  //----------------------------------------------------------------------------
  SingleUtf16CharMap::SingleUtf16CharMap(const uint16_t codepage_id)
    : codepage_id_(codepage_id)
    , hash_table_(ALL_CHARS_LEN)
  {}

  SingleUtf16CharMap::SingleUtf16CharMap()
    : codepage_id_(-1)
    , hash_table_(ALL_CHARS_LEN)
  {}

  void SingleUtf16CharMap::AddMapping(int single_utf16_c, char c)
  {
    hash_table_[single_utf16_c % ALL_CHARS_LEN][single_utf16_c] = c;
  }

  bool SingleUtf16CharMap::GetChar(int single_utf16_c, char * c) const
  {
    const Mapping & mapping = hash_table_[single_utf16_c % ALL_CHARS_LEN];
    Mapping::const_iterator it = mapping.find(single_utf16_c);
    if (unlikely(it == mapping.end()))
      return false;
    *c = it->second;
    return true;
  }

  //----------------------------------------------------------------------------
  typedef std::map<uint16_t, SingleUtf16CharMap> SingleUtf16CharMaps;
  static SingleUtf16CharMaps & get_maps()
  {
    static SingleUtf16CharMaps maps;
    return maps;
  }
  
  //----------------------------------------------------------------------------
  const SingleUtf16CharMap * find_encoding(const char * name)
  {
    for (size_t l = 0; l < LANGS_COUNT; ++l)
    {
      if (NKIT_STRCASECMP(langs[l].name, name) == 0)
      {
        SingleUtf16CharMaps & maps = get_maps();
        return &maps[langs[l].codepage_id];
      }
    }

    return NULL;
  }

  //----------------------------------------------------------------------------
  void build_utf16_convertors()
  {
    SingleUtf16CharMaps & maps = get_maps();
    for (size_t e = 0; e < ENCODINGS_COUNT; ++e)
    {
      const Encoding & encoding = encodings[e];
      SingleUtf16CharMap & map = maps[encoding.codepage_id];
      for (size_t i = 0; i < ALL_CHARS_LEN; ++i)
        map.AddMapping(encoding.map[i], i);
    }
  }

  //----------------------------------------------------------------------------
  int32_t utf16_to_ucs4(uint32_t *ucs4, const uint16_t *utf16,
    uint8_t *shorts_read)
  {
    if (utf16[0] < 0xD800)
    {
      *ucs4 = utf16[0];
      *shorts_read = 1;
      return true;
    }

    if (utf16[0] < 0xDC00) {
      if ((utf16[1] >= 0xDC00) && (utf16[1] < 0xE000))
      {
        *ucs4 = (((utf16[0] - 0xD800) << 10)
          + (utf16[1] - 0xDC00)) + 0x00010000;
        *shorts_read = 2;
        return true;
      }
      else return false;
    }

    if (utf16[0] < 0xFFFE)
    {
      *ucs4 = utf16[0];
      *shorts_read = 1;
      return true;
    }

    return false;
  }

  //----------------------------------------------------------------------------
  bool ucs4_to_utf8(char *utf8, uint32_t  ucs4)
  {
    if (ucs4 < 0x00000080)
    {
      utf8[0] = ucs4;
      //utf8[1] = '\0';
      return true;
    }

    if (ucs4 < 0x00000800)
    {
      utf8[0] = 0xc0 + (ucs4 >> 6);      // div 2^6
      utf8[1] = 0x80 + (ucs4 & 0x3F);    // mod 2^6
      //utf8[2] = '\0';
      return true;
    }

    // in UTF8 N.2 values in the range 0000 D800 to 0000 DFFF 
    // shall be excluded from conversion.                     
    // (I don't see the need of this, because this range is   
    // only relevant in UTF16 to decode 32-bit-values)        
    // -----> 97-09-27 Marcus Mueller                         

    if (ucs4 < 0x00010000)
    {
      utf8[0] = 0xe0 + (ucs4 >> 12);
      utf8[1] = 0x80 + ((ucs4 >> 6) & 0x3F);
      utf8[2] = 0x80 + (ucs4 & 0x3F);
      //utf8[3] = '\0';
      return true;
    }

    if (ucs4 < 0x00200000)
    {
      utf8[0] = 0xf0 + (ucs4 >> 18);
      utf8[1] = 0x80 + ((ucs4 >> 12) & 0x3F);
      utf8[2] = 0x80 + ((ucs4 >> 6) & 0x3F);
      utf8[3] = 0x80 + (ucs4 & 0x3F);
      //utf8[4] = '\0';
      return true;
    }

    if (ucs4 < 0x04000000)
    {
      utf8[0] = 0xf8 + (ucs4 >> 24);
      utf8[1] = 0x80 + ((ucs4 >> 18) & 0x3F);
      utf8[2] = 0x80 + ((ucs4 >> 12) & 0x3F);
      utf8[3] = 0x80 + ((ucs4 >> 6) & 0x3F);
      utf8[4] = 0x80 + (ucs4 & 0x3F);
      //utf8[5] = '\0';
      return true;
    }

    if (ucs4 < 0x80000000)
    {
      utf8[0] = 0xfc + (ucs4 >> 30);
      utf8[1] = 0x80 + ((ucs4 >> 24) & 0x3F);
      utf8[2] = 0x80 + ((ucs4 >> 18) & 0x3F);
      utf8[3] = 0x80 + ((ucs4 >> 12) & 0x3F);
      utf8[4] = 0x80 + ((ucs4 >> 6) & 0x3F);
      utf8[5] = 0x80 + (ucs4 & 0x3F);
      //utf8[6] = '\0';
      return true;
    }

    return false;
  }

  //----------------------------------------------------------------------------
  bool ucs4_to_utf16(uint16_t *utf16, uint32_t ucs4)
  {
    if (ucs4 <= 0x0000FFFD)
    {
      utf16[0] = (uint16_t)ucs4;
      //utf16[1] = 0l;
      return true;
    }

    if ((ucs4 >= 0x00010000) && (ucs4 <= 0x0010FFFF))
    {
      utf16[0] = ((ucs4 - 0x00010000) >> 10) + 0xD800;
      utf16[1] = ((ucs4 - 0x00010000) & 0x3FF) + 0xDC00;
      //utf16[2] = 0l;
      return true;
    }

    return false;
  }

  //----------------------------------------------------------------------------
  int32_t utf8_to_ucs4(uint32_t *ucs4, const char *utf8, uint8_t *bytes_read)
  {
    register const uint8_t *utf = (uint8_t*)utf8;
    register uint8_t read = 0;
    register uint32_t RC = 0;

    if ((utf[0]) && ((utf[0] & 0xC0) != 0x80))
    {
      if (utf[0] < 0xC0)
      {
        RC = utf[0];
        read = 1;
      }
      else if ((utf[1] & 0xC0) == 0x80)
      {
        if (utf[0] < 0xE0)
        {
          RC = (((utf[0] - 0xC0) << 6)
            + (utf[1] - 0x80));
          read = 2;
        }
        else if ((utf[2] & 0xC0) == 0x80)
        {
          if (utf[0] < 0xF0)
          {
            RC = (((utf[0] - 0xE0) << 12)
              + ((utf[1] - 0x80) << 6)
              + (utf[2] - 0x80));
            read = 3;
          }
          else if ((utf[3] & 0xC0) == 0x80)
          {
            if (utf[0] < 0xF8)
            {
              RC = (((utf[0] - 0xF0) << 18)
                + ((utf[1] - 0x80) << 12)
                + ((utf[2] - 0x80) << 6)
                + (utf[3] - 0x80));
              read = 4;
            }
            else if ((utf[4] & 0xC0) == 0x80)
            {
              if (utf[0] < 0xFC)
              {
                RC = (((utf[0] - 0xF8) << 24)
                  + ((utf[1] - 0x80) << 18)
                  + ((utf[2] - 0x80) << 12)
                  + ((utf[3] - 0x80) << 6)
                  + (utf[4] - 0x80));
                read = 4;
              }
              else if ((utf[5] & 0xC0) == 0x80)
              {
                if (utf[0] < 0xFE)
                {
                  RC = (((utf[0] - 0xFC) << 30)
                    + ((utf[1] - 0x80) << 24)
                    + ((utf[2] - 0x80) << 18)
                    + ((utf[3] - 0x80) << 12)
                    + ((utf[4] - 0x80) << 6)
                    + (utf[5] - 0x80));
                  read = 5;
                }
              }
            }
          }
        }
      }
    }

    if (read)
    {
      if (bytes_read) *bytes_read = read;
      if (ucs4) *ucs4 = RC;
      return true;
    }

    return false;
  }

  //----------------------------------------------------------------------------
  bool from_utf8(const SingleUtf16CharMap & map, const std::string & src,
    std::string * out)
  {
    out->reserve(std::max(out->size(), src.size()));
    const char * p = src.data();
    size_t bytes_left = src.size();
    while (bytes_left > 0)
    {
      uint16_t utf16_char[2];
      size_t simbol_length = utf8_size(*p);
      if (simbol_length > bytes_left)
        return false;
      if (!utf8_to_utf16(utf16_char, p))
        return false;
      char c;
      if (!map.GetChar(utf16_char[0], &c))
        return false;
      out->push_back(c);
      bytes_left -= simbol_length;
      p += simbol_length;
    }
    return true;
  }

  //----------------------------------------------------------------------------
  bool utf8_to_utf16(uint16_t *utf16, const char *utf8, uint8_t *bytes_read)
  {
    uint32_t character;
    if (!utf8_to_ucs4(&character, utf8, bytes_read))
      return false;
    return ucs4_to_utf16(utf16, character);
  }

  //----------------------------------------------------------------------------
  bool utf16_to_utf8(char *utf8, const uint16_t *utf16, uint8_t *shorts_read)
  {
    uint32_t character;
    if (!utf16_to_ucs4(&character, utf16, shorts_read))
      return false;
    return ucs4_to_utf8(utf8, character);
  }

  //----------------------------------------------------------------------------
  uint8_t utf8_size(char first_utf8_byte)
  {
    register uint8_t RC = 0;
    register uint8_t first = (uint8_t)first_utf8_byte;

    if (first < 0x80)
      RC = 1;
    else if (first < 0xC0) // 10xxxxxx
      RC = 0;             // ERROR
    else if (first < 0xE0) // 110xxxxx
      RC = 2;
    else if (first < 0xF0) // 1110xxxx
      RC = 3;
    else if (first < 0xF8) // 11110xxx
      RC = 4;
    else if (first < 0xFC) // 111110xx
      RC = 5;
    else if (first < 0xFE) // 1111110x
      RC = 6;
    else                  // ERROR
      RC = 0;

    return(RC);
  }
} // namespace nkit
