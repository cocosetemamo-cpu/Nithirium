Nithirium

Java FPS. Bedrock performance. For the PC you actually have.

Nithirium is a native C++ rendering engine replacement for Minecraft Java Edition 1.18.2. Instead of patching Java's renderer like every other optimization mod, Nithirium completely bypasses it — intercepting Minecraft's rendering pipeline via a Java agent and redirecting it to a custom C++ engine built on BGFX.
The result: Bedrock-level FPS on hardware that Java was never designed to run well on.

Why Nithirium?
Every optimization mod out there — Sodium, Optifine, Lithium — works within the limits of the JVM. They make Java faster. Nithirium gets rid of Java for rendering entirely.
The JVM's Garbage Collector causes lag spikes. Manual C++ memory management doesn't. That's the core idea.
Target audience: Players with low-end hardware (Celeron, Pentium, old i3, 2-4GB RAM, integrated graphics) who want to play on Java servers without the lag.

How it works
Minecraft Java 1.18.2 (vanilla)
        │
        │  -javaagent: NithiriumAgent.jar
        ▼
NithiriumAgent (Java)
        │
        ├── ASM bytecode transformer → intercepts LevelRenderer + ChunkRenderer
        ├── JNI → one-time init only
        └── Direct ByteBuffer (shared memory, zero GC pressure)
        │
        ▼
NithiriumCore (C++)
        ├── BGFX (auto-selects OpenGL / Vulkan / DX11 based on hardware)
        ├── Chunk mesher with SIMD
        ├── Frustum culling
        ├── Draw call batching
        └── Zero garbage collection interference
Java handles game logic. C++ handles rendering. Shared memory connects them with zero overhead.

Compatibility
ThingCompatible?Vanilla servers✅ YesCracked / offline servers✅ YesClient-side mods❌ No (intentional)Forge / Fabric mods❌ No (intentional)Minecraft version1.18.2 only (for now)
Server-side mods work fine — the server doesn't care how you render the world.

Stack

Java 17 — agent, ASM hooks, JNI bridge
ASM 9.x — bytecode interception
C++17 — rendering engine
BGFX — rendering abstraction (OpenGL 2.1 / 3.3 / Vulkan / DX11, auto-selected)
Shared memory — zero-copy communication between Java and C++
GitHub Actions — builds everything in the cloud


Roadmap
VersionStatusDescriptionv0.1🔨 In progressBase structure, ASM hooks, BGFX windowv0.2📋 PlannedExtract real chunk data from Minecraftv0.3📋 PlannedRender first chunk in BGFXv0.4📋 PlannedReplicate vanilla 1.18.2 shadersv0.5📋 PlannedFull world renderingv1.0📋 PlannedNLauncher — install and play in one click

Building
Nithirium is built entirely via GitHub Actions. You don't need Visual Studio or MinGW locally.
Just push to the repo and download the artifacts from the Actions tab:

NithiriumAgent.jar
NithiriumCore.dll


Installing (manual, pre-NLauncher)

Download NithiriumAgent.jar and NithiriumCore.dll from the latest GitHub Actions run
Place both files in a folder, e.g. C:\Nithirium\
In your Minecraft launcher (TLauncher Legacy, SKLauncher, etc), edit your 1.18.2 installation
Add to JVM arguments:

-javaagent:"C:\Nithirium\NithiriumAgent.jar" -Djava.library.path="C:\Nithirium"

Launch and enjoy


Status
Early development. Nothing renders yet. The hook infrastructure is in place — actual rendering replacement is next.

Built for the players who can't afford Bedrock but deserve good FPS.
