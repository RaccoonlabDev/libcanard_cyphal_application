/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_publishers.cpp
 * @author d.ponomarev
 * @date Aug 12, 2022
 */

#include "cyphal_publishers.hpp"
#include "cyphal.hpp"
#include "main.h"
#include "storage.h"

std::array<CyphalPublisher*, 15> CyphalPublisher::publishers;
uint8_t CyphalPublisher::publishers_amount{0};


void CyphalPublisher::setPortId(CanardPortID port_id) {
    transfer_metadata.port_id = port_id;
}

CanardPortID CyphalPublisher::getPortId() const {
    return transfer_metadata.port_id;
}


bool CyphalPublisher::isEnabled() const {
    constexpr uint16_t MAX_PORT_ID = 8191;
    uint16_t port_id = transfer_metadata.port_id;
    return (port_id == 0 || port_id > MAX_PORT_ID) ? false : true;
}

int32_t CyphalPublisher::push(size_t payload_size, const uint8_t* payload) {
    if (!isEnabled()) {
        return 0;
    }

    return driver->push(&transfer_metadata, payload_size, payload);
}

void HeartbeatPublisher::publish(const uavcan_node_Heartbeat_1_0& msg) {
    static uint8_t buffer[uavcan_node_Heartbeat_1_0_EXTENT_BYTES_];
    size_t buffer_size = uavcan_node_Heartbeat_1_0_EXTENT_BYTES_;
    int32_t result = uavcan_node_Heartbeat_1_0_serialize_(&msg, buffer, &buffer_size);
    if (NUNAVUT_SUCCESS == result) {
        push(buffer_size, buffer);
    }
}

/**
 * @brief Optimized version of the default nunavut generated serialization.
 * Nanuvut requires 10658 bytes: 2192 for msg and 8466 for serialization buffer.
 * This approach requires only 148 + N*2 bytes where N is number of publishers + subscribers.
 */
size_t PortListPublisher::uavcan_node_port_List_1_0_create() {
    // Clear
    memset(_port_list_buffer, 0x00, PORT_LIST_BUFFER_SIZE);

    // 1. Fill publishers
    uint8_t& enabled_pub_amount = _port_list_buffer[5];
    auto sparse_list = reinterpret_cast<uint16_t*>(&_port_list_buffer[6]);
    _port_list_buffer[4] = 1;

    enabled_pub_amount = 0;
    for (uint_fast8_t pub_idx = 0; pub_idx < CyphalPublisher::publishers_amount; pub_idx++) {
        if (CyphalPublisher::publishers[pub_idx]->isEnabled()) {
            auto port_id = CyphalPublisher::publishers[pub_idx]->getPortId();
            sparse_list[pub_idx] = port_id;
            enabled_pub_amount++;
        }
    }
    _port_list_buffer[0] = 2 + 2 * enabled_pub_amount;

    // 2. Fill subscribers
    size_t subs_offset = 6 + 2 * enabled_pub_amount;
    uint8_t& enabled_sub_amount = _port_list_buffer[5 + subs_offset];
    sparse_list = (uint16_t*)&_port_list_buffer[6 + subs_offset];
    _port_list_buffer[4 + subs_offset] = 1;

    enabled_sub_amount = 0;
    for (uint_fast8_t sub_idx = 0; sub_idx < driver->_sub_num; sub_idx++) {
        if (driver->_sub_info[sub_idx]->isEnabled()) {
            auto port_id = driver->_sub_info[sub_idx]->port_id;
            sparse_list[sub_idx] = port_id;
            enabled_sub_amount++;
        }
    }
    _port_list_buffer[0 + subs_offset] = 2 + 2 * enabled_sub_amount;

    // 3. Fix services
    _port_list_buffer[12 + (enabled_pub_amount + enabled_sub_amount) * 2] = 64;

    return 148 + (enabled_pub_amount + enabled_sub_amount) * 2;
}

void PortListPublisher::publish() {
    auto crnt_time_ms = HAL_GetTick();
    if (crnt_time_ms < next_pub_time_ms) {
        return;
    }
    next_pub_time_ms = crnt_time_ms + 5000;

    size_t size = uavcan_node_port_List_1_0_create();
    push(size, _port_list_buffer);
}
