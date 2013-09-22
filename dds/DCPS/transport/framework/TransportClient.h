/*
 * $Id$
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#ifndef OPENDDS_DCPS_TRANSPORT_CLIENT_H
#define OPENDDS_DCPS_TRANSPORT_CLIENT_H

#include "dds/DCPS/dcps_export.h"
#include "TransportConfig_rch.h"
#include "TransportImpl.h"
#include "DataLinkSet.h"

#include "dds/DCPS/AssociationData.h"

#include "ace/Time_Value.h"
#include "ace/Event_Handler.h"

#include <vector>

// Forward definition of a test-friendly class in the global name space
class DDS_TEST;

namespace OpenDDS {
namespace DCPS {

class EntityImpl;
class TransportInst;
class AssocationInfo;
class ReaderIdSeq;
class WriterIdSeq;
class DataSampleList;

/**
 * @brief Mix-in class for DDS entities which directly use the transport layer.
 *
 * DataReaderImpl and DataWriterImpl are TransportClients.  The TransportClient
 * class manages the TransportImpl objects that represent the available
 * communication mechanisms and the DataLink objects that represent the
 * currently active communication channels to peers.
 */
class OpenDDS_Dcps_Export TransportClient {
public:
  // Used by TransportImpl to complete associate() processing:
  void use_datalink(const RepoId& remote_id, const DataLink_rch& link);

  // values for flags parameter of transport_assoc_done():
  enum { ASSOC_OK = 1, ASSOC_ACTIVE = 2 };

protected:
  TransportClient();
  virtual ~TransportClient();


  // Local setup:

  void enable_transport(bool reliable, bool durable);
  void enable_transport_using_config(bool reliable, bool durable,
                                     const TransportConfig_rch& tc);

  bool swap_bytes() const { return swap_bytes_; }
  bool cdr_encapsulation() const { return cdr_encapsulation_; }
  const TransportLocatorSeq& connection_info() const { return conn_info_; }


  // Managing associations to remote peers:

  bool associate(const AssociationData& peer, bool active);
  void disassociate(const RepoId& peerId);


  // Data transfer:

  bool send_response(const RepoId& peer,
                     const DataSampleHeader& header,
                     ACE_Message_Block* payload); // [DR]
  void send(const DataSampleList& samples);
  SendControlStatus send_control(const DataSampleHeader& header,
                                 ACE_Message_Block* msg,
                                 void* extra = 0);
  bool remove_sample(const DataSampleListElement* sample);
  bool remove_all_msgs();

private:

  // Implemented by derived classes (DataReaderImpl/DataWriterImpl)
  virtual bool check_transport_qos(const TransportInst& inst) = 0;
  virtual const RepoId& get_repo_id() const = 0;
  virtual DDS::DomainId_t domain_id() const = 0;
  virtual Priority get_priority_value(const AssociationData& data) const = 0;
  virtual void transport_assoc_done(int /*flags*/, const RepoId& /*remote*/) {}

  // transport_detached() is called from TransportImpl when it shuts down
  friend class TransportImpl;
  void transport_detached(TransportImpl* which);

  // helpers
  typedef ACE_Guard<ACE_Thread_Mutex> Guard;
  void use_datalink_i(const RepoId& remote_id,
                      const DataLink_rch& link,
                      Guard& guard);
  void add_link(const DataLink_rch& link, const RepoId& peer);
  TransportSendListener* get_send_listener();
  TransportReceiveListener* get_receive_listener();

  // A class, normally provided by an unit test, who needs access to a client's
  // privates.
  friend class ::DDS_TEST;

  typedef std::map<RepoId, DataLink_rch, GUID_tKeyLessThan> DataLinkIndex;


  struct PendingAssoc : ACE_Event_Handler {
    bool active_, removed_;
    std::vector<TransportImpl_rch> impls_;
    CORBA::ULong blob_index_;
    AssociationData data_;
    TransportImpl::ConnectionAttribs attribs_;

    PendingAssoc()
      : active_(false), removed_(false), blob_index_(0)
    {}

    bool initiate_connect(TransportClient* tc, Guard& guard);
    int handle_timeout(const ACE_Time_Value& time, const void* arg);
  };

  typedef std::map<RepoId, PendingAssoc, GUID_tKeyLessThan> PendingMap;


  // Associated Impls and DataLinks:

  std::vector<TransportImpl_rch> impls_;
  PendingMap pending_;
  DataLinkSet links_, send_links_;
  DataLinkIndex data_link_index_;


  // Configuration details:

  bool swap_bytes_, cdr_encapsulation_, reliable_, durable_;
  ACE_Time_Value passive_connect_duration_;


  TransportLocatorSeq conn_info_;
  ACE_Thread_Mutex lock_;
  RepoId repo_id_;
};

}
}

#endif
