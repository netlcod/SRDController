#pragma once

#include <cstdint>

namespace can {

    static uint32_t CAN_TIS_ID = 2000;
    static uint8_t TURRET_NODE_ID = 1;
    static uint8_t MASTER_NODE_ID = 100;

    enum PayloadType {
        ptNoData = 0,
        ptError = 1,
        ptFloat = 2,
        ptLong = 3,
        ptUlong = 4,
        ptShort = 6,
        ptUshort = 7,
        ptChar = 9,
        ptUchar = 10,
        ptShort2 = 12,
        ptUchar4 = 16,
        ptUchar2 = 19,
        ptMemid = 21,
        ptChksum = 22,
        ptUchar3 = 27
    };
    enum ServiceCode {
        IDS = 0,        // identification service
        DDS = 2,        // data download service (send data to another device)
        TIS = 5,        // transmission interval service
        FPS = 6,        // flash programming service
        NIS = 11,       // node id setting service
        IDS_UAVOS = 100 // UAVOS identification service (IdsUavosType in msg
                        // code and 1-4 bytes payload)
    };

    enum ResponseCode {
        OUT_OF_RANGE = -6,  // CAN identifier or transmission rate out of range
        SECURITY_FAIL = -3, // invalid security code
        INVALID = -2,       // dds invalid response
        ABORT = -1,         // dds abort response
        OK = 0,
        XOFF = 0,
        XON = 1
    };

#pragma pack(push, 1)
    template<typename T> struct CanasFrame {
        uint32_t id = 0;
        uint8_t dlc = sizeof(T) + 4;
        uint8_t node_id = 0;
        uint8_t data_type = 0;
        uint8_t service_code = 0;
        uint8_t message_code = 0;
        T data {};
    };
#pragma pack(pop)

#pragma pack(push, 1)
    struct CanFrame {
        uint32_t id; /* 32 bit CAN_ID + EFF/RTR/ERR flags */
        uint8_t dlc; /* frame payload length in byte (0 .. CAN_MAX_DLEN) */
        uint8_t data[8] __attribute__((aligned(8)));
    };
#pragma pack(pop)

    // template<typename T, size_t TSize>
    // std::array<std::byte, TSize> serialize(const T &message)
    // {
    //     std::array<std::byte, TSize> data;
    //     auto pointer = data.data();
    //     pointer = serializeField(pointer, message.id);
    //     pointer = serializeField(pointer, message.dlc);
    //     pointer = serializeField(pointer, message.nodeId);
    //     pointer = serializeField(pointer, message.dataType);
    //     pointer = serializeField(pointer, message.serviceCode);
    //     pointer = serializeField(pointer, message.messageCode);
    //     serializePayload(pointer, message.data);
    //     return data;
    // }

    // #ifdef CAN_BIG_ENDIAN
    // template<typename Pointer, typename T>
    // Pointer serializeField(Pointer pointer, const T &field)
    // {
    //     std::byte *fieldPointer = (std::byte*)&field;
    //     for(size_t i = 0; i < sizeof(T); i++)
    //         pointer[i] = fieldPointer[i];
    //     return pointer + sizeof(T);
    // }

    // template<typename Pointer, typename T>
    // Pointer deserializeField(Pointer pointer, T &field)
    // {
    //     std::byte *fieldPointer = (std::byte*)&field;
    //     for(size_t i = 0; i < sizeof(T); i++)
    //         fieldPointer[i] = pointer[i];
    //     return pointer + sizeof(T);
    // }
    // #endif

    // #ifdef CAN_LITTLE_ENDIAN
    // template<typename Pointer, typename T>
    // Pointer serializeField(Pointer pointer, const T &field)
    // {
    //     std::byte *fieldPointer = (std::byte*)&field;
    //     for(size_t i = 0; i < sizeof(T); i++)
    //         pointer[i] = fieldPointer[sizeof(T) - i - 1];
    //     return pointer + sizeof(T);
    // }

    // template<typename Pointer, typename T>
    // Pointer deserializeField(Pointer pointer, T &field)
    // {
    //     std::byte *fieldPointer = (std::byte*)&field;
    //     for(size_t i = 0; i < sizeof(T); i++)
    //         fieldPointer[i] = pointer[sizeof(T) - i - 1];
    //     return pointer + sizeof(T);
    // }
    // #endif

} // namespace can
