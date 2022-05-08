#include "Window.h"

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	WI::InicializedWcex();
	WI::create_window(L"Class", L"WSZU", 700, 370);
	return 0;
}