#include "nkit/test.h"

namespace nkit_test
{
  using namespace nkit;

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(tools_simple_split_key_value)
  {
    std::string k("key"), v("value");

    std::string d(","), str(k + d + v), _k, _v;
    simple_split(str, d, &_k, &_v);
    NKIT_TEST_ASSERT_WITH_TEXT(k == _k, _k);
    NKIT_TEST_ASSERT_WITH_TEXT(v == _v, _v);

    str = k + " " + d + v;
    simple_split(str, d, &_k, &_v);
    NKIT_TEST_ASSERT_WITH_TEXT(k == _k, _k);
    NKIT_TEST_ASSERT_WITH_TEXT(v == _v, _v);

    d = ",,";
    str = k + d + " " + v;
    simple_split(str, d, &_k, &_v);
    NKIT_TEST_ASSERT_WITH_TEXT(k == _k, _k);
    NKIT_TEST_ASSERT_WITH_TEXT(v == _v, _v);

    str = k + " " + d + " " + v;
    simple_split(str, d, &_k, &_v);
    NKIT_TEST_ASSERT_WITH_TEXT(k == _k, _k);
    NKIT_TEST_ASSERT_WITH_TEXT(v == _v, _v);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(tools_simple_split)
  {
    std::string s0("qwe"), s1("asdasd"), s2("zxczxc"), s3("3333"), s4,
        s5("55555555");

    std::string d(","),
        str(s0 + d + " " + s1 + " " + d + " " + s2 + d + s3 + d);
    StringVector v;
    simple_split(str, d, &v);
    NKIT_TEST_ASSERT(v.size() == 5);
    NKIT_TEST_ASSERT_WITH_TEXT(v[0] == s0, v[0]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[1] == s1, v[1]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[2] == s2, v[2]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[3] == s3, v[3]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[4] == s4, v[4]);

    str += s5;
    simple_split(str, d, &v);
    NKIT_TEST_ASSERT(v.size() == 5);
    NKIT_TEST_ASSERT_WITH_TEXT(v[0] == s0, v[0]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[1] == s1, v[1]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[2] == s2, v[2]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[3] == s3, v[3]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[4] == s5, v[4]);

    str = s0;
    simple_split(str, d, &v);
    NKIT_TEST_ASSERT(v.size() == 1);
    NKIT_TEST_ASSERT_WITH_TEXT(v[0] == s0, v[0]);

    d = ",,";
    str = s0 + d + d;
    simple_split(str, d, &v);
    NKIT_TEST_ASSERT(v.size() == 3);
    NKIT_TEST_ASSERT_WITH_TEXT(v[0] == s0, v[0]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[1].empty(), v[1]);
    NKIT_TEST_ASSERT_WITH_TEXT(v[2].empty(), v[2]);
  }

} // namespace nkit_test
