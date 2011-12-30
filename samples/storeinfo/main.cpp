#include <iostream>
#include <vector>

#include "fairport/ndb/database.h"

#include "fairport/ltp/propbag.h"

using namespace fairport;

int main(int, char* argv[])
{
    std::string name(argv[1]);
    std::wstring wname(name.begin(), name.end());

    shared_db_ptr db(open_database(wname));
    property_bag store(db->lookup_node(nid_message_store));

    std::vector<prop_id> props = store.get_prop_list();
    for(size_t i = 0; i < props.size(); ++i)
    {
        printf("0x%X\n", props[i]);
    }
}
