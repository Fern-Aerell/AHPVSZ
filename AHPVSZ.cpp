#define PROGRAM_NAME "AHPVSZ"
#define PROGRAM_VERSION "1.0.0 - RELEASE"
#define WINDOW_GAME_TITLE "Plants vs. Zombies"
#define GAME_NAME "popcapgame1.exe"

#include <iostream>
#include <windows.h>
#include<TlHelp32.h>
#include <tchar.h>
#include <vector>

bool unlimited_sun = false;
bool plants_fast_recharge = false;
bool zombie_one_hit = false;
bool gameIsRunning = false;

DWORD pID = NULL;
HANDLE processHandle = NULL;
char gameName[] = GAME_NAME;

void clearConsole() {
	system("cls");
}

void deactivateAllCheat() {
	unlimited_sun = false;
	plants_fast_recharge = false;
	zombie_one_hit = false;
}

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
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

int main(int agrc, const char* argv) {

	while (TRUE) {
		SetConsoleTitle(PROGRAM_NAME " " PROGRAM_VERSION);
		clearConsole();
		HWND hGameWindow = FindWindow(NULL, WINDOW_GAME_TITLE);
		if (hGameWindow == NULL)
		{
			gameIsRunning = false;
			deactivateAllCheat();
			std::cout << "\033[1;31m" << "Plant Vs Zombie Not Running." << "\033[0m\n" << std::endl;
			std::cout << "Note : Please Open Game For Show Cheat Menu..." << std::endl;
		}
		else
		{
			gameIsRunning = true;
			std::cout << "\033[1;32m" << "Plant Vs Zombie Is Running..." << "\033[0m\n" << std::endl;
			std::cout << "Cheat Hotkey : " << std::endl;
			if (unlimited_sun) 
			{
				std::cout << "\033[1;32m" << "F5 - Unlimited Sun" << "\033[0m" << std::endl;
			}
			else {
				std::cout << "\033[1;31m" << "F5 - Unlimited Sun" << "\033[0m" << std::endl;
			}
			if (plants_fast_recharge)
			{
				std::cout << "\033[1;32m" << "F6 - Plants Fast Recharge" << "\033[0m" << std::endl;
			}
			else {
				std::cout << "\033[1;31m" << "F6 - Plants Fast Recharge" << "\033[0m" << std::endl;
			}
			if (zombie_one_hit)
			{
				std::cout << "\033[1;32m" << "F7 - Zombie One Hit" << "\033[0m" << std::endl;
			}
			else {
				std::cout << "\033[1;31m" << "F7 - Zombie One Hit" << "\033[0m" << std::endl;
			}
		}
		std::cout << "\nAbout : " << std::endl;
		std::cout << "Create By AerellHack" << std::endl;
		std::cout << "Version " << PROGRAM_VERSION << std::endl;
		if (gameIsRunning)
		{
			if (pID == NULL) {
				GetWindowThreadProcessId(hGameWindow, &pID);
			}

			if (processHandle == NULL) {
				processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
				if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL) {
					std::cout << "\033[1;31m" << "Failed to open process" << "\033[0m\n" << std::endl;
					return 0;
				}
			}

			DWORD gameBaseAddress = GetModuleBaseAddress(_T(gameName), pID);

			//std::cout << "\nDebug : " << std::endl;
			//std::cout << "PID : " << pID << std::endl;
			//std::cout << "ProcessHandle : " << processHandle << std::endl;
			//std::cout << "GameBaseAddress : " << gameBaseAddress << std::endl;

			if (unlimited_sun) {
				DWORD offsetGameToBaseAdress = 0x002A77BC;
				std::vector<DWORD> pointsOffsets{ 0x6C,0x2C,0x768,0x1C,0x140,0xCC,0x5560 };
				DWORD baseAddress = NULL;
				ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress + offsetGameToBaseAdress), &baseAddress, sizeof(baseAddress), NULL);
				DWORD pointsAddress = baseAddress;
				for (int i = 0; i < pointsOffsets.size() - 1; i++)
				{
					ReadProcessMemory(processHandle, (LPVOID)(pointsAddress + pointsOffsets.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
				}
				pointsAddress += pointsOffsets.at(pointsOffsets.size() - 1);
				int newPoints = 9999;
				WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &newPoints, 4, 0);
			}

			if (GetAsyncKeyState(VK_F5)) {
				unlimited_sun = !unlimited_sun;
				Beep((unlimited_sun) ? 1500 : 500, 100);
				Sleep(1000);
			}
			if (GetAsyncKeyState(VK_F6)) {
				plants_fast_recharge = !plants_fast_recharge;
				if (plants_fast_recharge) {
					DWORD addressRechargeDuration = 0x0048728C;
					unsigned char patchBytes[] = { 0x83, 0x47, 0x24, 0x79, 0x8B, 0x47, 0x24 };
					WriteProcessMemory(processHandle, (LPVOID)(addressRechargeDuration), patchBytes, sizeof(patchBytes), NULL);
				}
				else {
					DWORD addressRechargeDuration = 0x0048728C;
					unsigned char patchBytes[] = { 0x83, 0x47, 0x24, 0x01, 0x8B, 0x47, 0x24 };
					WriteProcessMemory(processHandle, (LPVOID)(addressRechargeDuration), patchBytes, sizeof(patchBytes), NULL);
				}
				Beep((plants_fast_recharge) ? 1500 : 500, 100);
				Sleep(1000);
			}
			if (GetAsyncKeyState(VK_F7)) {
				zombie_one_hit = !zombie_one_hit;
				if (zombie_one_hit) {
					DWORD addressZombieOneHit1 = 0x0053130F;
					unsigned char patchBytes1[] = { 0x2B, 0x7C, 0x24, 0x00, 0x89, 0x44, 0x24, 0x1C };
					WriteProcessMemory(processHandle, (LPVOID)(addressZombieOneHit1), patchBytes1, sizeof(patchBytes1), NULL);
					DWORD addressZombieOneHit2 = 0x00531053;
					unsigned char patchBytes2[] = { 0xEB, 0x09, 0x6A, 0x00, 0x8B, 0xC5 };
					WriteProcessMemory(processHandle, (LPVOID)(addressZombieOneHit2), patchBytes2, sizeof(patchBytes2), NULL);
					DWORD addressZombieOneHit3 = 0x0053105E;
					unsigned char patchBytes3[] = { 0x2B, 0x8D, 0xD0, 0x00, 0x00, 0x00 };
					WriteProcessMemory(processHandle, (LPVOID)(addressZombieOneHit3), patchBytes3, sizeof(patchBytes3), NULL);
					DWORD addressZombieOneHit4 = 0x00530CA1;
					unsigned char patchBytes4[] = { 0x89, 0x86, 0xDC, 0x00, 0x00, 0x00 };
					WriteProcessMemory(processHandle, (LPVOID)(addressZombieOneHit4), patchBytes4, sizeof(patchBytes4), NULL);
				}
				else {
					DWORD addressZombieOneHit1 = 0x0053130F;
					unsigned char patchBytes1[] = { 0x2B, 0x7C, 0x24, 0x20, 0x89, 0x44, 0x24, 0x1C };
					WriteProcessMemory(processHandle, (LPVOID)(addressZombieOneHit1), patchBytes1, sizeof(patchBytes1), NULL);
					DWORD addressZombieOneHit2 = 0x00531053;
					unsigned char patchBytes2[] = { 0x74, 0x09, 0x6A, 0x00, 0x8B, 0xC5 };
					WriteProcessMemory(processHandle, (LPVOID)(addressZombieOneHit2), patchBytes2, sizeof(patchBytes2), NULL);
					DWORD addressZombieOneHit3 = 0x0053105E;
					unsigned char patchBytes3[] = { 0x8B, 0x8D, 0xD0, 0x00, 0x00, 0x00 };
					WriteProcessMemory(processHandle, (LPVOID)(addressZombieOneHit3), patchBytes3, sizeof(patchBytes3), NULL);
					DWORD addressZombieOneHit4 = 0x00530CA1;
					unsigned char patchBytes4[] = { 0x29, 0x86, 0xDC, 0x00, 0x00, 0x00 };
					WriteProcessMemory(processHandle, (LPVOID)(addressZombieOneHit4), patchBytes4, sizeof(patchBytes4), NULL);
				}
				Beep((zombie_one_hit) ? 1500 : 500, 100);
				Sleep(1000);
			}
		}
		Sleep(10);
	}

	return 0;
}