#include "SearchAllFileOnSignature.h"

VOID SearchAllFileOnSignature::Scaning_Directories(HANDLE hFile, LPWSTR Path, BYTE* Signature, LPWSTR str, double& indexFile, LPWSTR infectedFile)
{
	FindFiles f(Path);
	LPWSTR strTmp = (LPWSTR)calloc(_SIZE_ + 1, sizeof(LPWSTR));
	DWORD iNW;
	DWORD iNR;
	OVERLAPPED olf{};
	LARGE_INTEGER li{};

	li.QuadPart = 0;
	olf.Offset = 0xFFFFFFFF;
	olf.OffsetHigh = 0xFFFFFFFF;

	while (f.Next())
	{
		wcscat_s(strTmp, _SIZE_, f.szCurPath.c_str());
		wcscat_s(strTmp, _SIZE_, L"\\");
		wcscat_s(strTmp, _SIZE_, f.Info.cFileName);
		if (_exten(strTmp, L"exe", L'.') || _exten(strTmp, L"dll", L'.') || _exten(strTmp, L"txt", L'.'))
		{
			if (Search_Signature(strTmp, Signature))
			{
				wcscat_s(strTmp, _SIZE_, L"\r\n");
				WriteFile(this->hFile, strTmp, wcslen(strTmp) * sizeof(WCHAR), &iNW, &olf);
				memset(infectedFile, L'\0', _SIZE_);
				wcscpy_s(infectedFile, _SIZE_, strTmp);
			}
			indexFile++;
		}
		wcscpy_s(str, _SIZE_, strTmp);
		memset(strTmp, L'\0', _SIZE_);
	}
}

DWORD SearchAllFileOnSignature::File_Size(LPWSTR full_fale_name)
{
	DWORD dwSize = 0;
	HANDLE hFile = CreateFile(full_fale_name,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return NULL;
	dwSize = GetFileSize(hFile, NULL);
	if (dwSize == INVALID_FILE_SIZE)
		return NULL;
	CloseHandle(hFile);
	return dwSize;
}

BOOL SearchAllFileOnSignature::Search_Signature(LPWSTR full_fale_name, BYTE* VIRUS_SIGNATURE)
{
	DWORD SIZE_FILE = File_Size(full_fale_name);
	HANDLE hFile = CreateFileW(full_fale_name, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	std::wstring file = std::wstring(full_fale_name).substr(std::wstring(full_fale_name).rfind('\\') + 1);
	LPCWSTR f1 = file.c_str();
	file.clear();

	HANDLE hMap = CreateFileMappingW(hFile, NULL, PAGE_READONLY, NULL, SIZE_FILE, f1);

	if (hMap != NULL)
		pBuffer = (BYTE*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, SIZE_FILE);
	else if (GetLastError() != 87 && GetLastError() != 1006)
		wprintf(L"ERROR_CREATE_FILE_MAPPING -> %d\r\n", GetLastError());
	if ((GetLastError() == 87 && SIZE_FILE == 0) || GetLastError() == 1006)
		return false;

	if (pBuffer == 0)
		return wprintf(L"%s - %d = %d\n", full_fale_name, GetLastError(), SIZE_FILE);

	BYTE* found = new BYTE[strlen((char*)VIRUS_SIGNATURE)]{};
	INT temp = 0;
	if (strlen((char*)VIRUS_SIGNATURE) > 0 && (GetFileAttributesW(full_fale_name) != INVALID_FILE_ATTRIBUTES))
	{
		for (INT i = 0; i < SIZE_FILE; i++)
		{
			/*if ((GetFileAttributesW(full_fale_name) == INVALID_FILE_ATTRIBUTES))
				return false;*/

			if (pBuffer[i] != VIRUS_SIGNATURE[0])
				continue;
			for (INT k = 0; k < strlen((char*)VIRUS_SIGNATURE); k++)
			{
				if (pBuffer[i + k] == VIRUS_SIGNATURE[k])
				{
					if (k == strlen((char*)VIRUS_SIGNATURE) - 1)
					{
						CloseHandle(hFile);
						Free();
						CloseHandle(hMap);
						temp = i;
						delete[] found;
						return true;
					}
					found[k] = pBuffer[i + k];
				}
				else break;
			}
		}
	}
	CloseHandle(hFile);
	if (pBuffer != NULL)
		Free();
	if (hMap != NULL)
		CloseHandle(hMap);
	return false;
}

std::vector<LPWSTR> SearchAllFileOnSignature::WriteDrive(double& indexFile, BYTE* Signature, LPWSTR str, LPWSTR infectedFile)
{
	std::vector<LPWSTR> path_vec;
	WIN32_FIND_DATA wdf;
	SetCurrentDirectory(this->path);
	HANDLE search = FindFirstFile(MASK_FOUND, &wdf);
	LPWSTR strTmp = (LPWSTR)calloc(_SIZE_ + 1, sizeof(WCHAR));

	while (FindNextFile(search, &wdf))
	{
		if ((wcscmp(wdf.cFileName, L".") && wcscmp(wdf.cFileName, L"..")))
		{
			if ((wdf.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (!(wdf.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)))
			{
				GetCurrentDirectory(_SIZE_, strTmp);
				if (ItsNotDriver)
					wcscat_s(strTmp, _SIZE_, L"\\");

				wcscat_s(strTmp, _SIZE_, wdf.cFileName);
				LPWSTR strTmp_ = (LPWSTR)malloc((wcslen(strTmp) + 1) * sizeof(WCHAR));
				wcscpy_s(strTmp_, wcslen(strTmp) + 1, strTmp);
				if (std::wstring(strTmp_).substr(std::wstring(strTmp_).rfind('\\') + 1) != L"Recovery")
					path_vec.push_back(strTmp_);
			}
			else
			{
				GetCurrentDirectory(_SIZE_, strTmp);
				if (ItsNotDriver)
					wcscat_s(strTmp, _SIZE_, L"\\");

				wcscat_s(strTmp, _SIZE_, wdf.cFileName);
				LPWSTR strTmp_ = (LPWSTR)malloc((wcslen(strTmp) + 1) * sizeof(WCHAR));
				wcscpy_s(strTmp_, wcslen(strTmp) + 1, strTmp);
				if (_exten(strTmp_, L"exe", L'.') || _exten(strTmp_, L"txt", L'.') || _exten(strTmp_, L"dll", L'.'))
				{
					wcscat_s(strTmp, _SIZE_, L"\r\n");
					DWORD iCount = 0;
					if (Search_Signature(strTmp_, Signature))
						wcscpy_s(infectedFile, _SIZE_, strTmp_);
					indexFile++;
				}
				wcscpy(str, strTmp_);
			}
		}
	}
	free(strTmp);
	FindClose(search);
	return path_vec;
}

BOOL SearchAllFileOnSignature::_exten(LPWSTR str, LPCWSTR exten, WCHAR const t)
{
	DWORD f = 0;
	for (DWORD i = 0; i < wcslen(str); i++)
		if (str[wcslen(str) - i - 1] == exten[wcslen(exten) - i - 1] && str[wcslen(str) - i - 1] != t)
			++f;
		else if (((str[wcslen(str) - i - 1] != exten[wcslen(exten) - i - 1])) || (str[wcslen(str) - i - 1] == t)) break;
	if (f == wcslen(exten))
		return true;
	else return false;
}

SearchAllFileOnSignature::SearchAllFileOnSignature(LPWSTR PATH, LPWSTR DEST_PATH_)
{
	this->path = PATH;
	this->DestPath = DEST_PATH_;
}

VOID SearchAllFileOnSignature::StartScanOnSignature(BYTE* Signature, LPWSTR str, double& indexFile, LPWSTR infectedFile)
{
	indexFile = 0;
	this->hFile = CreateFileW(this->DestPath, GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (std::wstring(this->path).substr(std::wstring(this->path).rfind('\\') - 1).c_str()[0] == L':' && std::wstring(this->path).substr(std::wstring(this->path).rfind('\\') + 1).c_str()[0] == L'\0')
		ItsNotDriver = false;
	else
		ItsNotDriver = true;
	std::vector<LPWSTR> path_Directory_ALL = WriteDrive(indexFile, Signature, str, infectedFile);
	for (LPWSTR& t : path_Directory_ALL)
		Scaning_Directories(this->hFile, t, Signature, str, indexFile, infectedFile);

	path_Directory_ALL.clear();
}

VOID SearchAllFileOnSignature::Free()
{
	UnmapViewOfFile(pBuffer);
}