<div align="center">

# ⚡ NITHIRIUM

### *Java FPS. Bedrock performance. For the PC you actually have.*

[![Build](https://img.shields.io/github/actions/workflow/status/cocosetemamo-cpu/Nithirium/build.yml?style=for-the-badge&logo=github&label=BUILD&color=black)](https://github.com/cocosetemamo-cpu/Nithirium/actions)
[![Version](https://img.shields.io/badge/VERSION-0.1--dev-red?style=for-the-badge)](https://github.com/cocosetemamo-cpu/Nithirium)
[![Minecraft](https://img.shields.io/badge/MINECRAFT-1.18.2-green?style=for-the-badge&logo=minecraft)](https://minecraft.net)
[![License](https://img.shields.io/badge/LICENSE-MIT-blue?style=for-the-badge)](LICENSE)

<br/>

> **Nithirium replaces Minecraft Java's rendering engine with a native C++ engine.**
> Not a mod. Not a patch. A full renderer replacement.

</div>

---

## 🤔 Why does this exist?

Every optimization mod out there — Sodium, Optifine, Lithium — works **within** the limits of the JVM. They make Java faster. **Nithirium gets rid of Java for rendering entirely.**

The JVM's Garbage Collector causes lag spikes. Manual C++ memory management doesn't.

```
❌  Vanilla Minecraft   →  constant GC lag spikes, low FPS
⚠️  Sodium + Lithium    →  better, but still limited by the JVM
✅  Nithirium           →  C++ renderer, zero GC interference
```

**Target:** Players with Celeron / Pentium / old i3, 2-4GB RAM, integrated graphics who want to play on Java servers without the lag.

---

## 🏗️ Architecture

```
Minecraft Java 1.18.2 (vanilla)
        │
        │  -javaagent: NithiriumAgent.jar
        ▼
┌─────────────────────────────┐
│     NithiriumAgent (Java)   │
│  ┌─────────────────────┐    │
│  │ ASM Transformer     │    │  ← intercepts LevelRenderer
│  │ JNI Bridge          │    │  ← one-time init only
│  │ Shared Memory       │    │  ← zero GC pressure
│  └─────────────────────┘    │
└────────────┬────────────────┘
             │
             ▼
┌─────────────────────────────┐
│    NithiriumCore (C++)      │
│  ┌─────────────────────┐    │
│  │ BGFX                │    │  ← auto-picks best backend
│  │ Chunk Mesher (SIMD) │    │  ← AVX2 / SSE2 fallback
│  │ Frustum Culling     │    │
│  │ Draw Call Batching  │    │
│  └─────────────────────┘    │
└─────────────────────────────┘
             │
             ▼
   OpenGL / Vulkan / DX11
   (auto-selected by BGFX)
```

Java handles **game logic**. C++ handles **rendering**. Shared memory connects them with zero overhead.

---

## ✅ Compatibility

| | Compatible? |
|---|---|
| 🌐 Vanilla servers | ✅ Yes |
| 🏴‍☠️ Cracked / offline servers | ✅ Yes |
| 🧩 Client-side mods | ❌ No (intentional) |
| ⚙️ Forge / Fabric | ❌ No (intentional) |
| 🎮 Minecraft version | 1.18.2 only (for now) |

> Server-side mods work fine — the server doesn't care how you render the world.

---

## 🛠️ Stack

| Component | Technology |
|---|---|
| Agent & Hooks | Java 17 + ASM 9.x |
| Java↔C++ Bridge | JNI + Direct ByteBuffer |
| Rendering Engine | C++17 + BGFX |
| Rendering Backend | OpenGL 2.1 / 3.3 / Vulkan / DX11 (auto) |
| Build System | GitHub Actions (no local compiler needed) |

---

## 🗺️ Roadmap

```
v0.1  🔨  Base structure, ASM hooks, BGFX window
v0.2  📋  Extract real chunk data from Minecraft
v0.3  📋  Render first chunk in BGFX
v0.4  📋  Replicate vanilla 1.18.2 shaders exactly
v0.5  📋  Full world rendering
v1.0  📋  NLauncher — install and play in one click
```

---

## 📦 Building

Nithirium builds entirely on **GitHub Actions**. You don't need Visual Studio or MinGW.

Push to the repo → download artifacts from the Actions tab:

```
NithiriumAgent.jar
NithiriumCore.dll
```

---

## 🚀 Installing (manual, pre-NLauncher)

**1.** Download `NithiriumAgent.jar` and `NithiriumCore.dll` from the latest Actions run

**2.** Place both in a folder, e.g. `C:\Nithirium\`

**3.** In TLauncher Legacy / SKLauncher, edit your 1.18.2 installation and add to JVM args:
```
-javaagent:"C:\Nithirium\NithiriumAgent.jar" -Djava.library.path="C:\Nithirium"
```

**4.** Launch and enjoy

---

## ⚠️ Status

> Early development. Nothing renders yet.
> The hook infrastructure is in place — actual rendering replacement is the next milestone.

---

<div align="center">

*Built for the players who can't afford Bedrock but deserve good FPS.*

</div>
