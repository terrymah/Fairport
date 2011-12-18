#include <iostream>
#include "fairport/disk/disk.h"
#include "fairport/ndb.h"

struct n_info
{
    fairport::node_id node;
    fairport::node_id parent;
};

const n_info node_info_uni[] = {
    { 33, 0 }, { 97, 0 }, { 290, 290 }, { 301, 0 }, { 302, 0 }, { 303, 0 },
    { 481, 0 }, { 513, 0 }, { 609, 0 }, { 641, 0 }, { 673, 0 }, { 801, 0 },
    { 1549, 0 }, { 1550, 0 }, { 1551, 0 }, { 1552, 0 }, { 1579, 0 },
    { 1612, 0 }, { 1649, 0 }, { 1682, 0 }, { 1718, 0 }, { 1751, 0 },
    { 1784, 0 }, { 3073, 0 }, { 3585, 0 }, { 3649, 0 }, { 8739, 290 }, 
    { 8742, 0 }, { 8743, 0 }, { 8752, 0 }, { 32802, 290 }, {32813, 0 },
    { 32814, 0 }, { 32815, 0 }, { 32834, 290 }, { 32845, 0 }, { 32846, 0 },
    { 32847, 0 }, { 32866, 32802 }, { 32877, 0 }, { 32878, 0 }, { 32879, 0 },
    { 32898, 32802 }, { 32909, 0 }, { 32910, 0 }, { 32911, 0 },
    { 2097188, 32802 }, { 2097220, 32898 }
};

const n_info node_info_ansi[] = {
    { 33, 0 }, { 97, 0 }, { 290, 290 }, { 301, 0 }, { 302, 0 }, { 303, 0 },
    { 481, 0 }, { 513, 0 }, { 609, 0 }, { 641, 0 }, { 673, 0 }, { 801, 0 },
    { 1549, 0 }, { 1550, 0 }, { 1551, 0 }, { 1552, 0 }, { 1579, 0 },
    { 1612, 0 }, { 1649, 0 }, { 1682, 0 }, { 1718, 0 }, { 1751, 0 },
    { 1784, 0 }, { 3073, 0 }, { 3585, 0 }, { 3649, 0 }, { 8739, 290 }, 
    { 8742, 0 }, { 8743, 0 }, { 8752, 0 }, { 32802, 290 }, {32813, 0 },
    { 32814, 0 }, { 32815, 0 }, { 32834, 290 }, { 32845, 0 }, { 32846, 0 },
    { 32847, 0 }, { 32866, 32802 }, { 32877, 0 }, { 32878, 0 }, { 32879, 0 },
    { 32898, 32802 }, { 32909, 0 }, { 32910, 0 }, { 32911, 0 },
    { 2097188, 32898 } 
};

struct b_info
{
    fairport::block_id block;
    fairport::ushort size;
    fairport::ushort refs;
};

const b_info block_info_uni[] = {
    { 4, 156, 4 }, { 8, 268, 4 }, { 12, 172, 4 }, { 16, 204, 3 },
    { 20, 164, 2 }, { 24, 100, 2 }, { 36, 92, 2 }, { 40, 124, 2 },
    { 44, 84, 2 }, { 48, 114, 2 }, { 72, 34, 2 }, { 100, 62, 2 },
    { 104, 86, 2 }, { 120, 88, 2 }, { 132, 104, 2 }, { 140, 38, 2 },
    { 156, 140, 2 }, { 176, 274, 2 }, { 188, 252, 2 }, { 192, 228, 2 },
    { 216, 4, 2 }, { 220, 188, 2 }, { 228, 3506, 2 }, { 232, 1312, 2 },
    { 238, 32, 2 }, { 240, 1655, 2 }, { 246, 32, 2 }, { 252, 1248, 2 },
    { 260, 852, 2 }, { 272, 464, 2 }, { 284, 484, 2 }, { 324, 1655, 2 }, 
    { 330, 32, 2 }, { 336, 1248, 2 }, { 344, 852, 2 }, { 352, 118, 2 },
    { 356, 510, 2 }, { 360, 116, 2 }, { 364, 228, 2 }, { 368, 142, 2 },
    { 372, 132, 2 }
};

const b_info block_info_ansi[] = {
    { 4, 156, 4 }, { 8, 268, 5 }, { 12, 172, 4 }, { 16, 204, 3 },
    { 20, 164, 2 }, { 24, 100, 2 }, { 36, 92, 2 }, { 40, 124, 2 },
    { 44, 84, 2 }, { 48, 112, 2 }, { 72, 34, 2 }, { 100, 62, 2 },
    { 104, 76, 2 }, { 120, 88, 2 }, { 132, 84, 2 }, { 140, 38, 2 },
    { 156, 108, 2 }, { 172, 404, 2 }, { 176, 258, 2 }, { 188, 252, 2 },
    { 192, 228, 2 }, { 220, 4, 2 }, { 224, 484, 2 }, { 228, 188, 2 },
    { 268, 2788, 2 }, { 272, 1312, 2 }, { 276, 716, 2 }, { 282, 28, 2 },
    { 284, 1655, 2 }, { 290, 16, 2 }, { 296, 1024, 2 }, { 304, 818, 2 },
    { 312, 104, 2 }, { 316, 480, 2 }, { 320, 102, 2 }, { 324, 228, 2 }, 
    { 328, 104, 2 }, { 332, 132, 2 }
};

void process_node(const fairport::node& n)
{
    using namespace std;
    using namespace fairport;

    for(const_subnodeinfo_iterator iter = n.subnode_info_begin();
                    iter != n.subnode_info_end();
                    ++iter)
    {
        process_node(node(n, *iter));
    }

}

void test_node_stream(fairport::node n)
{
    using namespace std;
    using namespace fairport;

    vector<fairport::byte> contents(n.size());
    fairport::byte b;
    int i = 0;
    node_stream stream(n.open_as_stream());
    stream.unsetf(ios::skipws);

    (void)n.read(contents, 0);

    // pick a larger node if this fires. I just want to make sure it's non-trivial.
    BOOST_CHECK_MESSAGE(n.size() > 100, "Sample node too small");

    while(stream >> b)
    {
        fairport::byte c = contents[i];
        BOOST_CHECK_EQUAL(b, c);
        ++i;
    }

    // test seeking from the beginning
    stream.clear();
    stream.seekg(0, ios_base::beg);
    stream.seekg( 10, ios_base::beg );
    BOOST_CHECK_EQUAL((int)stream.tellg(), 10);
    stream >> b;
    BOOST_CHECK_EQUAL((int)stream.tellg(), 11);
    BOOST_CHECK_EQUAL(b, contents[10]);

    // test seeking from current
    stream.seekg( 50, ios_base::cur );
    BOOST_CHECK_EQUAL((int)stream.tellg(), 61);
    stream >> b;
    BOOST_CHECK_EQUAL(b, contents[61]);

    // test seeking from end
    stream.seekg( -20, ios_base::end );
    BOOST_CHECK_EQUAL((int)stream.tellg(), (int)(n.size()-20));
    stream >> b;
    BOOST_CHECK_EQUAL(b, contents[ n.size() - 5 ]);
}

void node_check(fairport::shared_db_ptr db, const n_info* ni)
{
    using namespace std;
    using namespace std::tr1;
    using namespace fairport;
    fairport::uint node = 0;

    std::tr1::shared_ptr<const nbt_page> nbt_root = db->read_nbt_root();
    for(const_nodeinfo_iterator iter = nbt_root->begin();
                    iter != nbt_root->end();
                    ++iter, ++node)
    {
        BOOST_CHECK_EQUAL(iter->id, ni[node].node);
        BOOST_CHECK_EQUAL(iter->parent_id, ni[node].parent);
        fairport::node n(db, *iter);
        process_node(n);
    }
}

void block_check(fairport::shared_db_ptr db, const b_info* bi)
{
    using namespace std;
    using namespace std::tr1;
    using namespace fairport;
    fairport::uint block = 0;

    std::tr1::shared_ptr<const bbt_page> bbt_root = db->read_bbt_root();
    for(const_blockinfo_iterator iter = bbt_root->begin();
                    iter != bbt_root->end();
                    ++iter, ++block)
    {
        BOOST_CHECK_EQUAL(iter->id, bi[block].block);
        BOOST_CHECK_EQUAL(iter->size, bi[block].size);
        BOOST_CHECK_EQUAL(iter->ref_count, bi[block].refs);
    }
}

BOOST_AUTO_TEST_SUITE( database )

BOOST_AUTO_TEST_CASE( db_open )
{
    using namespace fairport;

    BOOST_CHECK_THROW(open_large_pst(L"test_ansi.pst"), invalid_format);
    BOOST_CHECK_THROW(open_small_pst(L"test_unicode.pst"), invalid_format);

    BOOST_CHECK_NO_THROW(open_large_pst(L"test_unicode.pst"));
    BOOST_CHECK_NO_THROW(open_small_pst(L"test_ansi.pst"));
}

BOOST_AUTO_TEST_CASE( node_check_uni )
{
    node_check(open_large_pst(L"test_unicode.pst"), node_info_uni);
}

BOOST_AUTO_TEST_CASE( node_check_ansi )
{
    node_check(open_small_pst(L"test_ansi.pst"), node_info_ansi);
}

BOOST_AUTO_TEST_CASE( block_check_uni )
{
    block_check(open_large_pst(L"test_unicode.pst"), block_info_uni);
}

BOOST_AUTO_TEST_CASE( block_check_ansi ) 
{ 
    block_check(open_small_pst(L"test_ansi.pst"), block_info_ansi);
}

BOOST_AUTO_TEST_CASE( test_node_stream_uni )
{
    using namespace fairport;
    test_node_stream(open_database(L"test_unicode.pst")->lookup_node(nid_message_store));
}

BOOST_AUTO_TEST_CASE( test_node_stream_ansi )
{
    using namespace fairport;
    test_node_stream(open_database(L"test_ansi.pst")->lookup_node(nid_message_store));
}

BOOST_AUTO_TEST_SUITE_END()
