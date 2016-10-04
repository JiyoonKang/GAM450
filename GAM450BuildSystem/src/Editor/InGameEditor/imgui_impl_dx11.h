/*******************************************************************************
 * All content ?2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: imgui_impl_dx11.h 
 * Author: 
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
// ImGui Win32 + DirectX11 binding
// You can copy and use unmodified imgui_impl_* files in your project. 
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// See main.cpp for an example of using this.
// https://github.com/ocornut/imgui
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

IMGUI_API bool        ImGui_ImplDX11_Init(void* hwnd, ID3D11Device* device, ID3D11DeviceContext* device_context);
IMGUI_API void        ImGui_ImplDX11_Shutdown();
IMGUI_API void        ImGui_ImplDX11_NewFrame();

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplDX11_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplDX11_CreateDeviceObjects();

// Handler for Win32 messages, update mouse/keyboard data.
// You may or not need this for your implementation, but it can serve as reference for handling inputs.
// Commented out to avoid dragging dependencies on <windows.h> types. You can copy the extern declaration in your code.

IMGUI_API LRESULT   ImGui_ImplDX11_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

