#ifndef H_FindFiles
#define H_FindFiles

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ios>
#include <vector>

class FindFiles
{ //Рекурсивный поиск файлов.
protected:
    std::vector<HANDLE> vSearchs; //текущий уровень поиска в дереве папок
    std::wstring szTempl;
    bool f = false;
    bool f1 = false;
public:
    std::wstring szCurPath; //путь до текущего файла 
    WIN32_FIND_DATAW Info; //полученные данные о файле.
    FindFiles(const std::wstring& szPatch); //указание пути поиска (без последнего разделителя: "C:/Docs")
    DWORD i = 0;
    ~FindFiles();
    bool Next(); //Найти следующий файла, заполнить Info и szCurPath. Если возвращает 0 поиск закончен.
};
#endif

