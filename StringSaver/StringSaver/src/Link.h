#include <windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <map>
#include <stdio.h>
#include <thread>
#include <future>
#include <tchar.h>
#include <d3d9.h>

#include "..\src\gui\imgui\imgui.h"
#include "..\src\gui\imgui\imgui_impl_win32.h"
#include "..\src\gui\imgui\imgui_impl_dx9.h"

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void StartSave();
void ConfigGUI();

void Save_Mem(int procs_id, std::string saveFile);
DWORD Svchost_Pid(const char* serviceName);
DWORD Process_Pid(std::string name);

void ProcessSave(std::string processName, bool is_svchost);

extern bool ScanJavaw;
extern bool ScanExplorer;
extern bool ScanLsass;
extern bool ScanDWM;
extern bool ScanDPS;
extern bool ScanPcaSvc;

extern bool ShowConsole;
extern bool SkipImage;