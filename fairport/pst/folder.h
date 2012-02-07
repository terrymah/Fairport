//! \file
//! \brief Folder objects
//!
//! Defines the folder and search folder abstractions, as well as the
//! transformations used by the boost iterator library to create the folder
//! iterators. Also defines a generic filter used to filter by node type,
//! used by both the folder object and pst object to filter through to the
//! specific nodes of interest when creating iterators.
//! \author Terry Mahaffey
//! \ingroup pst

#ifndef FAIRPORT_PST_FOLDER_H
#define FAIRPORT_PST_FOLDER_H

#include <algorithm>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include "fairport/util/primitives.h"
#include "fairport/util/errors.h"

#include "fairport/ndb/database_iface.h"

#include "fairport/ltp/propbag.h"
#include "fairport/ltp/table.h"

#include "fairport/pst/message.h"
#include "fairport/pst/contact.h"
#include "fairport/pst/task.h"
#include "fairport/pst/pstutil.h"

namespace fairport
{

//! \defgroup pst_folderrelated Folder Objects
//! \ingroup pst

//! \brief Search Folder object
//!
//! Search folders are different from regular folders mainly in that they
//! do not have a hierarchy table (and thus no subfolders). The messages
//! they "contain" are actually in other folders.
//!
//! This object exists to reflect that limited interface. Eventually this
//! object may support querying the criteria used to create the search folder.
//! \ingroup pst_folderrelated
class search_folder : public detail::itembase
{
    typedef boost::filter_iterator<detail::is_item_type, const_table_row_iter> row_filter_iterator;
public:
    //! \brief Message iterator type; a transform iterator over a table row iterator
    typedef boost::transform_iterator<message_transform_row, const_table_row_iter> message_iterator;
    //! \brief Contact iterator type; a transform iterator over a filter iterator over a table row iterator
    typedef boost::transform_iterator<contact_transform_row, row_filter_iterator> contact_iterator;
    //! \brief Task iterator type; a transform iterator over a filter iterator over a table row iterator
    typedef boost::transform_iterator<task_transform_row, row_filter_iterator> task_iterator;

    //! \brief Construct a search folder object
    //! \param[in] n A search folder node
    search_folder(const node& n)
        : detail::itembase(n) { }

    // subobject discovery/enumeration
    //! \brief Get an iterator to the first message in this folder
    //! \returns an iterator positioned on the first message in this folder
    message_iterator message_begin() const
        { return boost::make_transform_iterator(get_contents_table().begin(), message_transform_row(get_db())); }
    //! \brief Get the end message iterator
    //! \returns an iterator at the end position
    message_iterator message_end() const
        { return boost::make_transform_iterator(get_contents_table().end(), message_transform_row(get_db())); }

    //! \brief Get an iterator to the first contact in this folder
    //! \returns an iterator positioned on the first contact in this folder
    contact_iterator contact_begin() const
        { return boost::make_transform_iterator(boost::make_filter_iterator(detail::is_item_type(contact_class), get_contents_table().begin(), get_contents_table().end()), contact_transform_row(get_db())); }
    //! \brief Get the end contact iterator
    //! \returns an iterator at the end position
    contact_iterator contact_end() const
        { return boost::make_transform_iterator(boost::make_filter_iterator(detail::is_item_type(contact_class), get_contents_table().end(), get_contents_table().end()), contact_transform_row(get_db())); }

    //! \brief Get an iterator to the first task in this folder
    //! \returns an iterator positioned on the first task in this folder
    task_iterator task_begin() const
        { return boost::make_transform_iterator(boost::make_filter_iterator(detail::is_item_type(task_class), get_contents_table().begin(), get_contents_table().end()), task_transform_row(get_db())); }
    //! \brief Get the end contact iterator
    //! \returns an iterator at the end position
    task_iterator task_end() const
        { return boost::make_transform_iterator(boost::make_filter_iterator(detail::is_item_type(task_class), get_contents_table().end(), get_contents_table().end()), task_transform_row(get_db())); }

    // property access
    //! \brief Get the display name of this folder
    //! \returns The name of this folder
    std::wstring get_name() const
        { return get_property_bag().read_prop<std::wstring>(0x3001); }
    //! \brief Get the number of unread messages in this folder
    //! \returns The number of unread messages
    size_t get_unread_message_count() const
        { return get_property_bag().read_prop<slong>(0x3603); }
    //! \brief Get the number of messages in this folder
    //! \returns The number of messages
    size_t get_message_count() const
        { return get_property_bag().read_prop<slong>(0x3602); }

    // lower layer access

    //! \brief Get the contents table of this folder
    //! \returns The contents table
    table& get_contents_table();
    //! \copydoc search_folder::get_contents_table()
    const table& get_contents_table() const;

private:
    mutable std::tr1::shared_ptr<table> m_contents_table;
};

class folder;
typedef detail::item_transform_row<folder> folder_transform_row;
typedef detail::item_transform_row<search_folder> search_folder_transform_row;

//! \brief A folder in a PST file
//!
//! The folder object allows access to subfolders, messages, and associated 
//! messages which are contained in the folder. Similar to the \ref pst object,
//! the folder also offers a way to lookup subfolders by name.
//!
//! A folder currently doesn't have a concept of sorting. This was deemed
//! unnecessary because of the iterator based approach used for exposing
//! sub messages and folders - one can use these iterators to build up a 
//! container of messages or folders to be sorted, and calling std::sort
//! directly with an arbitrary sorting functor.
//! \ingroup pst_folderrelated
class folder : public detail::itembase
{
    typedef boost::filter_iterator<detail::is_item_type, const_table_row_iter> row_filter_iterator;
    typedef boost::filter_iterator<detail::is_nid_type<nid_type_search_folder>, const_table_row_iter> search_folder_filter_iterator;
    typedef boost::filter_iterator<detail::is_nid_type<nid_type_folder>, const_table_row_iter> folder_filter_iterator;

public:
    //! \brief Message iterator type; a transform iterator over a table row iterator
    typedef boost::transform_iterator<message_transform_row, const_table_row_iter> message_iterator;
    //! \brief Folder iterator type; a transform iterator over a filter iterator over table row iterator
    typedef boost::transform_iterator<folder_transform_row, folder_filter_iterator> folder_iterator;
    //! \brief Search folder iterator type; a transform iterator over a filter iterator over table row iterator
    typedef boost::transform_iterator<search_folder_transform_row, search_folder_filter_iterator> search_folder_iterator;
    //! \brief Contact iterator type; a transform iterator over a filter iterator over a table row iterator
    typedef boost::transform_iterator<contact_transform_row, row_filter_iterator> contact_iterator;
    //! \brief Task iterator type; a transform iterator over a filter iterator over a table row iterator
    typedef boost::transform_iterator<task_transform_row, row_filter_iterator> task_iterator;

    //! \brief Construct a folder object
    //! \param[in] n A folder node
    folder(const node& n)
        : detail::itembase(n) { }

    // subobject discovery/enumeration
    //! \brief Get an iterator to the first folder in this folder
    //! \returns an iterator positioned on the first folder in this folder
    folder_iterator sub_folder_begin() const
        { return boost::make_transform_iterator(boost::make_filter_iterator<detail::is_nid_type<nid_type_folder> >(get_hierarchy_table().begin(), get_hierarchy_table().end()), folder_transform_row(get_db())); }
    //! \brief Get the end folder iterator
    //! \returns an iterator at the end position
    folder_iterator sub_folder_end() const
        { return boost::make_transform_iterator(boost::make_filter_iterator<detail::is_nid_type<nid_type_folder> >(get_hierarchy_table().end(), get_hierarchy_table().end()), folder_transform_row(get_db())); }

    //! \brief Get an iterator to the first search folder in this folder
    //! \returns an iterator positioned on the first search folder in this folder
    search_folder_iterator sub_search_folder_begin() const
        { return boost::make_transform_iterator(boost::make_filter_iterator<detail::is_nid_type<nid_type_search_folder> >(get_hierarchy_table().begin(), get_hierarchy_table().end()), search_folder_transform_row(get_db())); }
    //! \brief Get the end search folder iterator
    //! \returns an iterator at the end position
    search_folder_iterator sub_search_folder_end() const
        { return boost::make_transform_iterator(boost::make_filter_iterator<detail::is_nid_type<nid_type_search_folder> >(get_hierarchy_table().end(), get_hierarchy_table().end()), search_folder_transform_row(get_db())); }

    //! \brief Open a specific subfolder in this folder, not recursive
    //! \param[in] name The name of the folder to open
    //! \throws key_not_found<std::wstring> If a folder of the specified name was not found in this folder
    //! \returns The first folder by that name found in this folder
    folder open_sub_folder(const std::wstring& name);

    //! \copydoc search_folder::message_begin()
    message_iterator message_begin() const
        { return boost::make_transform_iterator(get_contents_table().begin(), message_transform_row(get_db())); }
    //! \copydoc search_folder::message_end()
    message_iterator message_end() const
        { return boost::make_transform_iterator(get_contents_table().end(), message_transform_row(get_db())); }
    
    //! \copydoc search_folder::contact_begin()
    contact_iterator contact_begin() const
        { return boost::make_transform_iterator(boost::make_filter_iterator(detail::is_item_type(contact_class), get_contents_table().begin(), get_contents_table().end()), contact_transform_row(get_db())); }
    //! \copydoc search_folder::contact_end()
    contact_iterator contact_end() const
        { return boost::make_transform_iterator(boost::make_filter_iterator(detail::is_item_type(contact_class), get_contents_table().end(), get_contents_table().end()), contact_transform_row(get_db())); }

    //! \copydoc search_folder::task_begin()
    task_iterator task_begin() const
        { return boost::make_transform_iterator(boost::make_filter_iterator(detail::is_item_type(task_class), get_contents_table().begin(), get_contents_table().end()), task_transform_row(get_db())); }
    //! \copydoc search_folder::task_end()
    task_iterator task_end() const
        { return boost::make_transform_iterator(boost::make_filter_iterator(detail::is_item_type(task_class), get_contents_table().end(), get_contents_table().end()), task_transform_row(get_db())); }

    //! \brief Get an iterator to the first associated message in this folder
    //! \returns an iterator positioned on the first associated message in this folder
    message_iterator associated_message_begin() const
        { return boost::make_transform_iterator(get_associated_contents_table().begin(), message_transform_row(get_db())); }
    //! \brief Get the end associated message iterator
    //! \returns an iterator at the end position
    message_iterator associated_message_end() const
        { return boost::make_transform_iterator(get_associated_contents_table().end(), message_transform_row(get_db())); }

    // property access
    //! \copydoc search_folder::get_name()
    std::wstring get_name() const
        { return get_property_bag().read_prop<std::wstring>(0x3001); }
    //! \brief Get the number of sub folders in this folder
    //! \returns The number of subfolders
    size_t get_subfolder_count() const
        { return get_hierarchy_table().size(); } 
    //! \copydoc search_folder::get_unread_message_count()
    size_t get_unread_message_count() const
        { return get_property_bag().read_prop<slong>(0x3603); }
    //! \copydoc search_folder::get_message_count()
    size_t get_message_count() const
        { return get_property_bag().read_prop<slong>(0x3602); }
    //! \brief Get the number of associated messages in this folder
    //! \returns The number of associated messages
    size_t get_associated_message_count() const
        { return get_property_bag().read_prop<slong>(0x3617); }

    // lower layer access

    //! \brief Get the hierarchy table of this folder
    //! \returns The hierarchy table
    table& get_hierarchy_table();
    //! \copydoc search_folder::get_contents_table()
    table& get_contents_table();
    //! \brief Get the associated contents table of this folder
    //! \returns The associated contents table
    table& get_associated_contents_table();
    //! \copydoc folder::get_hierarchy_table()
    const table& get_hierarchy_table() const;
    //! \copydoc search_folder::get_contents_table()
    const table& get_contents_table() const;
    //! \copydoc folder::get_associated_contents_table()
    const table& get_associated_contents_table() const;

private:
    mutable std::tr1::shared_ptr<table> m_contents_table;
    mutable std::tr1::shared_ptr<table> m_associated_contents_table;
    mutable std::tr1::shared_ptr<table> m_hierarchy_table;
};

typedef detail::item_transform_info<folder> folder_transform_info;

} // end namespace fairport

inline const fairport::table& fairport::search_folder::get_contents_table() const
{
    if(!m_contents_table)
        m_contents_table.reset(new table(get_db()->lookup_node(make_nid(nid_type_search_contents_table, get_nid_index(get_property_bag().get_node().get_id())))));

    return *m_contents_table;
}

inline fairport::table& fairport::search_folder::get_contents_table()
{
    return const_cast<table&>(const_cast<const search_folder*>(this)->get_contents_table());
}


namespace internal
{

struct folder_name_equal : public std::unary_function<bool, const fairport::folder&>
{
    folder_name_equal(const std::wstring& name) : m_name(name) { }
    bool operator()(const fairport::folder& f) const { return f.get_name() == m_name; }
    std::wstring m_name;
};

} // end namespace internal

inline fairport::folder fairport::folder::open_sub_folder(const std::wstring& name)
{
    folder_iterator iter = std::find_if(sub_folder_begin(), sub_folder_end(), internal::folder_name_equal(name));

    if(iter != sub_folder_end())
        return *iter;

    throw key_not_found<std::wstring>(name);
}

inline const fairport::table& fairport::folder::get_contents_table() const
{
    if(!m_contents_table)
        m_contents_table.reset(new table(get_db()->lookup_node(make_nid(nid_type_contents_table, get_nid_index(get_property_bag().get_node().get_id())))));

    return *m_contents_table;
}

inline fairport::table& fairport::folder::get_contents_table()
{
    return const_cast<table&>(const_cast<const folder*>(this)->get_contents_table());
}

inline const fairport::table& fairport::folder::get_hierarchy_table() const
{
    if(!m_hierarchy_table)
        m_hierarchy_table.reset(new table(get_db()->lookup_node(make_nid(nid_type_hierarchy_table, get_nid_index(get_property_bag().get_node().get_id())))));

    return *m_hierarchy_table;
}

inline fairport::table& fairport::folder::get_hierarchy_table()
{
    return const_cast<table&>(const_cast<const folder*>(this)->get_hierarchy_table());
}

inline const fairport::table& fairport::folder::get_associated_contents_table() const
{
    if(!m_associated_contents_table)
        m_associated_contents_table.reset(new table(get_db()->lookup_node(make_nid(nid_type_associated_contents_table, get_nid_index(get_property_bag().get_node().get_id())))));

    return *m_associated_contents_table;
}

inline fairport::table& fairport::folder::get_associated_contents_table()
{
    return const_cast<table&>(const_cast<const folder*>(this)->get_associated_contents_table());
}

#endif
