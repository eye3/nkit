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

#include <iostream>
#include <algorithm>
#include <string>
#include <limits>

#include "nkit/detail/config.h"
#include "nkit/test.h"
#include "nkit/dynamic.h"
#include "nkit/dynamic_getter.h"
#include "nkit/logger_brief.h"
#include "nkit/version.h"

#define TABLE_GROW_SIZE 10

namespace nkit_test
{
  using namespace nkit;

  NKIT_TEST_CASE(DynamicStrAppend)
  {
    Dynamic str("a");
    str.Append("b");
    NKIT_TEST_EQ(str, Dynamic("ab"));

    str = Dynamic("ab");
    str.Append("bb", 2);
    NKIT_TEST_EQ(str, Dynamic("abbb"));
  }

  NKIT_TEST_CASE(DynamicStrAssign)
  {
    Dynamic str("a");
    str.Assign("b");
    NKIT_TEST_EQ(str, Dynamic("b"));

    str.Assign("b");
    str.Append("a", 1);
    NKIT_TEST_EQ(str, Dynamic("ba"));
  }

  NKIT_TEST_CASE(DynamicStrTrim)
  {
    Dynamic str(" a ");
    str.Trim(" ");
    NKIT_TEST_EQ(str, Dynamic("a"));

    str = Dynamic(" a b\n");
    str.Trim(" \n");
    NKIT_TEST_EQ(str, Dynamic("a b"));

    str = Dynamic(" \n");
    str.Trim(" \n");
    NKIT_TEST_EQ(str, Dynamic(""));

    str = Dynamic(" a b\n");
    NKIT_TEST_EQ(str.TrimCopy(" \n"), "a b");
    NKIT_TEST_EQ(str, Dynamic(" a b\n"));

    str = Dynamic(" a");
    NKIT_TEST_EQ(str.TrimCopy(" "), "a");
    NKIT_TEST_EQ(str, Dynamic(" a"));
  }
} // namespace nkit_test
