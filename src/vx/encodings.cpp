#include "nkit/tools.h"
#include "nkit/detail/encodings.h"

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

} // namespace nkit
