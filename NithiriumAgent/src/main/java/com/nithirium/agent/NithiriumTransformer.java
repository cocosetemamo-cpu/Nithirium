package com.nithirium.agent;

import org.objectweb.asm.ClassReader;
import org.objectweb.asm.ClassVisitor;
import org.objectweb.asm.ClassWriter;
import org.objectweb.asm.MethodVisitor;
import org.objectweb.asm.Opcodes;
import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.IllegalClassFormatException;
import java.security.ProtectionDomain;

public final class NithiriumTransformer implements ClassFileTransformer {

    private static final String LEVEL_RENDERER_CLASS = "net/minecraft/client/renderer/LevelRenderer";
    private static final String CHUNK_RENDER_DISPATCHER_RENDER_CHUNK_CLASS = "net/minecraft/client/renderer/chunk/ChunkRenderDispatcher$RenderChunk";

    @Override
    public byte[] transform(ClassLoader loader, String className, Class<?> classBeingRedefined,
                            ProtectionDomain protectionDomain, byte[] classfileBuffer) throws IllegalClassFormatException {

        if (LEVEL_RENDERER_CLASS.equals(className)) {
            System.out.println("[NithiriumTransformer] Intercepted LevelRenderer: " + className);
            return transformLevelRenderer(classfileBuffer);
        }
        if (CHUNK_RENDER_DISPATCHER_RENDER_CHUNK_CLASS.equals(className)) {
            System.out.println("[NithiriumTransformer] Intercepted ChunkRenderDispatcher$RenderChunk: " + className);
        }
        return null;
    }

    private byte[] transformLevelRenderer(byte[] originalBytecode) {
        ClassReader classReader = new ClassReader(originalBytecode);
        ClassWriter classWriter = new ClassWriter(classReader, ClassWriter.COMPUTE_FRAMES);
        LevelRendererClassVisitor classVisitor = new LevelRendererClassVisitor(Opcodes.ASM9, classWriter);
        classReader.accept(classVisitor, ClassReader.EXPAND_FRAMES);
        System.out.println("[NithiriumTransformer] Transformation of LevelRenderer complete.");
        return classWriter.toByteArray();
    }

    private static class LevelRendererClassVisitor extends ClassVisitor {
        public LevelRendererClassVisitor(int api, ClassVisitor classVisitor) {
            super(api, classVisitor);
        }

        @Override
        public MethodVisitor visitMethod(int access, String name, String descriptor, String signature, String[] exceptions) {
            MethodVisitor mv = super.visitMethod(access, name, descriptor, signature, exceptions);
            final String targetMethodName = "renderLevel";
            if (name.equals(targetMethodName)) {
                System.out.println("[NithiriumTransformer] Found renderLevel. Injecting hook...");
                return new RenderMethodVisitor(api, mv);
            }
            return mv;
        }
    }

    private static class RenderMethodVisitor extends MethodVisitor {
        public RenderMethodVisitor(int api, MethodVisitor methodVisitor) {
            super(api, methodVisitor);
        }

        @Override
        public void visitCode() {
            super.visitCode();
            super.visitMethodInsn(
                Opcodes.INVOKESTATIC,
                "com/nithirium/agent/NithiriumBridge",
                "onRenderLevel",
                "^(^)V",
                false
            );
        }
    }
}
