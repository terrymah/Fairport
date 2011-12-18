#include <iostream>
#include <fstream>
#include "fairport/disk.h"
#include "fairport/util.h"

template<typename T>
void test_block(fairport::file& file, fairport::disk::block_reference<T>& ref, fairport::ushort size, fairport::byte) 
{
    using namespace fairport;
    using namespace std;
    size_t aligned_size = disk::align_disk<T>(size);

    std::vector<fairport::byte> buffer(aligned_size);
    disk::block_trailer<T>* bt = (disk::block_trailer<T>*)(&buffer[0] + aligned_size - sizeof(disk::block_trailer<T>));

    file.read(buffer, ref.ib);

    BOOST_CHECK_EQUAL(bt->cb, size);
    BOOST_CHECK_EQUAL(bt->signature, disk::compute_signature(ref));
    BOOST_CHECK_EQUAL(bt->crc, disk::compute_crc(&buffer[0], size));
}

template<typename T>
void test_page(fairport::file& file, fairport::disk::block_reference<T> ref, fairport::byte crypt_method)
{
    using namespace fairport;
    using namespace std;

    std::vector<fairport::byte> buffer(disk::page_size);
    disk::page<T>* ppage = (disk::page<T>*)&buffer[0];
    file.read(buffer, ref.ib);

    BOOST_CHECK_EQUAL(ppage->trailer.crc, disk::compute_crc(ppage, disk::page<T>::page_data_size));
    BOOST_CHECK_EQUAL(ppage->trailer.signature, disk::compute_signature(ref));
    BOOST_CHECK_EQUAL(ppage->trailer.page_type, ppage->trailer.page_type_repeat);

    switch(ppage->trailer.page_type)
    {
        case disk::page_type_bbt:
            if(((disk::bbt_leaf_page<T>*)(ppage))->level != 0)
            {
                disk::bbt_nonleaf_page<T>* nonleaf = (disk::bbt_nonleaf_page<T>*)ppage;
                for(int i = 0; i < nonleaf->num_entries; ++i)
                {
                    test_page<T>(file, nonleaf->entries[i].ref, crypt_method);
                }
            }
            else
            {
                disk::bbt_leaf_page<T>* leaf = (disk::bbt_leaf_page<T>*)ppage;
                for(int i = 0; i < leaf->num_entries; ++i)
                {
                    test_block<T>(file, leaf->entries[i].ref, leaf->entries[i].size, crypt_method);
                }
            }
            break;
        case disk::page_type_nbt:
            if(((disk::nbt_leaf_page<T>*)ppage)->level != 0)
            {
                disk::nbt_nonleaf_page<T>* nonleaf = (disk::nbt_nonleaf_page<T>*)ppage;
                for(int i = 0; i < nonleaf->num_entries; ++i)
                {
                    test_page<T>(file, nonleaf->entries[i].ref, crypt_method);
                }
            }
            break;
    }
}

template<typename T>
void test_disk_structures(fairport::file& file)
{
    using namespace fairport;
    using namespace std;

    vector<fairport::byte> buffer(sizeof(disk::header<T>));
    disk::header<T>* pheader = (disk::header<T>*)&buffer[0];

    file.read(buffer, 0);

    test_page<T>(file, pheader->root_info.brefNBT, pheader->bCryptMethod);
    test_page<T>(file, pheader->root_info.brefBBT, pheader->bCryptMethod);
}

BOOST_AUTO_TEST_SUITE( disk_structures )

BOOST_AUTO_TEST_CASE( test_disk_ansi )
{
    using namespace std;
    using namespace fairport;
    file ansi(L"test_ansi.pst");
    test_disk_structures<fairport::ulong>(ansi);
}

BOOST_AUTO_TEST_CASE( test_disk_unicode )
{
    using namespace std;
    using namespace fairport;
    file uni(L"test_unicode.pst");
    test_disk_structures<fairport::ulonglong>(uni);
}

BOOST_AUTO_TEST_SUITE_END()
