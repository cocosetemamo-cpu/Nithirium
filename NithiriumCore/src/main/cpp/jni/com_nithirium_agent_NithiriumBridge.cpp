#include "shared_memory.h"
#include <jni.h>
#include <iostream>
#include <cstring>

static_assert(sizeof(SharedMemory) < 4096, "sizeof^(SharedMemory^) exceeds Java buffer size of 4096 bytes.");

void run_render_loop();

extern "C" {

JNIEXPORT void JNICALL Java_com_nithirium_agent_NithiriumBridge_nativeInitialize(
    JNIEnv* env,
    jclass,
    jobject sharedMemoryBuffer) {

    std::cout << "[NithiriumCore-JNI] nativeInitialize called." << std::endl;

    void* buffer_ptr = env->GetDirectBufferAddress(sharedMemoryBuffer);
    if (!buffer_ptr) {
        std::cerr << "[NithiriumCore-JNI] FATAL: Could not get buffer address." << std::endl;
        jclass ex = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(ex, "Failed to get direct buffer address");
        return;
    }

    jlong capacity = env->GetDirectBufferCapacity(sharedMemoryBuffer);
    std::cout << "[NithiriumCore-JNI] Buffer address: " << buffer_ptr << std::endl;
    std::cout << "[NithiriumCore-JNI] Buffer capacity: " << capacity << " bytes." << std::endl;
    std::cout << "[NithiriumCore-JNI] sizeof^(SharedMemory^): " << sizeof(SharedMemory) << " bytes." << std::endl;

    g_shared_memory = static_cast<SharedMemory*>(buffer_ptr);
    memset(g_shared_memory, 0, static_cast<size_t>(capacity));
    std::cout << "[NithiriumCore-JNI] Shared memory pointer set." << std::endl;

    try {
        run_render_loop();
    } catch (const std::exception& e) {
        std::cerr << "[NithiriumCore] Exception: " << e.what() << std::endl;
        jclass ex = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(ex, e.what());
    } catch (...) {
        std::cerr << "[NithiriumCore] Unknown exception." << std::endl;
        jclass ex = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(ex, "Unknown C++ exception in render loop.");
    }
}

} // extern "C"
