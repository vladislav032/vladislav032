#pragma once

#include "SearchAllFileOnSignature.h"
#include <shlobj.h>

#define ID_hButtonScanStart 1			//ID ?????? ?????
#define ID_hButtonStopSearch 2			//ID ?????? ????
#define ID_hButtonDiscriptors 3			//ID ?????? ??????? ???????????
#define ID_hButtonDeleteInfectedFile 4	//ID ?????? ???????? ?????????????? ??????
#define ID_hListBoxWriteDriver 5		//ID listbox ??? ?????? ????? 
#define ID_hInfectedFilesListBox 6		//ID lisbox ??? ?????? ????? ?????????? ??????

#define DEST_PATH L"log.txt"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

BYTE* VIRUS_SIGNATURE = new BYTE[_SIZE_SIGNATURE_VIRUS]{};
LPWSTR Buffer = (LPWSTR)malloc((_SIZE_ + 1) * sizeof(LPWSTR));
wchar_t STR[_SIZE_] = L"\0";
wchar_t InfectedFile[_SIZE_] = L"\0";
double indexFile = 0;
TCHAR szPath[_SIZE_];
std::vector<wchar_t*> Pahts{ 0 };
wchar_t Buff[128]{};

HANDLE Thread;
HDC hdc;
PAINTSTRUCT ps;
DWORD indexDrive, prevIndexDrive;
wchar_t buf[26]{};
bool Change_Drives = false;
HBRUSH hBrush = nullptr;
HWND hPath = nullptr;
HWND hSearchFiles = nullptr;
HWND hSignatureEdit;
HWND hPathSearch = nullptr;
bool flag = false;
HWND hButtonScanStart = nullptr;
HWND hListBoxWriteDrive = nullptr;
HWND hInfectedFilesListBox = nullptr;
HWND hButtonStopSearch = nullptr;
HWND hButtonDiscriptors = nullptr;
HWND hButtonDeleteInfectedFile = nullptr;
SearchAllFileOnSignature safos(Buffer, (LPWSTR)DEST_PATH);
LARGE_INTEGER li = {};
OVERLAPPED olf = {};
DWORD iNumberRead = 0;
HANDLE hSignatureFiles = CreateFile(L"Signature.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);