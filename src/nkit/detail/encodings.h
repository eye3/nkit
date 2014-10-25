#ifndef VX_ENCODINGS_H
#define VX_ENCODINGS_H

#include "expat.h"

namespace nkit
{
  //----------------------------------------------------------------------------
  class SingleUtf16CharMap
  {
  private:
    typedef std::map<int, char> Mapping;

  public:
    SingleUtf16CharMap();
    SingleUtf16CharMap(const uint16_t codepage_id);
    void AddMapping(int single_utf16_c, char c);
    bool GetChar(int single_utf16_c, char * c) const;

  private:
    const uint16_t codepage_id_;
    std::vector<Mapping> hash_table_;
  };

  //----------------------------------------------------------------------------
  const SingleUtf16CharMap * find_encoding(const char * name);
  bool    find_encoding(const char * name, XML_Encoding * info);
  void    build_utf16_convertors();
  bool    from_utf8(const SingleUtf16CharMap & map, const std::string & src,
    std::string * out);
  bool    utf16_to_utf8(char *utf8, const uint16_t *utf16, uint8_t *shorts_read);
  bool    utf8_to_utf16(uint16_t *utf16, const char *utf8, uint8_t *bytes_read = NULL);
  uint8_t utf8_size(char first_utf8_byte);

} // namespace nkit

#endif // VX_ENCODINGS_H
