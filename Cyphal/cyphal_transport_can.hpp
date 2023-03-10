/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2022 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

/**
 * @file cyphal_transport_can.h
 * @author d.ponomarev
 * @date Jul 07, 2022
 */

#ifndef CYPHAL_CYPHAL_TRANSPORT_CAN_HPP_
#define CYPHAL_CYPHAL_TRANSPORT_CAN_HPP_

#include "canard.h"

class CyphalTransportCan {
public:
    CyphalTransportCan() = default;
    bool init(uint32_t can_speed, uint8_t can_driver_idx);
    bool receive(CanardFrame* can_frame);
    bool transmit(const CanardTxQueueItem* transfer);
private:
    uint8_t _out_payload[256];
    uint8_t _can_driver_idx;
};

#endif  // CYPHAL_CYPHAL_TRANSPORT_CAN_HPP_
