package com.nithirium.agent;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public final class NithiriumBridge {

    private static final int SHARED_MEMORY_SIZE = 4096;

    static {
        try {
            System.loadLibrary("NithiriumCore");
            System.out.println("[NithiriumBridge] Native library loaded successfully.");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("[NithiriumBridge] FATAL: Native code library failed to load.\n" + e);
            System.exit(1);
        }
    }

    public static void init() {
        System.out.println("[NithiriumBridge] Initializing bridge to native core.");
        ByteBuffer sharedMemory = ByteBuffer.allocateDirect(SHARED_MEMORY_SIZE);
        sharedMemory.order(ByteOrder.nativeOrder());
        System.out.println("[NithiriumBridge] Direct ByteBuffer allocated. Size: " + SHARED_MEMORY_SIZE + " bytes.");
        nativeInitialize(sharedMemory);
        System.out.println("[NithiriumBridge] Native core has shut down.");
    }

    public static void onRenderLevel() {
        System.out.println("[NithiriumBridge] renderLevel intercepted!");
    }

    private static native void nativeInitialize(ByteBuffer sharedMemory);
}
