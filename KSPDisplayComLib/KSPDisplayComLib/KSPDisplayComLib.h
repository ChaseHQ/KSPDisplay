#include <Windows.h>
#include <SetupAPI.h>
#include <string>
#include <algorithm>

#define KSPDCL_EXPORT __declspec(dllexport)
#define KSPDCL_CDECL __cdecl

#define DEVICE_ID "vid_04d8&pid_0042"

const GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30};

HANDLE hDeviceRead = NULL;
HANDLE hDeviceWrite = NULL;
bool bDeviceConnected = false;

extern "C" {
	// Exports
	KSPDCL_EXPORT bool KSPDCL_CDECL connectToDevice(void);
	KSPDCL_EXPORT void KSPDCL_CDECL closeDevice(void);
	KSPDCL_EXPORT void KSPDCL_CDECL sendInteger(int);
}