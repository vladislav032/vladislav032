#pragma once

#include "SearchAllFileOnSignature.h"
#include <thread>
#include <shlobj.h>

#define ID_hButtonScanStart 1			//ID кнопки старт
#define ID_hButtonStopSearch 2			//ID  нопки стоп
#define ID_hButtonDiscriptors 3			//ID  нопки выборки дискриптора
#define ID_hButtonDeleteInfectedFile 4	//ID кнопки удалени€ инфицированных файлов
#define ID_hListBoxWriteDriver 5		//ID listbox дл€ выбора диска 
#define ID_hInfectedFilesListBox 6		//ID lisbox дл€ вывода путей зараженных файлов

#define DEST_PATH L"log.txt"

BYTE* VIRUS_SIGNATURE = new BYTE[_SIZE_SIGNATURE_VIRUS]{};
LPWSTR Buffer = (LPWSTR)malloc((_SIZE_ + 1) * sizeof(LPWSTR));
wchar_t STR[_SIZE_] = L"\0";
wchar_t InfectedFile[_SIZE_] = L"\0";
double indexFile = 0;
TCHAR szPath[_SIZE_];
std::vector<wchar_t*> Pahts{ 0 };
wchar_t Buff[128]{};

std::thread thr;
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
HWND hListBoxWriteDriver = nullptr;
HWND hInfectedFilesListBox = nullptr;
HWND hButtonStopSearch = nullptr;
HWND hButtonDiscriptors = nullptr;
HWND hButtonDeleteInfectedFile = nullptr;
SearchAllFileOnSignature safos(Buffer, (LPWSTR)DEST_PATH);
LARGE_INTEGER li = {};
OVERLAPPED olf = {};
DWORD iNumberRead = 0;
HANDLE hSignatureFiles = CreateFile(L"Signature.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);