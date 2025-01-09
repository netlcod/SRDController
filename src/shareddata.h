#pragma once

#include <cstdint>

namespace SharedData {

    static int pitchRange = 360;
    static int yawRange = 360;

    enum class Device {
        Rotator,
    };

    enum class Direction { Left, Right, Up, Down };

} // namespace SharedData
