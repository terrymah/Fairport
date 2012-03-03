//! \file
//! \brief PST Utility related classes and functions
//! \author Terry Mahaffey
//!
//! General utility classes and functions common across the PST layer go here
//! \ingroup pst

#ifndef FAIRPORT_PST_PSTUTIL_H
#define FAIRPORT_PST_PSTUTIL_H

#include <vector>

#include "fairport/ndb/database_iface.h"
#include "fairport/ndb/node.h"

#include "fairport/ltp/propbag.h"
#include "fairport/ltp/table.h"

namespace fairport
{

//! \defgroup pst_utilrelated PST Utilities
//! \ingroup pst

//! \brief Construct a MAPI entry id from a given nid
//! \param[in] db The database
//! \param[in] n The node id
//! \returns The entry id
//! \ingroup pst_utilrelated
std::vector<byte> calculate_entry_id(const shared_db_ptr& db, node_id nid);

namespace detail
{

class itembase
{
public:
    //! \brief Constructs an itembase
    //! \param[in] n An item node
    explicit itembase(const node& n)
        : m_bag(n) { }

    //! \brief Get the node_id of this item
    //! \returns The node_id of the item
    node_id get_id() const
        { return m_bag.get_node().get_id(); }

    //! \brief Get the MAPI entry ID of this item
    //! \returns The MAPI entry ID of this item
    std::vector<byte> get_entry_id() const;

    // lower layer access
    //! \brief Get the property bag backing this item
    //! \returns The property bag
    property_bag& get_property_bag()
        { return m_bag; }
    //! \copydoc itembase::get_property_bag()
    const property_bag& get_property_bag() const
        { return m_bag; }

    //! \brief Get the database pointer used by this folder
    //! \returns The database pointer
    shared_db_ptr get_db() const 
        { return m_bag.get_node().get_db(); }

private:
    property_bag m_bag;
};

template<node_id Type>
struct is_nid_type
{
    bool operator()(const node_info& info)
        { return get_nid_type(info.id) == Type; }
    bool operator()(const const_table_row& row)
        { return get_nid_type(row.get_row_id()) == Type; }
};

struct is_item_type
{
    is_item_type(std::wstring message_class) 
        : m_message_class(message_class) { }

    bool operator()(const const_property_object& po)
        { return m_message_class == po.read_prop<std::wstring>(0x1a); }

    std::wstring m_message_class;
};

template<typename T>
class item_transform_row : public std::unary_function<const_table_row, T>
{
public:
    item_transform_row(const shared_db_ptr& db) 
        : m_db(db) { }
    T operator()(const const_table_row& row) const
        { return T(m_db->lookup_node(row.get_row_id())); }

private:
    shared_db_ptr m_db;
};

template<typename T>
class item_transform_info : public std::unary_function<node_info, T>
{
public:
    item_transform_info(const shared_db_ptr& db) 
        : m_db(db) { }
    T operator()(const node_info& info) const
        { return T(node(m_db, info)); }

private:
    shared_db_ptr m_db;
};
} // end namespace detail
} // end namespace fairport

inline std::vector<fairport::byte> fairport::calculate_entry_id(const shared_db_ptr& db, node_id nid)
{
    using namespace std;

    // This function doesn't know how to handle anything other than vanilla
    // PSTs.  OSTs, for example, require a more complex calculation.
    if (!db->is_pst())
        throw key_not_found<prop_id>(0x0fff);

    // A MAPI entry id contains 4 leading 0 bytes, the data store ID, and
    // the node ID (in little-endian byte order).
    vector<byte> entry_id(4, 0);

    node store(db->lookup_node(nid_message_store));
    property_bag store_props(store);
    vector<byte> store_id(store_props.read_prop<vector<byte> >(0x0ff9));
    copy(store_id.begin(), store_id.end(),
         insert_iterator<vector<byte> >(entry_id, entry_id.end()));

    for (size_t i = 0; i < sizeof(node_id); ++i)
        entry_id.push_back((nid >> 8*i) & 0xff);

    return entry_id;
}

inline std::vector<fairport::byte> fairport::detail::itembase::get_entry_id() const
{
    if (m_bag.get_node().is_subnode())
    {
        // subitems should not have PidTagEntryId.
        throw key_not_found<prop_id>(0x0fff);
    }
    else
    {
        return calculate_entry_id(m_bag.get_node().get_db(), get_id());
    }
}
#endif
