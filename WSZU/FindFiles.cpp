#include "FindFiles.h"

FindFiles::FindFiles(const std::wstring& szPatch) {
    szTempl = L"*.*"; //������, ����� �������� � �����
    vSearchs.push_back(0);
    szCurPath = szPatch;
}
FindFiles::~FindFiles() {
    for (std::vector<HANDLE>::iterator i = vSearchs.begin(); i != vSearchs.end(); ++i)
        if (*i) FindClose(*i);
}
bool FindFiles::Next() {
    while (1) {
        if (vSearchs.back() == 0) { //�������� ������ �����
            if (szCurPath == L"") return 0;
            vSearchs.back() = FindFirstFileW((szCurPath + L"\\" + szTempl).c_str(), &Info);
            if (vSearchs.back() == INVALID_HANDLE_VALUE) return 0;
        }
        else if (!FindNextFileW(vSearchs.back(), &Info)) { //��� �����������
            if (vSearchs.size() < 2) return 0; //�� ������� � ������� ����� - ����� ������
            FindClose(vSearchs.back()); //�� �������� � �������� ���������� ����� � ��������
            vSearchs.pop_back();
            szCurPath = szCurPath.substr(0, szCurPath.rfind(L'\\'));
            continue;
        }
        if (wcscmp(Info.cFileName, L".") == 0 || wcscmp(Info.cFileName, L"..") == 0) continue; //������� ������ �� ���� � ��������
        if (Info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { //����� �����, ������� � ��
            szCurPath += L"\\" + std::wstring(Info.cFileName);
            vSearchs.push_back(0);
            continue;
        }
        else
            i++;
        return 1; //������ ���� (������ ���� �������� �����)
    }
    return 0;
}