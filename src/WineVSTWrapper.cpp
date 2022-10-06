#include "aeffectx.hpp"
#include <stdio.h>
#include <windows.h>

typedef AEffect *WINAPI (*VSTPluginMainFunc)(audioMasterCallback);

static volatile HMODULE hModule = NULL;
static volatile HMODULE hPlugin = NULL;
static volatile VSTPluginMainFunc instantiate = nullptr;

BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
  if (hModule == NULL) {
    hModule = hinstDLL;
  }

  if (hPlugin == NULL) {
    char proxyDllPath[MAX_PATH + 1];
    char winegccDllPath[MAX_PATH + 4];

    ::GetModuleFileNameA(hModule, proxyDllPath, MAX_PATH);
    snprintf(winegccDllPath, strlen(proxyDllPath) + 4, "%s.so", proxyDllPath);

    hPlugin = ::LoadLibraryA(winegccDllPath);

    if (hPlugin == NULL) {
      return FALSE;
    }

    instantiate = reinterpret_cast<VSTPluginMainFunc>(
        reinterpret_cast<void *>(::GetProcAddress(hPlugin, "VSTPluginMain")));
  }

  switch (fdwReason) {
  case DLL_PROCESS_DETACH:
    if (instantiate != nullptr) {
      instantiate = nullptr;
    }

    if (hPlugin != NULL) {
      ::FreeLibrary(hPlugin);
      hPlugin = NULL;
    }

  default:
    break;
  }

  return TRUE;
};

extern "C" __declspec(dllexport) AEffect *VSTPluginMain(
    audioMasterCallback callback) {

  if (instantiate != nullptr) {
    return instantiate(callback);
  }

  return nullptr;
};
