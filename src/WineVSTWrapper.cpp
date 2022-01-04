#include <windows.h>
#include <stdio.h>
#include "aeffectx.hpp"

typedef AEffect *WINAPI (*VSTPluginMainFunc)(audioMasterCallback);

static volatile HMODULE hModule = NULL;
static volatile HMODULE hPlugin = NULL;
static volatile VSTPluginMainFunc instantiate = nullptr;

BOOL DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
  if (hModule == NULL) {
    hModule = hinstDLL;
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
  char fullpath[MAX_PATH + 1];
  char dllpath[MAX_PATH + 1];

  ::GetModuleFileNameA(hModule, fullpath, MAX_PATH);
  snprintf(dllpath, strlen(fullpath) + 4, "%s.so", fullpath);

  hPlugin = ::LoadLibraryA(dllpath);
  if (hPlugin == NULL) {
    return nullptr;
  }

  instantiate = reinterpret_cast<VSTPluginMainFunc>(
      (::GetProcAddress(hPlugin, "VSTPluginMain")));
  if (instantiate == nullptr) {
    return nullptr;
  }

  return instantiate(callback);
};
