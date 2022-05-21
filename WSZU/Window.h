#ifndef Window_H
#define Window_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace WI
{
	VOID InicializedWcex();//Инициализация окна
	VOID create_window(LPCWSTR, LPCWSTR, DWORD, DWORD);//Создание окна предварительно инициализировав его
}

#endif

