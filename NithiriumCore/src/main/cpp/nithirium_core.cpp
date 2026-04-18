#include "shared_memory.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>
#include <stdexcept>
#include <iostream>

#if BX_PLATFORM_WINDOWS
#include <windows.h>
#endif

SharedMemory* g_shared_memory = nullptr;

namespace {
    uint32_t s_reset = BGFX_RESET_VSYNC;
    uint32_t s_width = 1280;
    uint32_t s_height = 720;
}

#if BX_PLATFORM_WINDOWS
LRESULT CALLBACK wndProc(HWND _hWnd, UINT _id, WPARAM _wParam, LPARAM _lParam) {
    switch (_id) {
        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(_hWnd, _id, _wParam, _lParam);
    }
}
#endif

void run_render_loop() {
    void* native_window_handle = nullptr;
#if BX_PLATFORM_WINDOWS
    WNDCLASSW wnd;
    memset(&wnd, 0, sizeof(wnd));
    wnd.lpszClassName = L"NithiriumWindowClass";
    wnd.lpfnWndProc = wndProc;
    wnd.style = CS_HREDRAW | CS_VREDRAW;
    wnd.hInstance = GetModuleHandle(nullptr);
    RegisterClassW(&wnd);
    HWND hwnd = CreateWindowW(wnd.lpszClassName, L"Nithirium Core", WS_OVERLAPPEDWINDOW, 0, 0, s_width, s_height, NULL, NULL, wnd.hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW);
    native_window_handle = hwnd;
#else
    throw std::runtime_error("Platform not supported yet.");
#endif

    if (!native_window_handle) {
        std::cerr << "[NithiriumCore] FATAL: Failed to create window." << std::endl;
        return;
    }

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    init.resolution.width = s_width;
    init.resolution.height = s_height;
    init.resolution.reset = s_reset;
    init.platformData.nwh = native_window_handle;

    if (!bgfx::init(init)) {
        std::cerr << "[NithiriumCore] FATAL: BGFX init failed." << std::endl;
        return;
    }

    std::cout << "[NithiriumCore] BGFX Initialized. Renderer: " << bgfx::getRendererName(bgfx::getCaps()->rendererType) << std::endl;
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    bool running = true;
    while (running) {
#if BX_PLATFORM_WINDOWS
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) { running = false; break; }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (!IsWindow(static_cast<HWND>(native_window_handle))) running = false;
        if (!running) break;
#endif
        const bx::Vec3 at  = { 0.0f, 0.0f,  0.0f };
        const bx::Vec3 eye = { 0.0f, 0.0f, -5.0f };
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, float(s_width) / float(s_height), 0.1f, 1000.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);
        bgfx::setViewRect(0, 0, 0, uint16_t(s_width), uint16_t(s_height));
        bgfx::touch(0);
        bgfx::frame();
    }

    bgfx::shutdown();
    std::cout << "[NithiriumCore] BGFX shut down." << std::endl;
}
