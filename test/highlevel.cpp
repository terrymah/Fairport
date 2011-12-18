#include <iostream>
#include <string>
#include "fairport/ndb.h"
#include "fairport/ltp.h"


void test_prop_stream(fairport::const_property_object& obj, fairport::prop_id id)
{
    fairport::prop_stream stream(obj.open_prop_stream(id));
    std::vector<fairport::byte> contents = obj.read_prop<std::vector<fairport::byte> >(id);
    fairport::byte b;
    size_t pos = 0;

    BOOST_CHECK_EQUAL(contents.size(), obj.size(id));

    stream.unsetf(std::ios::skipws);
    while(stream >> b)
        BOOST_CHECK_EQUAL(b, contents[pos++]);
}

void test_table(const fairport::table& tc)
{
    using namespace std;
    using namespace fairport;

    //wcout << "Properties on this table (" << tc.size() << "): " << endl;
    (void)tc.size();

    std::vector<prop_id> prop_list = tc.get_prop_list();

    for(fairport::uint i = 0; i < tc.size(); ++i)
    {
        //wcout << "RowID: " << tc[i].get_row_id() << endl;
        (void)tc[i].get_row_id();

        std::vector<fairport::ushort> proplist(tc[i].get_prop_list());
        for(fairport::uint j = 0; j < proplist.size(); ++j)
        {
            try {
                if(tc[i].get_prop_type(proplist[j]) == prop_type_wstring)
                {
                    //wcout << "\t" << hex << proplist[j] << ": " << tc[i].read_prop<std::wstring>(proplist[j]) << endl;
                    (void)tc[i].read_prop<std::wstring>(proplist[j]);
                }
                else if(tc[i].get_prop_type(proplist[j]) == prop_type_long)
                {
                    //wcout << "\t" << hex << proplist[j] << ": " << dec << tc[i].read_prop<slong>(proplist[j]) << endl;
                    (void)tc[i].read_prop<slong>(proplist[j]);
                }
                else if(tc[i].get_prop_type(proplist[j]) == prop_type_boolean)
                {
                    //wcout << "\t" << hex << proplist[j] << ": " << dec << (tc[i].read_prop<bool>(proplist[j]) ? L"true" : L"false") << endl;
                    (void)tc[i].read_prop<bool>(proplist[j]);
                }
                else if(tc[i].get_prop_type(proplist[j]) == prop_type_binary)
                {
                    const_table_row row = tc[i];
                    test_prop_stream(row, proplist[j]);
                }
                else if(tc[i].get_prop_type(proplist[j]) == prop_type_guid)
                {
                    (void)tc[i].read_prop<guid>(proplist[j]);
                }
                else
                {
                    //wcout << "\t" << hex << proplist[j] << "(" << dec << tc[i].get_prop_type(proplist[j]) << ")" << endl;
                    (void)tc[i].get_prop_type(proplist[j]);
                }
            } catch(key_not_found<prop_id>&)
            {
            }
        }
    }
}

void test_attachment_table(const fairport::node& message, const fairport::table& tc)
{
    using namespace std;
    using namespace fairport;
    for(fairport::uint i = 0; i < tc.size(); ++i)
    {
        node attach = message.lookup(tc[i].get_row_id());

        property_bag pc(attach);
        std::vector<fairport::ushort> proplist(pc.get_prop_list());
        for(fairport::uint i = 0; i < proplist.size(); ++i)
        {
            if(pc.get_prop_type(proplist[i]) == prop_type_wstring)
            {
                (void)pc.read_prop<std::wstring>(proplist[i]);
            }
            else if(pc.get_prop_type(proplist[i]) == prop_type_long)
            {
                (void)pc.read_prop<slong>(proplist[i]);
            }
            else if(pc.get_prop_type(proplist[i]) == prop_type_boolean)
            {
                (void)pc.read_prop<bool>(proplist[i]);
            }
            else if(pc.get_prop_type(proplist[i]) == prop_type_binary)
            {
                test_prop_stream(pc, proplist[i]);
            }
            else
            {
                (void)pc.get_prop_type(proplist[i]);
            }
        }
    }
}

void iterate(fairport::shared_db_ptr pdb)
{
    using namespace std;
    using namespace std::tr1;
    using namespace fairport;
    std::tr1::shared_ptr<const nbt_page> nbt_root = pdb->read_nbt_root();
    for(const_nodeinfo_iterator iter = nbt_root->begin();
            iter != nbt_root->end();
            ++iter)
    {
        node n(pdb, *iter);
        std::vector<fairport::byte> buffer(n.size());
        n.read(buffer, 0);

        try
        {
            property_bag bag(n);
            std::vector<fairport::ushort> proplist(bag.get_prop_list());

            // look for mv props
            for(fairport::uint i = 0; i < proplist.size(); ++i)
            {
                switch(bag.get_prop_type(proplist[i]))
                {
                case prop_type_mv_wstring:
                    (void)bag.read_prop_array<wstring>(proplist[i]);
                    break;
                case prop_type_mv_string:
                    (void)bag.read_prop_array<string>(proplist[i]);
                    break;
                case prop_type_mv_binary:
                    (void)bag.read_prop_array<vector<fairport::byte> >(proplist[i]);
                    break;
                default:
                    break;
                }
            }
        } 
        catch(exception&)
        {
        }


        if(get_nid_type(n.get_id()) == nid_type_message)
        {
            property_bag pc(n);
            std::vector<fairport::ushort> proplist(pc.get_prop_list());
            for(fairport::uint i = 0; i < proplist.size(); ++i)
            {
                if(pc.get_prop_type(proplist[i]) == prop_type_wstring)
                {
                    /*wcout <<*/ pc.read_prop<std::wstring>(proplist[i])/* << endl*/;
                }
                else if(pc.get_prop_type(proplist[i]) == prop_type_binary)
                {
                    test_prop_stream(pc, proplist[i]);
                }
            }

            // attachment table
            for(const_subnodeinfo_iterator si = n.subnode_info_begin();
                    si != n.subnode_info_end();
                    ++si)
            {
                if(get_nid_type(si->id) == nid_type_attachment_table)
                {
                    table atc(node(n, *si));
                    (void)atc.size();
                    test_table(atc);
                    test_attachment_table(n, atc);
                }

                if(get_nid_type(si->id) == nid_type_recipient_table)
                {
                    table atc(node(n, *si));
                    (void)atc.size();
                    test_table(atc);
                    //test_attachment_table(n, atc);
                }
            }

        }

        try{
            heap h(n);
            std::tr1::shared_ptr<bth_node<fairport::ushort, disk::prop_entry> > bth = h.open_bth<fairport::ushort, disk::prop_entry>(h.get_root_id());
         }
        catch(exception&)
        {
        }

        if(get_nid_type(n.get_id()) == nid_type_contents_table)
        {
            table tc(n);
            //wcout << "Found TC: " << tc.size() << endl;
            test_table(tc);
        }
        else if(get_nid_type(n.get_id()) == nid_type_associated_contents_table)
        {
            table tc(n);
            //wcout << "Found Associated TC: " << tc.size() << endl;
            test_table(tc);
        }
        else if(get_nid_type(n.get_id()) == nid_type_hierarchy_table)
        {
            table tc(n);
            //wcout << "Found Hierarchy TC: " << tc.size() << endl;
            test_table(tc);
        }
    }
}

BOOST_AUTO_TEST_SUITE( highlevel )

BOOST_AUTO_TEST_CASE( name_idmap )
{
    using namespace fairport;
    shared_db_ptr pdb = open_database(L"sample1.pst");
    name_id_map nm(pdb);

    const guid g1 = { 0x20386, 0, 0, { 0xc0, 0, 0, 0, 0, 0, 0, 0x46 } };
    const guid g2 = { 0x62002, 0, 0, { 0xc0, 0, 0, 0, 0, 0, 0, 0x46 } };

    // is the count correct?
    BOOST_CHECK_EQUAL(nm.get_prop_count(), 172);

    // test that the lookup succeeds and matches a few well known named props
    named_prop t1(ps_public_strings, L"urn:schemas-microsoft-com:office:outlook#storetypeprivate");
    BOOST_CHECK_EQUAL(nm.lookup(t1), 0x800f);
    BOOST_CHECK(nm.lookup(0x800f).get_name() == L"urn:schemas-microsoft-com:office:outlook#storetypeprivate");

    named_prop t2(ps_public_strings, L"Keywords");
    BOOST_CHECK_EQUAL(nm.lookup(t2), 0x8012);
    BOOST_CHECK(nm.lookup(0x8012).get_name() == L"Keywords");

    named_prop t3(g1, L"x-ms-exchange-organization-authdomain");
    BOOST_CHECK_EQUAL(nm.lookup(t3), 0x801c);
    BOOST_CHECK(nm.lookup(0x801c).get_name() == L"x-ms-exchange-organization-authdomain");

    named_prop t4(g2, 0x8233);
    BOOST_CHECK_EQUAL(nm.lookup(t4), 0x8008);
    BOOST_CHECK_EQUAL(nm.lookup(0x8008).get_id(), 0x8233);

    named_prop t5(g2, 0x8205);
    BOOST_CHECK_EQUAL(nm.lookup(t5), 0x8000);
    BOOST_CHECK_EQUAL(nm.lookup(0x8000).get_id(), 0x8205);

    // test that the lookup fails for a few known-to-not-exist named props
    named_prop t6(ps_public_strings, L"fake-property");
    BOOST_CHECK_THROW(nm.lookup(t6), key_not_found<named_prop>);
}

BOOST_AUTO_TEST_CASE( iterate_uni )
{
    iterate(open_database(L"test_unicode.pst"));
}

BOOST_AUTO_TEST_CASE( iterate_ansi )
{
    iterate(open_database(L"test_ansi.pst"));
}

BOOST_AUTO_TEST_CASE( iterate_sample1 )
{
    iterate(open_database(L"sample1.pst"));
}

BOOST_AUTO_TEST_CASE( iterate_sample2 )
{
    iterate(open_database(L"sample2.pst"));
}

BOOST_AUTO_TEST_CASE( iterate_submessage )
{
    iterate(open_database(L"submessage.pst"));
}

BOOST_AUTO_TEST_SUITE_END()
