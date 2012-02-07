//! \file
//! \brief Task related classes
//! \author Terry Mahaffey
//!
//! Defines the task abstraction.
//! \ingroup pst

#ifndef FAIRPORT_PST_TASK_H
#define FAIRPORT_PST_TASK_H

#include <string>
#include <memory>

#include "fairport/ndb/database_iface.h"
#include "fairport/ndb/node.h"

#include "fairport/ltp/propbag.h"
#include "fairport/ltp/table.h"
#include "fairport/ltp/nameid.h"

#include "fairport/pst/pstutil.h"
#include "fairport/pst/message.h"

namespace fairport 
{

//! \defgroup pst_taskrelated Contact Objects
//! \ingroup pst

//! \brief Encapsulates a task
//! 
//! A contact object is a higher level abstraction of a task, offering propreties
//! specific to Outlook tasks.
//! \sa [MS-OXOCNTC]
//! \ingroup pst_taskrelated
class task : public detail::itembase
{
public:
    //! \brief Construct a task object
    //! \param[in] n The node to construct this task object on top of
    explicit task(const node& n)
        : detail::itembase(n) { }

	// property access
    //! \brief Get the name of this task
    //! \sa [MS-OXCMSG] 2.2.1.46
    //! \returns The message subject
    std::wstring get_name() const;
    //! \brief Check to see if a subject is set on this message
    //! \returns true if a subject is set on this message
    bool has_subject() const
        { return get_property_bag().prop_exists(0x37); }

    //! \brief Query the completion status of this taks
    //! \sa [MS-OXOTASK] 2.2.2.2.20
    //! \returns true if this task is complete
    bool is_complete() const
        { return get_property_bag().read_prop<bool>(get_name_id_map().lookup(ps_task, 0x811c)); }

    //! \brief Get the start date of this task
    //! \sa [MS-OXOTASK] 2.2.2.2.4
    //! \returns The start date of this task
    boost::posix_time::ptime get_start_date() const
        { return get_property_bag().read_prop<boost::posix_time::ptime>(get_name_id_map().lookup(ps_task, 0x8104)); }
    //! \brief Check to see if this task has a start date associated with it
    //! \returns true if this task has a start date associated with it
    bool has_start_date() const
        { return get_property_bag().prop_exists(get_name_id_map().lookup(ps_task, 0x8104)); }
    //! \brief Get the due date of this task
    //! \sa [MS-OXOTASK] 2.2.2.2.5
    //! \returns The due date of this task
    boost::posix_time::ptime get_due_date() const
        { return get_property_bag().read_prop<boost::posix_time::ptime>(get_name_id_map().lookup(ps_task, 0x8105)); }
    //! \brief Check to see if this task has a due date associated with it
    //! \returns true if this task has a due date associated with it
    bool has_due_date() const
        { return get_property_bag().prop_exists(get_name_id_map().lookup(ps_task, 0x8105)); }
private:
    const name_id_map& get_name_id_map() const;

    mutable std::tr1::shared_ptr<name_id_map> m_map;     //!< The named property map of this store
};

typedef detail::item_transform_row<task> task_transform_row;

} // end namespace fairport

inline const fairport::name_id_map& fairport::task::get_name_id_map() const
{
    if(!m_map)
        m_map.reset(new name_id_map(get_property_bag().get_node().get_db()));

    return *m_map;
}

inline std::wstring fairport::task::get_name() const
{
    std::wstring buffer = get_property_bag().read_prop<std::wstring>(0x37);

    if(buffer.size() && buffer[0] == message_subject_prefix_lead_byte)
    {
        // Skip the second chracter as well
        return buffer.substr(2);
    }
    else
    {
        return buffer;
    }
}

#endif