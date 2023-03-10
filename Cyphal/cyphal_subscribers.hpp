/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal.h
 * @author d.ponomarev
 * @date Dec 28, 2021
 */

#ifndef CYPHAL_CYPHAL_SUBSCRIBERS_HPP_
#define CYPHAL_CYPHAL_SUBSCRIBERS_HPP_

#include "canard.h"
#include "uavcan/node/GetInfo_1_0.h"
#include "uavcan/node/ExecuteCommand_1_0.h"

class Cyphal;

class CyphalSubscriber {
public:
    CyphalSubscriber(Cyphal* driver_, CanardPortID port_id_) : driver(driver_), port_id(port_id_) {}
    virtual void callback(const CanardRxTransfer& transfer) = 0;
    bool isEnabled() const;
    CanardRxSubscription subscription;
    Cyphal* driver;
    CanardPortID port_id;
};

struct NodeGetInfoSubscriber: public CyphalSubscriber {
    explicit NodeGetInfoSubscriber(Cyphal* driver_);
    void callback(const CanardRxTransfer& transfer) override;
    static void setHardwareVersion(uint8_t major, uint8_t minor);
private:
    void updateNodeName();
    uavcan_node_GetInfo_Response_1_0 get_info_response = {};
    uint8_t _transfer_id = 0;
    static uavcan_node_Version_1_0 hw_version;
};

struct ExecuteCommandSubscriber: public CyphalSubscriber {
    explicit ExecuteCommandSubscriber(Cyphal* driver_) :
        CyphalSubscriber(driver_, uavcan_node_ExecuteCommand_1_0_FIXED_PORT_ID_) {};
    void callback(const CanardRxTransfer& transfer) override;
private:
    uint8_t _transfer_id = 0;
};

#endif  // CYPHAL_CYPHAL_SUBSCRIBERS_HPP_
