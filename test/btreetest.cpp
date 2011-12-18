#include <iostream>
#include <string>
#include <cstring>
#include "fairport/util/btree.h"

using namespace fairport;
using namespace std;

class leaf : public btree_node_leaf<int, string>
{
public:
    leaf(int k1, string v1, int k2, string v2, int k3, string v3);
    ~leaf() { }

    const string& get_value(fairport::uint pos) const 
        { return values[pos]; }
    const int& get_key(fairport::uint pos) const 
        { return keys[pos]; }
    fairport::uint num_values() const 
        { return 3; }

private:
    string values[3];
    int keys[3];
};

class non_leaf : public btree_node_nonleaf<int, string>
{
public:
    non_leaf(int k1, leaf* l1, int k2, leaf* l2, int k3, leaf* l3);
    ~non_leaf() { }

    const int& get_key(fairport::uint pos) const 
        { return keys[pos]; }
    btree_node<int,string>* get_child(fairport::uint i)
        { return leafs[i]; }
    const btree_node<int,string>* get_child(fairport::uint i) const 
        { return leafs[i]; }
    fairport::uint num_values() const 
        { return 3; }

private:
    int keys[3];
    leaf* leafs[3];
};

non_leaf::non_leaf(int k1, leaf* l1, int k2, leaf* l2, int k3, leaf* l3)
{
    keys[0] = k1;
    keys[1] = k2;
    keys[2] = k3;

    leafs[0] = l1;
    leafs[1] = l2;
    leafs[2] = l3;
}

leaf::leaf(int k1, string v1, int k2, string v2, int k3, string v3)
{
    keys[0] = k1;
    keys[1] = k2;
    keys[2] = k3;

    values[0] = v1;
    values[1] = v2;
    values[2] = v3;
}

BOOST_AUTO_TEST_SUITE( btree )

BOOST_AUTO_TEST_CASE( test_btree )
{

    leaf l1(0, "zero", 1, "one", 2, "two");
    leaf l2(3, "three", 4, "four", 5, "five");
    leaf l3(6, "six", 7, "seven", 8, "eight");
    non_leaf nl(0, &l1, 3, &l2, 6, &l3);

    const char * results[] = {
        "zero",
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight"
    };

    for(int i = 0; i < 9; ++i)
    {
        BOOST_CHECK_EQUAL(strcmp(nl.lookup(i).c_str(), results[i]), 0);
    }

    BOOST_CHECK_THROW(nl.lookup(10), key_not_found<int>);
    BOOST_CHECK_THROW(nl.lookup(-1), key_not_found<int>);

    int i = 0;
    for(non_leaf::const_iterator iter = nl.begin(); 
            iter != nl.end(); 
            ++iter, ++i)
    {
        BOOST_CHECK_EQUAL(strcmp(iter->c_str(), results[i]), 0);
    }

    const non_leaf& nlr = nl;
    for(non_leaf::const_iterator citer = nlr.begin();
            citer != nlr.end();
            ++citer)
            {
            }

    non_leaf::const_iterator i2 = nl.end();
    int j = 9;
    while(i2 != nl.begin())
    {
        BOOST_CHECK_EQUAL(strcmp((--i2)->c_str(), results[--j]), 0);
    }

}

BOOST_AUTO_TEST_SUITE_END()
