package com.nithirium.agent;

import java.lang.instrument.Instrumentation;

public final class NithiriumAgent {

    public static void premain(String agentArgs, Instrumentation inst) {
        System.out.println("[NithiriumAgent] Nithirium Agent is initializing...");

        Thread nithiriumCoreThread = new Thread(() -> {
            try {
                NithiriumBridge.init();
            } catch (Throwable t) {
                System.err.println("[NithiriumAgent] Critical error in native thread!");
                t.printStackTrace();
            }
        }, "NithiriumCore-Thread");

        nithiriumCoreThread.setUncaughtExceptionHandler((thread, throwable) -> {
            System.err.println("[NithiriumAgent] Uncaught exception in thread: " + thread.getName());
            throwable.printStackTrace();
        });

        nithiriumCoreThread.start();
        System.out.println("[NithiriumAgent] Native core thread started.");

        inst.addTransformer(new NithiriumTransformer(), true);
        System.out.println("[NithiriumAgent] Class file transformer has been attached.");
    }
}
