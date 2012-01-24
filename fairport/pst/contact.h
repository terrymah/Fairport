//! \file
//! \brief Contact related classes
//! \author Terry Mahaffey
//!
//! Defines the contact abstraction.
//! \ingroup pst

#ifndef FAIRPORT_PST_CONTACT_H
#define FAIRPORT_PST_CONTACT_H

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

//! \defgroup pst_contactrelated Contact Objects
//! \ingroup pst

class contact
{
public:
    //! \brief Construct a contact object
    //! \param[in] n The node to construct this contact object on top of
    explicit contact(const node& n)
        : m_bag(n) { }

	// property access
    //! \brief Get the full name of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.1.8
    //! \returns The full name of this contact
    std::wstring get_full_name() const
        { return m_bag.read_prop<std::wstring>(0x3001); }

    //! \brief Get the company of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.6.2
    //! \returns The company of this contact
    std::wstring get_company() const
        { return m_bag.read_prop<std::wstring>(0x3a16); }
    //! \brief Check to see if this contact has a company associated with it
    //! \returns true if this contact has a company associated with it
    bool has_company() const
        { return m_bag.prop_exists(0x3a16); }

    //! \brief Get the job title of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.6.1
    //! \returns The job title of this contact
    std::wstring get_job_title() const
        { return m_bag.read_prop<std::wstring>(0x3a17); }
     //! \brief Check to see if this contact has a job title associated with it
    //! \returns true if this contact has a job title associated with it
    bool has_job_title() const
        { return m_bag.prop_exists(0x3a17); }

    //! \brief Get the email address of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.2.3
    //! \returns The email address of this contact
    std::wstring get_email_address() const
        { return m_bag.read_prop<std::wstring>(get_name_id_map().lookup(ps_contact, 0x8083)); }
    //! \brief Check to see if this contact has an email address associated with it
    //! \returns true if this contact has an email address associated with it
    bool has_email_address() const
        { return m_bag.prop_exists(get_name_id_map().lookup(ps_contact, 0x8083)); }

    //! \brief Get the second email address of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.2.3
    //! \returns The second email address of this contact
    std::wstring get_email_address2() const
        { return m_bag.read_prop<std::wstring>(get_name_id_map().lookup(ps_contact, 0x8093)); }
    //! \brief Check to see if this contact has a second email address associated with it
    //! \returns true if this contact has a second email address associated with it
    bool has_email_address2() const
        { return m_bag.prop_exists(get_name_id_map().lookup(ps_contact, 0x8093)); }

    //! \brief Get the third email address of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.2.3
    //! \returns The third email address of this contact
    std::wstring get_email_address3() const
        { return m_bag.read_prop<std::wstring>(get_name_id_map().lookup(ps_contact, 0x80a3)); }
    //! \brief Check to see if this contact has a third email address associated with it
    //! \returns true if this contact has a third email address associated with it
    bool has_email_address3() const
        { return m_bag.prop_exists(get_name_id_map().lookup(ps_contact, 0x80a3)); }

    //! \brief Get the business phone number of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.4.3
    //! \returns The business phone number of this contact
    std::wstring get_business_phone() const
        { return m_bag.read_prop<std::wstring>(0x3a08); }
    //! \brief Check to see if this contact has a business phone number associated with it
    //! \returns true if this contact has a business phone number associated with it 
    bool has_business_phone() const
        { return m_bag.prop_exists(0x3a08); }

    //! \brief Get the home phone number of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.4.4
    //! \returns The home phone number of this contact
    std::wstring get_home_phone() const
        { return m_bag.read_prop<std::wstring>(0x3a09); }
    //! \brief Check to see if this contact has a home phone number associated with it
    //! \returns true if this contact has a home phone number associated with it
    bool has_home_phone() const
        { return m_bag.prop_exists(0x3a09); }

    //! \brief Get the primary phone number of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.4.5
    //! \returns The primary phone number of this contact
    std::wstring get_primary_phone() const
        { return m_bag.read_prop<std::wstring>(0x3a1a); }
    //! \brief Check to see if this contact has a primary phone number associated with it
    //! \returns true if this contact has a primary phone number associated with it
    bool has_primary_phone() const
        { return m_bag.prop_exists(0x3a1a); }

    //! \brief Get the mobile phone number of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.4.7
    //! \returns The mobile phone number of this contact 
    std::wstring get_mobile_phone() const
        { return m_bag.read_prop<std::wstring>(0x3a1c); }
    //! \brief Check to see if this contact has a mobile phone number associated with it
    //! \returns true if this contact has a mobile phone number associated with it
    bool has_mobile_phone() const
        { return m_bag.prop_exists(0x3a1c); }

    //! \brief Get the home address of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.3.8
    //! \returns The home address of this contact
    std::wstring get_home_address() const
        { return m_bag.read_prop<std::wstring>(get_name_id_map().lookup(ps_contact, 0x801a)); }
    //! \brief Check to see if this contact has a home address associated with it
    //! \returns true if this contact has a home address associated with it
    bool has_home_address() const
        { return m_bag.prop_exists(get_name_id_map().lookup(ps_contact, 0x801a)); }

    //! \brief Get the business address of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.3.8
    //! \returns The business address of this contact
    std::wstring get_business_address() const
        { return m_bag.read_prop<std::wstring>(get_name_id_map().lookup(ps_contact, 0x801b)); }
    //! \brief Check to see if this contact has a business address associated with it
    //! \returns true if this contact has a business address associated with it
    bool has_business_address() const
        { return m_bag.prop_exists(get_name_id_map().lookup(ps_contact, 0x801b)); }

    //! \brief Get the photo of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.8.3
    //! \returns The photo of this contact, as an attachment
    attachment& get_contact_photo_attachment()
        { return const_cast<attachment&>(const_cast<const contact*>(this)->get_contact_photo_attachment()); }
    //! \copydoc contact::get_contact_photo_attachment()
    const attachment& get_contact_photo_attachment() const;
    //! \brief Get the photo of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.8.3
    //! \returns The raw bytes of the photo of this contact
    std::vector<byte> get_contact_photo_bytes() const
        { return get_contact_photo_attachment().get_bytes(); }
    //! \brief Get the photo of this contact
    //! \sa [MS-OXOCNTC] 2.2.1.8.3
    //! \returns A stream of the photo
    hnid_stream_device open_contact_photo_stream()
        { return get_contact_photo_attachment().open_byte_stream(); }
    //! \brief Check to see if this contact has a contact photo associated with it
    //! \sa [MS-OXOCNTC] 2.2.1.8.1
    //! \returns true if this contact has a contact photo associated with it
    bool has_contact_photo() const
        { return m_bag.read_prop<bool>(get_name_id_map().lookup(ps_contact, 0x8015)); }

    //! \brief Get the node_id of this contact
    //! \returns The node_id of the contact
    node_id get_id() const
        { return m_bag.get_node().get_id(); }

    //! \brief Get the MAPI entry ID of this contact
    //! \returns The MAPI entry ID of this contact
    std::vector<byte> get_entry_id() const
        { return calculate_entry_id(m_bag.get_node().get_db(), get_id()); }

private:
    const name_id_map& get_name_id_map() const;

    contact& operator=(const contact&); // = delete

    property_bag m_bag;
    mutable std::tr1::shared_ptr<name_id_map> m_map;     //!< The named property map of this store
    mutable std::tr1::shared_ptr<attachment> m_photo;    //!< The contact photo attachment
};

typedef detail::item_transform_row<contact> contact_transform_row;

} // end namespace fairport

inline const fairport::name_id_map& fairport::contact::get_name_id_map() const
{
    if(!m_map)
        m_map.reset(new name_id_map(m_bag.get_node().get_db()));

    return *m_map;
}

inline const fairport::attachment& fairport::contact::get_contact_photo_attachment() const
{
    if(!m_photo)
    {
        table t(m_bag.get_node().lookup(nid_attachment_table));

        for(const_table_row_iter i = t.begin(); i != t.end(); ++i)
        {
            attachment a(property_bag(m_bag.get_node().lookup(i->get_row_id())));
            if(a.get_property_bag().read_prop<bool>(0x7fff))
            {
                m_photo.reset(new attachment(a));
                break;
            }
        }

        if(!m_photo)
            throw key_not_found<prop_id>(0x7fff);
    }

    return *m_photo;
}

#endif