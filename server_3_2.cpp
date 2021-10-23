﻿#include <cstdio>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <conio.h>
#include "process.h" 

#pragma warning(disable : 4996)
#define KILOBYTE 1024
#define BUFFER_SIZE (25 * KILOBYTE)

CRITICAL_SECTION cs;
HANDLE hSemaphore;
HANDLE hsl;
using namespace std;
void df(void*) {
	DWORD nBytesRead = 0;
	LONG count = 0;
	char* buf = (char*)calloc(BUFFER_SIZE, sizeof(char));
	while (TRUE) {
		EnterCriticalSection(&cs);
		if (WaitForSingleObject(hSemaphore, 0) == WAIT_OBJECT_0)
			{
				ReadFile(hsl, buf, BUFFER_SIZE, &nBytesRead, NULL);
				cout << "Новое сообщение:" << endl;
				cout << buf << endl;
			}
		LeaveCriticalSection(&cs);

		
	}
}
HANDLE mut;
int showMenu() {
	LeaveCriticalSection(&cs);
	printf("1. - Получить информацию о почтовом ящике\n");
	printf("2. - Получить сообщение из почтового ящика\n");
	printf("3. - Выход\n");
	int c = 1;
	c = getch();
	if (c != 0)
		EnterCriticalSection(&cs);
	cin >> c;
	fscanf(stdin, "%*c");
	return c;
}
int showMenu();

int main()
{
	setlocale(LC_ALL, "Russian");
	string name, name1;
	cout << "Введите имя почтового ящика:";
	cin >> name;
	InitializeCriticalSection(&cs);

	name1 = "\\\\.\\mailslot\\" + name;
	LPCSTR lpstrHash = name1.c_str();
	hsl = CreateMailslot(lpstrHash, 0, 0, NULL);
	if (hsl != INVALID_HANDLE_VALUE)
	{
		cout << "Mailslot создан" << endl;
	}
	else
	{
		hsl = CreateFile(lpstrHash, GENERIC_WRITE,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hsl != INVALID_HANDLE_VALUE)
		{
			cout << "Файл создан" << endl;
		}
	}
	LONG cMax = 2;
	cout << "Введите имя для семафора:";
	cin >> name;
	LPCSTR lpstrHas = name.c_str();
	hSemaphore = CreateSemaphore(

		NULL,	// нет атрибута
		0,	// начальное состояние
		100,	// максимальное состояние
		lpstrHas	// без имени
	);
	if (hSemaphore == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		hSemaphore = OpenSemaphore(NULL, FALSE, lpstrHas);
		if (hSemaphore == NULL)
			printf("OpenSemaphore error: %d\n", GetLastError());
		return 1;
	}

	int m;
	_beginthread(df, 1024, NULL);
	char* buf = (char*)calloc(BUFFER_SIZE, sizeof(char));
	DWORD nBytesRead = 0;
	LONG count = 0;
	DWORD bt = BUFFER_SIZE, bt_for_skip = 0;
	while ((m = showMenu()) != 3) {
		switch (m) {
		case 2: {
			DWORD read;
			if (ReadFile(hsl, buf, nBytesRead, &nBytesRead, NULL) != 0)
			{
				cout << buf << endl;
			}
			else {
				cout << "Ошибка: не получилось прочитать почту!" << endl;
			}
			break;
		}
		case 1: {
			DWORD dwmes = 0, next_msg_size = 0, max_msg_size = 0;
			if (GetMailslotInfo(hsl, &max_msg_size, &next_msg_size, &dwmes, NULL) != 0) {
				cout << "Кол-во сообщений:" << dwmes << endl;
				cout << "Размер последнего сообщения:" << next_msg_size << endl;
				cout << "Наибольший допустимый размер сообщения для данного ящика:" << max_msg_size << endl;
			}
			else {
				cout << "Ошибка: не получилось получить информацию о ящике!" << endl;
			}

			break;
		}
		}

	}
	return (EXIT_SUCCESS);
}

