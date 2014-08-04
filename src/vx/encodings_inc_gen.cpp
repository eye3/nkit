#include "nkit/tools.h"
#include "nkit/logger_brief.h"

#ifdef NKIT_WINNT

struct Lang
{
  const char * name;
  int codepage_id;
};

static Lang langs[] =
{
  { "437", 437},
  { "_iso-2022-jp", 50221},
  { "_iso-2022-jp$sio", 50222},
  { "ansi_x3.4-1968", 1252},
  { "ansi_x3.4-1986", 1252},
  { "arabic", 28596},
  { "ascii", 1252},
  { "asmo-708", 708},
  { "big5", 950},
  { "chinese", 936},
  { "cn-big5", 950},
  { "cn-gb", 936},
  { "codepage437", 437},
  { "cp1026", 1026},
  { "cp1251", 1251},
  { "cp1256", 1256},
  { "cp367", 1252},
  { "cp437", 437},
  { "cp500", 775},
  { "cp819", 1252},
  { "cp852", 852},
  { "cp866", 866},
  { "cp870", 870},
  { "csascii", 1252},
  { "csbig5", 950},
  { "cseuckr", 51949},
  { "cseucpkdfmtjapanese", 51932},
  { "csgb2312", 936},
  { "csgb231280", 936},
  { "csiso", 28591},
  { "csiso2022jp", 50221},
  { "csiso2022kr", 50225},
  { "csiso58gb231280", 936},
  { "csisolatin2", 28592},
  { "csisolatin4", 28594},
  { "csisolatin5", 28595},
  { "csisolatinarabic", 28596},
  { "csisolatincyrillic", 28595},
  { "csisolatingreek", 28597},
  { "csisolatinhebrew", 28598},
  { "cskoi8r", 20866},
  { "csksc56011987", 949},
  { "cspc8", 437},
  { "csshiftjis", 932},
  { "csunicode11utf7", 65000},
  { "cswindows31j", 932},
  { "cyrillic", 28595},
  { "dos-720", 720},
  { "dos-862", 862},
  { "dos-874", 874},
  { "ebcdic-cp-us", 37},
  { "ecma-114", 28596},
  { "ecma-118", 28597},
  { "elot_928", 28597},
  { "euc-cn", 51936},
  { "euc-jp", 51932},
  { "euc-kr", 51949},
  { "extended_unix_code_packed_format_for_japanese", 51932},
  { "gb2312", 936},
  { "gb2312-80", 936},
  { "gb231280", 936},
  { "gb_2312-80", 936},
  { "gbk", 936},
  { "greek", 28597},
  { "greek8", 28597},
  { "hebrew", 28598},
  { "hz-gb-2312", 52936},
  { "ibm367", 1252},
  { "ibm437", 437},
  { "ibm737", 737},
  { "ibm775", 775},
  { "ibm819", 1252},
  { "ibm850", 850},
  { "ibm852", 852},
  { "ibm857", 857},
  { "ibm861", 861},
  { "ibm866", 866},
  { "ibm869", 869},
  { "iso-2022-jp", 50222},
  { "iso-2022-kr", 50225},
  { "iso-8859-1", 1252},
  { "iso-8859-11", 874},
  { "iso-8859-15", 28605},
  { "iso-8859-2", 28592},
  { "iso-8859-3", 28593},
  { "iso-8859-4", 28594},
  { "iso-8859-5", 28595},
  { "iso-8859-6", 28596},
  { "iso-8859-7", 28597},
  { "iso-8859-8", 1255},
  { "iso-8859-8-i", 38598},
  { "iso-8859-9", 1254},
  { "iso-ir-100", 1252},
  { "iso-ir-101", 28592},
  { "iso-ir-109", 28593},
  { "iso-ir-110", 28594},
  { "iso-ir-126", 28597},
  { "iso-ir-127", 28596},
  { "iso-ir-138", 28598},
  { "iso-ir-144", 28595},
  { "iso-ir-148", 1254},
  { "iso-ir-149", 949},
  { "iso-ir-58", 936},
  { "iso-ir-6", 1252},
  { "iso-ir-6us", 20127},
  { "iso646-us", 1252},
  { "iso8859-1", 1252},
  { "iso8859-2", 28592},
  { "iso_646.irv:1991", 1252},
  { "iso_8859-1", 1252},
  { "iso_8859-15", 28605},
  { "iso_8859-1:1987", 1252},
  { "iso_8859-2", 28592},
  { "iso_8859-2:1987", 28592},
  { "iso_8859-3", 28593},
  { "iso_8859-3:1988", 28593},
  { "iso_8859-4", 28594},
  { "iso_8859-4:1988", 28594},
  { "iso_8859-5", 28595},
  { "iso_8859-5:1988", 28595},
  { "iso_8859-6", 28596},
  { "iso_8859-6:1987", 28596},
  { "iso_8859-7", 28597},
  { "iso_8859-7:1987", 28597},
  { "iso_8859-8", 28598},
  { "iso_8859-8-i", 1255},
  { "iso_8859-8:1988", 28598},
  { "iso_8859-9", 1254},
  { "iso_8859-9:1989", 1254},
  { "johab", 1361},
  { "koi", 20866},
  { "koi8", 20866},
  { "koi8-r", 20866},
  { "koi8-ru", 21866},
  { "koi8-u", 21866},
  { "koi8r", 20866},
  { "korean", 949},
  { "ks_c_5601", 949},
  { "ks_c_5601-1987", 949},
  { "ks_c_5601-1989", 949},
  { "ks_c_5601_1987", 949},
  { "ksc5601", 949},
  { "ksc_5601", 949},
  { "l1", 28591},
  { "l2", 28592},
  { "l3", 28593},
  { "l4", 28594},
  { "l5", 28599},
  { "l9", 28605},
  { "latin1", 1252},
  { "latin2", 28592},
  { "latin3", 28593},
  { "latin4", 28594},
  { "latin5", 1254},
  { "latin9", 28605},
  { "logical", 38598},
  { "macintosh", 10000},
  { "ms_kanji", 932},
  { "shift-jis", 932},
  { "shift_jis", 932},
  { "tis-620", 874},
  { "unicode", 1200},
  { "unicode-1-1-utf-7", 65000},
  { "unicode-1-1-utf-8", 65001},
  { "unicode-2-0-utf-8", 65001},
  { "unicodefffe", 1201},
  { "us", 1252},
  { "us-ascii", 1252},
  { "utf-16", 1200},
  { "utf-7", 65000},
  { "utf-8", 65001},
  { "visual", 1255},
  { "windows-1250", 1250},
  { "windows-1251", 1251},
  { "windows-1252", 1252},
  { "windows-1253", 1253},
  { "windows-1254", 1254},
  { "windows-1255", 1255},
  { "windows-1256", 1256},
  { "windows-1257", 1257},
  { "windows-1258", 1258},
  { "windows-874", 874},
  { "x-ansi", 1252},
  { "x-chinese-cns", 20000},
  { "x-chinese-eten", 20002},
  { "x-cp1250", 1250},
  { "x-cp1251", 1251},
  { "x-ebcdic-arabic", 20420},
  { "x-ebcdic-cp-us-euro", 1140},
  { "x-ebcdic-cyrillicrussian", 20880},
  { "x-ebcdic-cyrillicserbianbulgarian", 21025},
  { "x-ebcdic-denmarknorway", 20277},
  { "x-ebcdic-denmarknorway-euro", 1142},
  { "x-ebcdic-finlandsweden", 20278},
  { "x-ebcdic-finlandsweden-euro", 1143},
  { "x-ebcdic-france", 1143},
  { "x-ebcdic-france-euro", 1147},
  { "x-ebcdic-germany", 20273},
  { "x-ebcdic-germany-euro", 1141},
  { "x-ebcdic-greek", 20423},
  { "x-ebcdic-greekmodern", 875},
  { "x-ebcdic-hebrew", 20424},
  { "x-ebcdic-icelandic", 20871},
  { "x-ebcdic-icelandic-euro", 1149},
  { "x-ebcdic-international-euro", 1148},
  { "x-ebcdic-italy", 20280},
  { "x-ebcdic-italy-euro", 1144},
  { "x-ebcdic-japaneseandjapaneselatin", 50939},
  { "x-ebcdic-japaneseandkana", 50930},
  { "x-ebcdic-japaneseanduscanada", 50931},
  { "x-ebcdic-japanesekatakana", 20290},
  { "x-ebcdic-koreanandkoreanextended", 50933},
  { "x-ebcdic-koreanextended", 20833},
  { "x-ebcdic-simplifiedchinese", 50935},
  { "x-ebcdic-spain", 20284},
  { "x-ebcdic-spain-euro", 1145},
  { "x-ebcdic-thai", 20838},
  { "x-ebcdic-traditionalchinese", 50937},
  { "x-ebcdic-turkish", 20905},
  { "x-ebcdic-uk", 20285},
  { "x-ebcdic-uk-euro", 1146},
  { "x-euc", 51932},
  { "x-euc-cn", 51936},
  { "x-euc-jp", 51932},
  { "x-europa", 29001},
  { "x-ia5", 20105},
  { "x-ia5-german", 20106},
  { "x-ia5-norwegian", 20108},
  { "x-ia5-swedish", 20107},
  { "x-iscii-as", 57006},
  { "x-iscii-be", 57003},
  { "x-iscii-de", 57002},
  { "x-iscii-gu", 57010},
  { "x-iscii-ka", 57008},
  { "x-iscii-ma", 57009},
  { "x-iscii-or", 57007},
  { "x-iscii-pa", 57011},
  { "x-iscii-ta", 57004},
  { "x-iscii-te", 57005},
  { "x-mac-arabic", 10004},
  { "x-mac-ce", 10029},
  { "x-mac-chinesesimp", 10008},
  { "x-mac-chinesetrad", 10002},
  { "x-mac-cyrillic", 10007},
  { "x-mac-greek", 10006},
  { "x-mac-hebrew", 10005},
  { "x-mac-icelandic", 10079},
  { "x-mac-japanese", 10001},
  { "x-mac-korean", 10003},
  { "x-mac-turkish", 10081},
  { "x-ms-cp932", 932},
  { "x-sjis", 932},
  { "x-unicode-2-0-utf-7", 65000},
  { "x-unicode-2-0-utf-8", 65001},
  { "x-x-big5", 950}
};

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

void enc_gen()
{
  std::cout << "Encoding encodings[] = {\n";
  const size_t LANG_COUNT = sizeof(langs)/sizeof(Lang);
  for (size_t l=0; l < LANG_COUNT; ++l)
  {
    const Lang & lang = langs[l];
    CPINFO cp_info;
    if( 1 != GetCPInfo( lang.codepage_id, &cp_info) )
    continue;
    if( 1 != cp_info.MaxCharSize )
    continue;
    wchar_t conv[ALL_CHARS_LEN];
    if( !MultiByteToWideChar(lang.codepage_id, 0,
            &(all_chars[0]), ALL_CHARS_LEN, conv, ALL_CHARS_LEN) )
    continue;
    std::cout << "  {\n";
    std::cout << "    \"" << lang.name << "\",\n";
    std::cout << "    " << "{\n";
    size_t c=0;
    while(c < ALL_CHARS_LEN)
    {
      std::cout << "      ";
      for (size_t i=0; (i < 10) && (c < ALL_CHARS_LEN); ++i, ++c)
      std::cout << conv[c] << ((c+1 == ALL_CHARS_LEN) ? ' ': ',');
      std::cout << '\n';
      //c++;
    }
    std::cout << "    " << "}\n";
    std::cout << "  }" << ((l+1 == LANG_COUNT) ? ' ': ',') << "\n";
  }
  std::cout << "};\n";
}

#else
void enc_gen()
{
  CERR("You can generate encodings.inc file only on windows platform");
}
#endif
