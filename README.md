# nkit c++ library

## Intro

C++ library with following components:

1. **Dynamic data typing**. It is realized in **Dynamic class** (see src/nkit/dynamic.h) which supports following data types:
    - **INTEGER**: represets integer values (implemented via int64_t type)
    - **UNSIGNED_INTEGER**: represets unsigned integer values (implemented via uint64_t type)
    - **FLOAT**: represets numbers with floating point (implemented via double type)
    - **BOOL**: represets boolean values (implemented via bool type)
    - **DATE_TIME**: represets simple date-time values with microseconds and 0 <= year <= 9999 (implemented via uint64_t type)
    - **STRING**: represets string values (implemented via std::string)
    - **LIST**: represets list of Dynamic values (implemented via std::vector<Dynamic>)
    - **DICT**: represets map of string keys to Dynamic values (implemented via std::map<std::string, Dynamic>)
    - **MONGODB_OID**: represets string values with limited behaviour for holding MongoDb ObjectID (implemented via std::string)
    - **TABLE**: represets table of Dynamic values with support of multiple indexes, sorting and grouping by multiple fields
    - **UNDEFINED**: represents uninitialized value, that can be changed in the future
    - **NONE**: represents NULL value, that can not be changed

2. Simple test framework

3. Logger

**Dynamic** values can be used to deal with JSON: it is possible to convert JSON to Dynamic variable and vice-versa.

## Requirements

It is mandatory to have **yajl** library for JSON paring (https://github.com/lloyd/yajl, version >= 2.0.4)

If your C++ compiler does not support std::shared_ptr, then it is mandatory to have Boost library (http://boost.org, version >= 1.53)

## Build & install

cd to/nkit/root

This commands will configure, build and install debug version of nkit library (with non-system boost):

*./bootstrap.sh --prefix=/path/to/installation/folder --with-boost=/path/to/boost --with-yajl=/path/to/yajl --debug*

*make -C Debug-build*

*make -C Debug-build install*

This commands will configure, build and install release version of nkit library (with system boost):

*./bootstrap.sh --prefix=/path/to/installation/folder --with-boost --with-yajl=/path/to/yajl --release*

*make -C Release-build*

*make -C Release-build install*

This commands will configure, build and install release-with-debug-version of nkit library (without boost, if C++ compiler supports std::shared_ptr):

*./bootstrap.sh --prefix=/path/to/installation/folder --with-yajl=/path/to/yajl --rdebug*

*make -C RelWithDebInfo-build*

*make -C RelWithDebInfo-build install*

For all configure options type

*./bootstrap.sh --help*

## Usage

See **test/test_*.cpp** files for various use cases.

User friendly usage info will be added here as soon as possible.

## Author

Boris T. Darchiev (boris.darchiev <at> gmail.com)

On github: https://github.com/eye3

## THANKS TO

Vasiliy Soshnikov (dedok.mad <at> gmail.com):

On github: https://github.com/dedok

Wrote all cmake build files and bootstrap.sh

Participated in development of following parts of nkit:
- TABLE data type in Dynamic class
- Logger

