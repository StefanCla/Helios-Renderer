#pragma once

/*
*	This file is used to define any global values & functions. It is part of the precompiled header.
*	The file will never include any files like the standard libary, as this is already done via CMake.
*	For any precompiled includes, check the CMakeLists.txt in the root.
*/

// Function to assert on fail.
static inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw std::exception();
    }
}

// The buffer count used for the renderer.
static const uint32_t g_BufferCount = 3;

// The width of the client's window.
static uint32_t g_ClientWidth = 1280;
// The height of the client's window.
static uint32_t g_ClientHeight = 720;