#include <iostream>
#include <fstream>
#include <string>
#include "fairport/util.h"

BOOST_AUTO_TEST_SUITE( util )

BOOST_AUTO_TEST_CASE( test_bytes_to_wstring )
{
    using namespace fairport;

    // Convert to and from Windows Unicode bytestring.
    const fairport::byte abc_data[] = { 'a', 0, 'b', 0, 'c', 0 };
    std::vector<fairport::byte> abc_bytes(abc_data, abc_data + 6);
    std::wstring abc_wstring(L"abc");

    std::vector<fairport::byte> out_bytes = wstring_to_bytes(abc_wstring);
    BOOST_CHECK_EQUAL_COLLECTIONS(out_bytes.begin(), out_bytes.end(), abc_bytes.begin(), abc_bytes.end());

    BOOST_CHECK(abc_wstring == bytes_to_wstring(abc_bytes));

    // Handle zero-length strings.
    BOOST_CHECK_EQUAL(wstring_to_bytes(std::wstring()).size(), 0);
    BOOST_CHECK_EQUAL(bytes_to_wstring(std::vector<fairport::byte>()).size(), 0);
}

BOOST_AUTO_TEST_CASE( test_codepage_1252_to_wstring )
{
    using namespace fairport;

    BOOST_CHECK(codepage_1252_to_wstring("") == L"");
#ifdef _MSC_VER
#pragma warning(disable:4428)
#endif
    BOOST_CHECK(codepage_1252_to_wstring("ab\xa7") == L"ab\u00a7");
    // Handle the character block with non-trivial Unicode mappings.
    BOOST_CHECK(codepage_1252_to_wstring("\x80\x81\x82\x83\x84\x85\x86\x87") == L"\u20ac\x0081\u201a\u0192\u201e\u2026\u2020\u2021");
    BOOST_CHECK(codepage_1252_to_wstring("\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f") == L"\u02c6\u2030\u0160\u2039\u0152\x008d\u017d\x008f");
    BOOST_CHECK(codepage_1252_to_wstring("\x90\x91\x92\x93\x94\x95\x96\x97") == L"\x0090\u2018\u2019\u201c\u201d\u2022\u2013\u2014");
    BOOST_CHECK(codepage_1252_to_wstring("\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f") == L"\u02dc\u2122\u0161\u203a\u0153\x009d\u017e\u0178");
}

BOOST_AUTO_TEST_SUITE_END()
