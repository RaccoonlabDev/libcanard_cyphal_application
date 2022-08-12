/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal.h
 * @author d.ponomarev
 * @date Dec 28, 2021
 */

#ifndef LIBCYPHAL_CYPHAL_HPP_
#define LIBCYPHAL_CYPHAL_HPP_

#include "cyphal_transport_can.hpp"
#include "canard.h"
#include "o1heap.h"
#include "uavcan/node/GetInfo_1_0.h"

#define HEAP_SIZE           2048

class Cyphal;

class CyphalSubscriber {
public:
    CyphalSubscriber(Cyphal* driver_, CanardPortID port_id_) : driver(driver_), port_id(port_id_) {}
    virtual void callback(const CanardRxTransfer& transfer) = 0;
    CanardRxSubscription subscription;
    Cyphal* driver;
    CanardPortID port_id;
};

struct NodeGetInfoSubscriber: public CyphalSubscriber {
    NodeGetInfoSubscriber(Cyphal* driver_, CanardPortID port_id_) : CyphalSubscriber(driver_, port_id_) {};
    void callback(const CanardRxTransfer& transfer) override;
};


class Cyphal {
public:
    Cyphal() {};
    Cyphal(uint8_t id) : node_id(id) {};
    int init();
    void process();
    int32_t push(const CanardTransferMetadata *metadata, size_t payload_size, const void *payload);
    int8_t subscribe(CyphalSubscriber* sub_info, size_t size, CanardTransferKind kind);
private:
    void spinReceivedFrame(const CanardMicrosecond rx_timestamp_usec,
                           const CanardFrame* const received_frame);
    void spinTransmit();
    void processReceivedTransfer(const uint8_t redundant_interface_index,
                                 const CanardRxTransfer& transfer);

    ///< application
    int8_t subscribeApplication();
    void publishHeartbeat();
    // void publishNodeGetInfoResponse(const CanardRxTransfer& transfer);

    CyphalTransportCan transport;
    CanardInstance canard_instance;
    CanardTxQueue queue;
    uint8_t base[HEAP_SIZE] __attribute__ ((aligned (O1HEAP_ALIGNMENT)));
    uint8_t my_message_transfer_id;
    uavcan_node_GetInfo_Response_1_0 node_status;
    uint32_t error_counter = 0;
    uint8_t node_id{42};

    static constexpr size_t MAX_SUB_NUM = 10;
    CyphalSubscriber* _sub_info[MAX_SUB_NUM];
    size_t _sub_num{0};
};

#endif  // LIBCYPHAL_CYPHAL_HPP_