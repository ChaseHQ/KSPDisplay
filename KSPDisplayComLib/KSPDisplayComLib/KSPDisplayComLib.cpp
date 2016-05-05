#include "KSPDisplayComLib.h"

bool APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	
	return true;
}

KSPDCL_EXPORT bool KSPDCL_CDECL connectToDevice(void) {

	bool bReturnStatus = false;
	HDEVINFO hDeviceInfoTable = NULL;

	// Get all Device Interfaces that are currently Present
	hDeviceInfoTable = SetupDiGetClassDevs(&InterfaceClassGuid,NULL,NULL,DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	// Hold info about devices we Enumerate
	PSP_DEVICE_INTERFACE_DATA pInterfaceDS = new SP_DEVICE_INTERFACE_DATA;
	pInterfaceDS->cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	// Hold our device info data
	SP_DEVINFO_DATA sDevInfoData;
	// Hold our Detailed Interface
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetailedInterfaceDS = new SP_DEVICE_INTERFACE_DETAIL_DATA;

	DWORD dwDeviceIndex = 0;
	DWORD dwRegType = 0;
	DWORD dwRegSize = 0;
	DWORD dwDetailedSize = 0;

	PBYTE pPropertyBuffer = 0;

	while (SetupDiEnumDeviceInterfaces(hDeviceInfoTable,NULL,&InterfaceClassGuid, dwDeviceIndex, pInterfaceDS) && !bDeviceConnected) {
		if (GetLastError() == ERROR_NO_MORE_ITEMS) 
			break; // No more items left

		// Get the Hardware Info about the Device Enumerated
		sDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		SetupDiEnumDeviceInfo(hDeviceInfoTable,dwDeviceIndex,&sDevInfoData);

		// Now we are holding the Device Info Data, Get the Property Size of the Hardware Id
		SetupDiGetDeviceRegistryProperty(hDeviceInfoTable,&sDevInfoData,SPDRP_HARDWAREID,&dwRegType,NULL,0,&dwRegSize);
		// dwRegSize should now have the size of the Hardware ID, Allocate Space For it
		pPropertyBuffer = new BYTE[dwRegSize];
		if (pPropertyBuffer == NULL) // check to see we actually have space 
			break;

		// Check to see if this is our device
		SetupDiGetDeviceRegistryProperty(hDeviceInfoTable,&sDevInfoData,SPDRP_HARDWAREID,&dwRegType,pPropertyBuffer,dwRegSize,NULL);

		// Check to see if this is the right Device now by comparing strings
		std::string sKnownDeviceID = DEVICE_ID;
		std::string sFoundDeviceID = (char *)pPropertyBuffer;
		// Convert to lower
		std::transform(sKnownDeviceID.begin(), sKnownDeviceID.end(), sKnownDeviceID.begin(), ::tolower);
		std::transform(sFoundDeviceID.begin(), sFoundDeviceID.end(), sFoundDeviceID.begin(), ::tolower);
		if (sFoundDeviceID.find(sKnownDeviceID) != std::string::npos) { // They are Equal! Found Device!
			// Get the size of the detailed structure
			pDetailedInterfaceDS->cbSize = sizeof (SP_DEVICE_INTERFACE_DETAIL_DATA);
			// populate the struct size
			SetupDiGetDeviceInterfaceDetail(hDeviceInfoTable,pInterfaceDS,NULL,NULL,&dwDetailedSize,NULL);
			delete pDetailedInterfaceDS;
			pDetailedInterfaceDS = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(dwDetailedSize);
			if (pDetailedInterfaceDS == NULL) 
				break;
			// Now actually get the detail device including path
			pDetailedInterfaceDS->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			SetupDiGetDeviceInterfaceDetail(hDeviceInfoTable,pInterfaceDS,pDetailedInterfaceDS,dwDetailedSize,NULL,NULL);
			// Now Open Up Handles for Reading and Writing
			hDeviceWrite = CreateFile(pDetailedInterfaceDS->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
			if (GetLastError() == ERROR_SUCCESS)
				bReturnStatus = true;
			hDeviceRead = CreateFile(pDetailedInterfaceDS->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
			if (GetLastError() == ERROR_SUCCESS)
				bReturnStatus = true;

			free(pDetailedInterfaceDS);
			bDeviceConnected = true;
		}

		// Clean Up
		delete pPropertyBuffer;
		pInterfaceDS->cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		// Next Device Check
		++dwDeviceIndex;
	}

	SetupDiDestroyDeviceInfoList(hDeviceInfoTable); // Clean up 

	return bReturnStatus;
}

KSPDCL_EXPORT void KSPDCL_CDECL closeDevice(void) {
	if (!bDeviceConnected) return;

	CloseHandle(hDeviceRead);
	CloseHandle(hDeviceWrite);
}

KSPDCL_EXPORT void KSPDCL_CDECL sendInteger(int iRecieved) {
	if (!bDeviceConnected) return;

	DWORD dwBytesWritten = 0;
	BYTE bOutBuffer[65] = {};

	bOutBuffer[1] = 0x80;

	*(int *)&bOutBuffer[2] = iRecieved;

	WriteFile(hDeviceWrite,&bOutBuffer,65,&dwBytesWritten,0);
}