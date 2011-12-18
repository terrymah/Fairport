#include <iostream>
#include <string>
#include <algorithm>

#include "fairport/ndb/database.h"
#include "fairport/ndb/database_iface.h"
#include "fairport/ndb/page.h"

#include "fairport/pst/message.h"
#include "fairport/pst/folder.h"
#include "fairport/pst/pst.h"

void process_recipient(const fairport::recipient& r)
{
    using namespace std;
    using namespace fairport;

    //wcout << "\t\t" << r.get_name() << "(" << r.get_email_address() << ")\n";
    (void)r.get_name(); (void)r.get_email_address();
}

void process_message(const fairport::message& m);
void process_attachment(const fairport::attachment& a)
{
    using namespace std;
    using namespace fairport;

    //wcout << "\t\t" << a.get_filename() << endl;
    (void)a.get_filename();

    if(a.is_message())
    {
        process_message(a.open_as_message());
    }
    else
    {
        std::wstring wfilename = a.get_filename();
        std::string filename(wfilename.begin(), wfilename.end());
        ofstream newfile(filename.c_str(), ios::out | ios::binary);
        newfile << a;

        std::vector<fairport::byte> contents = a.get_bytes();
        BOOST_CHECK_EQUAL(contents.size(), a.content_size());
    }
}

void process_message(const fairport::message& m)
{
    using namespace std;
    using namespace fairport;

    //wcout << "Message Subject: " << m.get_subject() << endl;
    (void)m.get_subject();

    //wcout << "\tAttachment Count: " << m.get_attachment_count() << endl;
    (void)m.get_attachment_count();
    for_each(m.attachment_begin(), m.attachment_end(), process_attachment);

    //wcout << "\tRecipient Count: " << m.get_recipient_count() << endl;
    (void)m.get_recipient_count();
    for_each(m.recipient_begin(), m.recipient_end(), process_recipient);
}


void process_folder(const fairport::folder& f)
{
    using namespace std;
    using namespace fairport;

    //wcout << "Folder (M" << f.get_message_count() << ", F" << f.get_subfolder_count() << ") : " << f.get_name() << endl;
    (void)f.get_message_count(); (void) f.get_subfolder_count();
    (void)f.get_name();

    for_each(f.message_begin(), f.message_end(), process_message);

    for_each(f.sub_folder_begin(), f.sub_folder_end(), process_folder);
}

void process_pst(const fairport::pst& p)
{
    using namespace std;
    using namespace fairport;

    //wcout << "PST Name: " << p.get_name() << endl;
    (void)p.get_name();
    folder root = p.open_root_folder();
    process_folder(root);
}

BOOST_AUTO_TEST_SUITE( pstlevel )

BOOST_AUTO_TEST_CASE( entry_id )
{
    using namespace std;
    using namespace fairport;

    pst sample1(L"sample1.pst");
    pst submessage(L"submessage.pst");

    // Folder "Sample1" (000000006a552b813c43f94384f18b7da2393e9582800000)
    folder f(*(--sample1.folder_end()));
    static const fairport::byte expected1_bytes[24] = {
        0x00, 0x00, 0x00, 0x00, 0x6a, 0x55, 0x2b, 0x81, 0x3c, 0x43, 0xf9, 0x43,
        0x84, 0xf1, 0x8b, 0x7d, 0xa2, 0x39, 0x3e, 0x95, 0x82, 0x80, 0x00, 0x00, 
    };
    vector<fairport::byte> expected1(expected1_bytes, expected1_bytes + 24);
    vector<fairport::byte> actual1 = f.get_entry_id();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected1.begin(), expected1.end(), actual1.begin(), actual1.end());

    // Message (000000006a552b813c43f94384f18b7da2393e9524002000)
    message m(*sample1.message_begin());
    static const fairport::byte expected2_bytes[24] = {
        0x00, 0x00, 0x00, 0x00, 0x6a, 0x55, 0x2b, 0x81, 0x3c, 0x43, 0xf9, 0x43,
        0x84, 0xf1, 0x8b, 0x7d, 0xa2, 0x39, 0x3e, 0x95, 0x24, 0x00, 0x20, 0x00, 
    };
    vector<fairport::byte> expected2(expected2_bytes, expected2_bytes + 24);
    vector<fairport::byte> actual2 = m.get_entry_id();
    BOOST_CHECK_EQUAL_COLLECTIONS(expected2.begin(), expected2.end(), actual2.begin(), actual2.end());

    // Submessage should not have an entry ID.
    attachment a(*submessage.message_begin()->attachment_begin());
    message sm(a.open_as_message());
    BOOST_CHECK_THROW(sm.get_entry_id(), key_not_found<prop_id>);
}

BOOST_AUTO_TEST_CASE( folder_by_name )
{
    pst uni(L"test_unicode.pst");
    process_folder(uni.open_folder(L"Folder"));
}

BOOST_AUTO_TEST_CASE( process_ansi )
{
    pst ansi(L"test_ansi.pst");
    process_pst(ansi);
}

BOOST_AUTO_TEST_CASE( process_sample1 )
{
    pst s1(L"sample1.pst");
    process_pst(s1);
}

BOOST_AUTO_TEST_CASE( process_sample2 )
{
    pst s2(L"sample2.pst");
    process_pst(s2);
}

BOOST_AUTO_TEST_CASE( process_sub )
{
    pst submess(L"submessage.pst");
    process_pst(submess);
}

BOOST_AUTO_TEST_SUITE_END()
