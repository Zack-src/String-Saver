#include "..\Link.h"


static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
HWND hwnd;

template <typename T>
static ImVec4 color(T r, T g, T b, T a = 255)
{
    return
    {
        static_cast<float>(r) / 255.0f,
        static_cast<float>(g) / 255.0f,
        static_cast<float>(b) / 255.0f,
        static_cast<float>(a) / 255.0f
    };
}

ImVec4 clear_color = color(230, 120, 0);

ImGuiWindowFlags window_flags = 0;
ImGuiWindowFlags OtherWindow_flags = 0;

int menu_movement_x = 0;
int menu_movement_y = 0;

POINT CursorPosition;
RECT MenuPosition;
POINT cursor_position;

void GuiStart() {

    //GUI start
    {
        ImGui::Begin("###SaverGui", NULL, window_flags);

        ImGui::SetWindowSize(ImVec2(300, 300));
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::PushItemWidth(250);
        
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, color(35, 35, 35));
            ImGui::Begin("###Title bare", NULL, OtherWindow_flags);

            ImGui::SetWindowSize(ImVec2(600, 20));
            ImGui::SetWindowPos(ImVec2(0, 0));

            if (ImGui::IsMouseClicked(0))
            {
                GetCursorPos(&CursorPosition);
                GetWindowRect(hwnd, &MenuPosition);

                menu_movement_x = CursorPosition.x - MenuPosition.left;
                menu_movement_y = CursorPosition.y - MenuPosition.top;
            }
            if ((menu_movement_y >= 0 && menu_movement_y <= 30) && ImGui::IsMouseDragging(0, 0.0f))
            {
                GetCursorPos(&cursor_position);
                SetWindowPos(hwnd, nullptr, cursor_position.x - menu_movement_x, cursor_position.y - menu_movement_y, 0, 0, SWP_NOSIZE);
            }
            ImGui::PopStyleColor();
            ImGui::End();
        }

        {
            ImGui::Begin("###ListkGui", NULL, OtherWindow_flags);

            ImGui::SetWindowSize(ImVec2(120, 300));
            ImGui::SetWindowPos(ImVec2(0, 35));
            ImGui::PushItemWidth(250);

            ImGui::Checkbox("Javaw", &ScanJavaw);
            ImGui::Checkbox("Explorer", &ScanExplorer);
            ImGui::Checkbox("Dwm", &ScanDWM);
            ImGui::Checkbox("Lsass", &ScanLsass);
            ImGui::Checkbox("DPS", &ScanDPS);
            ImGui::Checkbox("PcaSvc", &ScanPcaSvc);

            if (ImGui::Button("Start")) {
                unsigned __int64 ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                StartSave();
                unsigned __int64 ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                    
                std::cout << "End in : " << ms2 - ms1 << "ms\n\n";
            }
                ImGui::End();
        }

        {
            ImGui::Begin("###RClickGui", NULL, OtherWindow_flags);

            ImGui::SetWindowSize(ImVec2(200, 400));
            ImGui::SetWindowPos(ImVec2(120, 35));
            ImGui::PushItemWidth(250);

            ImGui::Checkbox("Show Console", &ShowConsole);
            ImGui::Checkbox("Skip Image for javaw", &SkipImage);

            if (ShowConsole) {
                ShowWindow(GetConsoleWindow(), 1);
            }
            else {
                ShowWindow(GetConsoleWindow(), 0);
            }
            ImGui::End();
            
        }

        ImGui::End();
    }
}

void ConfigGUI() {

    D3DCOLOR clear_col_dx;
    HRESULT result;

    OtherWindow_flags |= ImGuiWindowFlags_NoTitleBar;
    OtherWindow_flags |= ImGuiWindowFlags_NoMove;
    OtherWindow_flags |= ImGuiWindowFlags_NoResize;
    OtherWindow_flags |= ImGuiWindowFlags_NoNav;

    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;             
    window_flags |= ImGuiWindowFlags_NoNav;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        GetModuleHandle(NULL),
        NULL,
        NULL,
        NULL,
        NULL,
        _T("String Saver"),
        NULL
    };

    ::RegisterClassEx(&wc);
    hwnd = ::CreateWindow(
        wc.lpszClassName,
        _T("String Saver"),
        WS_POPUP,
        500, // pos spawn X 
        200, // pos spawn Y
        300, // size X
        300, // size Y
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0;
    style.WindowRounding = 0;


    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;
    
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        
        ImGui::NewFrame();
        GuiStart();
        ImGui::EndFrame();
        
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}