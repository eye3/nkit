#include "nkit/tools.h"
#include "nkit/logger_brief.h"

#include "nkit/test.h"

#ifdef NKIT_WINNT

static const size_t ALL_CHARS_LEN = 0x100;

static const char all_chars[ALL_CHARS_LEN + 1] =
"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f"
"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f"
"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f"
"\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f"
"\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f"
"\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f"
"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f"
"\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f"
"\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf"
"\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"
"\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf"
"\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf"
"\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef"
"\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff";

std::list<CPINFOEX> code_page_list_;

BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString)
{
  uint64_t codepage_id = NKIT_STRTOLL(lpCodePageString, NULL, 10);
  CPINFOEX cp_info;
  if (0 == GetCPInfoEx((UINT)codepage_id, 0, &cp_info))
    return TRUE;
  code_page_list_.push_back(cp_info);
  return TRUE;
}

NKIT_TEST_CASE(enc_gen)
{
  EnumSystemCodePages(EnumCodePagesProc, CP_INSTALLED);

  std::list<CPINFOEX>::const_iterator begin = code_page_list_.begin(),
    it = begin, end = code_page_list_.end();

  std::stringstream stream;
  stream << "Encoding encodings[] = {\n";

  for (size_t l=0; it != end; ++it, ++l)
  {
    const CPINFOEX & cpinfo = (*it);
    if( 1 != cpinfo.MaxCharSize )
    {
      //CINFO(cpinfo.CodePageName);
      continue;
    }
    wchar_t conv[ALL_CHARS_LEN];
    if( !MultiByteToWideChar(cpinfo.CodePage, 0,
            &(all_chars[0]), ALL_CHARS_LEN, conv, ALL_CHARS_LEN) )
      continue;
    stream << "  {\n";
    stream << "    " << cpinfo.CodePage << ",\n";
    stream << "    " << "{\n";
    size_t c=0;
    while(c < ALL_CHARS_LEN)
    {
      stream << "      ";
      for (size_t i=0; (i < 10) && (c < ALL_CHARS_LEN); ++i, ++c)
        stream << conv[c] << ((c+1 == ALL_CHARS_LEN) ? ' ': ',');
      stream << '\n';
    }
    stream << "    " << "}\n";
    stream << "  }" << ((l+1 == code_page_list_.size()) ? ' ': ',') << "\n";
  }
  stream << "};\n";

  nkit::string_to_text_file("../../src/vx/encodings_inc.cpp", stream.str());
}

#else
NKIT_TEST_CASE(enc_gen)
{
  NKIT_TEST_ASSERT_WITH_TEXT(false,
    "You can generate encodings.inc file only on windows platform");
}
#endif
