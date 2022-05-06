#pragma once

#include "deff.h"

void threadFunction(wchar_t* str, double& procent, SearchAllFileOnSignature& safos, wchar_t* InfectedFile)
{
	safos.StartScanOnSignature(VIRUS_SIGNATURE, str, procent, InfectedFile);
}
static int CALLBACK	BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg) {
	case BFFM_INITIALIZED:
		if (lpData)
			SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	case BFFM_SELCHANGED:
		if (lParam == 1)
		{
			SHGetPathFromIDList(LPITEMIDLIST(lParam), szPath);
			SendMessage(hWnd, BFFM_SETSTATUSTEXT, NULL, LPARAM(szPath));
		}
		break;
	}
	return 0;
}
BOOL GetFolder(LPCTSTR szRoot, HWND hWndOwner)
{
	bool result = false;

	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR) {
		BROWSEINFO bi;
		::ZeroMemory(&bi, sizeof bi);
		bi.ulFlags = BIF_DONTGOBELOWDOMAIN;
		bi.hwndOwner = hWndOwner;
		bi.lpszTitle = L"";
		bi.lpfn = BrowseCallbackProc;
		bi.ulFlags |= BIF_STATUSTEXT;
		bi.lParam = LPARAM(L"");

		// установка корневого каталога
		szRoot = L"";
		if (szRoot != NULL) {
			IShellFolder* pDF;
			if (SHGetDesktopFolder(&pDF) == NOERROR) {
				LPITEMIDLIST pIdl = NULL;
				ULONG        chEaten;
				ULONG        dwAttributes;
				LPOLESTR oleStr = LPOLESTR(szRoot);

				pDF->ParseDisplayName(NULL, NULL, oleStr, &chEaten, &pIdl, &dwAttributes);
				pDF->Release();

				bi.pidlRoot = pIdl;
			}
		}

		LPITEMIDLIST pidl = ::SHBrowseForFolder(&bi);
		if (pidl != NULL)
		{
			if (::SHGetPathFromIDList(pidl, szPath))
				result = true;
			pMalloc->Free(pidl);
		}
		if (bi.pidlRoot != NULL)
			pMalloc->Release();
		return result;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
	switch (message)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_hButtonScanStart && !flag)
		{
			SendMessage(hInfectedFilesListBox, LB_RESETCONTENT, 0, 0);
			Pahts.clear();
			flag = true;
			thr = std::thread(threadFunction, std::ref(STR), std::ref(indexFile), std::ref(safos), std::ref(InfectedFile));
		}
		if (LOWORD(wParam) == ID_hButtonStopSearch && flag)
		{
			flag = false;
			TerminateThread(thr.native_handle(), NULL);
			thr.detach();
			safos.Free();
		}
		if (LOWORD(wParam) == ID_hButtonDiscriptors)
		{
			if (GetFolder(Buffer, hwnd))
			{
				memset(Buffer, '\0', _SIZE_);
				wcscat_s(Buffer, _SIZE_, szPath);
			}
		}
		if (LOWORD(wParam) == ID_hButtonDeleteInfectedFile && !flag)
		{
			for (size_t i = 0; i < Pahts.size(); i++)
				DeleteFile(Pahts[i]);
		}
		if (LOWORD(wParam) == ID_hListBoxWriteDriver)
		{
			if (HIWORD(wParam) == LBN_DBLCLK)
			{
				int uSelectedItem, nSize;
				uSelectedItem = (int)SendMessage(hListBoxWriteDriver,
					LB_GETCURSEL, 0, 0L);

				if (uSelectedItem != LB_ERR)
					SendMessage(hListBoxWriteDriver, LB_GETTEXT,
						uSelectedItem, (LPARAM)Buffer);
			}
		}
		break;
	case WM_CREATE:
		hPath = CreateWindow(L"Edit", L"", WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER, 40, 275, 610, 20, hwnd, 0, hInst, 0);
		hSearchFiles = CreateWindow(L"Edit", L"", WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER | ES_CENTER, 40, 250, 150, 20, hwnd, 0, hInst, 0);
		hPathSearch = CreateWindow(L"Edit", L"", WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER, 550, 200, 100, 20, hwnd, 0, hInst, 0);
		hSignatureEdit = CreateWindowA("Edit", "", WS_CHILD | WS_VISIBLE | ES_READONLY | WS_BORDER | ES_CENTER, 200, 250, 280, 20, hwnd, 0, hInst, 0);
		hButtonDiscriptors = CreateWindow(L"Button", L". . .", WS_CHILD | WS_VISIBLE | WS_BORDER, 550, 160, 100, 30, hwnd, (HMENU)ID_hButtonDiscriptors, hInst, 0);
		hButtonScanStart = CreateWindow(L"Button", L"Сканировать", WS_CHILD | WS_VISIBLE | WS_BORDER, 550, 40, 100, 50, hwnd, (HMENU)ID_hButtonScanStart, hInst, 0);
		hButtonStopSearch = CreateWindow(L"Button", L"Остановить", WS_CHILD | WS_VISIBLE | WS_BORDER, 550, 100, 100, 50, hwnd, (HMENU)ID_hButtonStopSearch, hInst, 0);
		hButtonDeleteInfectedFile = CreateWindow(L"Button", L"Удалить файлы", WS_CHILD | WS_VISIBLE | WS_BORDER, 490, 230, 160, 40, hwnd, (HMENU)ID_hButtonDeleteInfectedFile, hInst, 0);
		hListBoxWriteDriver = CreateWindow(L"listBox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT, 40, 40, 500, 80, hwnd, (HMENU)ID_hListBoxWriteDriver, hInst, 0);
		hInfectedFilesListBox = CreateWindow(L"listBox", NULL, WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_WANTKEYBOARDINPUT | WS_TABSTOP | WS_HSCROLL | LBS_NOINTEGRALHEIGHT, 40, 130, 500, 90, hwnd, (HMENU)ID_hInfectedFilesListBox, hInst, 0);

		SendMessage(hListBoxWriteDriver, WM_SETREDRAW, FALSE, 0L);
		SendMessage(hListBoxWriteDriver, LB_ADDSTRING, 0, (LPARAM)L"C:\\");
		SendMessage(hListBoxWriteDriver, LB_ADDSTRING, 0, (LPARAM)L"D:\\");
		SendMessage(hListBoxWriteDriver, LB_ADDSTRING, 0, (LPARAM)L"F:\\");
		SendMessage(hListBoxWriteDriver, LB_GETTEXT, 0, (LPARAM)Buffer);
		SendMessage(hListBoxWriteDriver, WM_SETREDRAW, TRUE, 0L);
		SendMessage(GetDlgItem(hwnd, ID_hInfectedFilesListBox), LB_SETHORIZONTALEXTENT, 1000, 0);

		li.QuadPart = 0;
		olf.Offset = li.LowPart;
		olf.OffsetHigh = li.HighPart;
		ReadFile(hSignatureFiles, (BYTE*)VIRUS_SIGNATURE, _SIZE_SIGNATURE_VIRUS, &iNumberRead, &olf);
		olf.Offset += iNumberRead;

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, 10, 320, 674, 10);
		TextOut(hdc, 40, 20, L"Выбрать диск :", 15);
		TextOut(hdc, 40, 110, L"Зараженные файлы :", 19);
		TextOut(hdc, 40, 230, L"Файлов проверенно :", 20);
		TextOut(hdc, 290, 230, L"По сигнатуре :", 15);
		SetWindowTextA(hSignatureEdit, (char*)VIRUS_SIGNATURE);
		EndPaint(hwnd, &ps);
		break;
	case WM_TIMER:
		_itow(indexFile, Buff, 10);
		if (flag)
		{
			SetWindowText(hSearchFiles, Buff);
			SetWindowText(hPath, STR);
		}
		//GetWindowTextA(hSignatureEdit, (char*)VIRUS_SIGNATURE, 128);
		SetWindowText(hPathSearch, Buffer);
		if (wcsnlen_s(InfectedFile, _SIZE_) != NULL)
		{
			SendMessage(hInfectedFilesListBox, LB_ADDSTRING, 0, (LPARAM)InfectedFile);
			LPWSTR BUF = (LPWSTR)calloc(_SIZE_ + 1, sizeof(LPWSTR));
			wcscat_s(BUF, _SIZE_, InfectedFile);
			Pahts.push_back(BUF);
			InfectedFile[0] = L'\0';
		}
		break;
	case WM_SETFOCUS:
		SetFocus(hListBoxWriteDriver);
	case WM_DESTROY:
		break;
	case WM_CLOSE:
		if (flag)
		{
			TerminateThread(thr.native_handle(), NULL);
			thr.detach();
		}
		safos.Free();
		exit(NULL);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		break;
	}
	return 0;
}
