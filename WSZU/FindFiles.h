#ifndef H_FindFiles
#define H_FindFiles

#include <Windows.h>
#include <ios>
#include <vector>
class FindFiles
{ //����������� ����� ������.
protected:
    std::vector<HANDLE> vSearchs; //������� ������� ������ � ������ �����
    std::wstring szTempl;
    bool f = false;
    bool f1 = false;
public:
    std::wstring szCurPath; //���� �� �������� ����� 
    WIN32_FIND_DATAW Info; //���������� ������ � �����.
    FindFiles(const std::wstring& szPatch); //�������� ���� ������ (��� ���������� �����������: "C:/Docs")
    DWORD i = 0;
    ~FindFiles();
    bool Next(); //����� ��������� �����, ��������� Info � szCurPath. ���� ���������� 0 ����� ��������.
};
#endif

