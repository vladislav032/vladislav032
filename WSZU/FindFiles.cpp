#include "FindFiles.h"

FindFiles::FindFiles(const std::wstring& szPatch) {
    szTempl = L"*.*"; //шаблон, чтобы находить и папки
    vSearchs.push_back(0);
    szCurPath = szPatch;
}
FindFiles::~FindFiles() {
    for (std::vector<HANDLE>::iterator i = vSearchs.begin(); i != vSearchs.end(); ++i)
        if (*i) FindClose(*i);
}
bool FindFiles::Next() {
    while (1) {
        if (vSearchs.back() == 0) { //проводим первый поиск
            if (szCurPath == L"") return 0;
            vSearchs.back() = FindFirstFileW((szCurPath + L"\\" + szTempl).c_str(), &Info);
            if (vSearchs.back() == INVALID_HANDLE_VALUE) return 0;
        }
        else if (!FindNextFileW(vSearchs.back(), &Info)) { //или последующий
            if (vSearchs.size() < 2) return 0; //не найдено в главной папке - конец поиска
            FindClose(vSearchs.back()); //не найденно в подпапке продолжаем поиск в родителе
            vSearchs.pop_back();
            szCurPath = szCurPath.substr(0, szCurPath.rfind(L'\\'));
            continue;
        }
        if (wcscmp(Info.cFileName, L".") == 0 || wcscmp(Info.cFileName, L"..") == 0) continue; //пропуск ссылок на себя и родителя
        if (Info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { //нашли папку, заходим в неё
            szCurPath += L"\\" + std::wstring(Info.cFileName);
            vSearchs.push_back(0);
            continue;
        }
        else
            i++;
        return 1; //найден файл (данные были записаны сразу)
    }
    return 0;
}