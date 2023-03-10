/// This software is distributed under the terms of the MIT License.
/// Copyright (c) 2023 Dmitry Ponomarev.
/// Author: Dmitry Ponomarev <ponomarevda96@gmail.com>

#ifndef UDRAL_GNSS_HPP_
#define UDRAL_GNSS_HPP_

#include "cyphal.hpp"
#include "reg/udral/physics/kinematics/geodetic/PointStateVarTs_0_1.h"

struct GpsPointPublisher: public CyphalPublisher {
    GpsPointPublisher(Cyphal* driver_, uint16_t port_id) : CyphalPublisher(driver_, port_id) {}
    void publish(const reg_udral_physics_kinematics_geodetic_PointStateVarTs_0_1& msg);
};

struct Int16Publisher: public CyphalPublisher {
    Int16Publisher(Cyphal* driver_, uint16_t port_id) : CyphalPublisher(driver_, port_id) {}
    void publish(uint16_t value);
};

#endif  // UDRAL_GNSS_HPP_
