#pragma once
#include <cstdint>

#pragma pack(push, 1)

struct CameraData {
    double x;
    double y;
    double z;
    float yaw;
    float pitch;
};

struct InputState {
    int32_t mouse_dx;
    int32_t mouse_dy;
    uint8_t keys[512];
};

struct SharedMemory {
    volatile uint64_t update_counter;
    CameraData camera;
    InputState input;
};

#pragma pack(pop)

static_assert(sizeof(SharedMemory) < 4096, "SharedMemory exceeds buffer size of 4096 bytes");

extern SharedMemory* g_shared_memory;
