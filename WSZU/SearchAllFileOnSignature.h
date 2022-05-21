#ifndef H_SearchAllFileOnSignature
#define H_SearchAllFileOnSignature

#include "FindFiles.h"
#define MASK_FOUND L"*"
#define _SIZE_ 300
#define _SIZE_SIGNATURE_VIRUS 1024
#pragma warning(disable : 4996)

class SearchAllFileOnSignature
{
private:
	BOOL ItsNotDriver = false;
	BYTE* pBuffer = NULL;
	HANDLE hFile = NULL;
	HWND hWnd = nullptr;
	LPWSTR path = 0, DestPath = 0, STR = 0;
	VOID Scaning_Directories(HANDLE, LPWSTR, BYTE*, LPWSTR, double& procent, LPWSTR infectedFile);
	DWORD File_Size(LPWSTR);
	BOOL Search_Signature(LPWSTR, BYTE*);
	std::vector<LPWSTR> WriteDrive(double& procent, BYTE* Signature, LPWSTR str, LPWSTR infectedFile);
	BOOL _exten(LPWSTR, LPCWSTR, WCHAR const);
	//std::string hasha(LPWSTR);
public:
	SearchAllFileOnSignature(LPWSTR PATH, LPWSTR DEST_PATH);//»нициализаци€ класса
	VOID StartScanOnSignature(BYTE* Signature, LPWSTR str, double& indexFile, LPWSTR infectedFile);//—тарт сканировани€ в заданной директории/диск
	VOID Free();//ќсвобождение зан€той оперативной пам€ти
};

#endif // !H_SearchAllFileOnSignature

