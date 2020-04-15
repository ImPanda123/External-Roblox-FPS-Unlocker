#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>

using namespace std;

DWORD dwGetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
	DWORD dwModuleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 ModuleEntry32 = { 0 };
	ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &ModuleEntry32))
	{
		do {
			if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
			{
				dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
				break;
			}
		} while (Module32Next(hSnapshot, &ModuleEntry32));
	}
	CloseHandle(hSnapshot);
	return dwModuleBaseAddress;
}

int main()
{
	HWND hGameWindow;
	DWORD pID;
	HANDLE pHandle;
	char moduleName[] = "RobloxPlayerBeta.exe";
	DWORD baseAddress;
	DWORD AdressValue;
	DOUBLE NewAdressValue = 0.000006;

	cout << "External FPS Unlocker" << endl;

	while (true)
	{
		hGameWindow = FindWindow(NULL, "Roblox");
		GetWindowThreadProcessId(hGameWindow, &pID);
		pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
		DWORD clientBase = dwGetModuleBaseAddress(_T(moduleName), pID);
		ReadProcessMemory(pHandle, (LPCVOID)(clientBase + 0x01EB0910), &baseAddress, sizeof(baseAddress), NULL);
		AdressValue = baseAddress + 0x128;
		WriteProcessMemory(pHandle, (LPVOID)(AdressValue), &NewAdressValue, sizeof(NewAdressValue), 0);
		CloseHandle(pHandle);
		Sleep(1000);
	}
}
