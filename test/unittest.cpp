#define BOOST_TEST_MODULE Fairport

#if _MSC_VER && _MSC_VER < 1600
#pragma warning(disable: 4996)
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4389 4535)
#endif
#include <boost/test/included/unit_test.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include "utiltest.cpp"
#include "btreetest.cpp"
#include "disktest.cpp"
#include "databasetest.cpp"
#include "highlevel.cpp"
#include "pstlevel.cpp"
