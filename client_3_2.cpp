#include <cstdio>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <string>
#pragma warning(disable : 4996)
#define KILOBYTE 1024
#define BUFFER_SIZE (25 * KILOBYTE)




using namespace std;
int showMenu() {
	printf("1. - Поместить сообщение в почтовый ящик \n");
	printf("2. - Выход\n");
	int c = 1;
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
	name1 = "\\\\.\\mailslot\\" + name;
	LPCSTR lpstrHash = name1.c_str();
	HANDLE hf = NULL, hsl = NULL;
	hsl = CreateMailslot(lpstrHash, 0, 0, NULL);
	if (hsl != INVALID_HANDLE_VALUE)
	{
		cout << "Mailslot создан" << endl;
	}

		hf = CreateFile(lpstrHash, GENERIC_WRITE,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hf != INVALID_HANDLE_VALUE)
		{
			cout << "Файл создан" << endl;
		}
	
	cout << "Введите имя для семафора:";
	cin >> name;
	LPCSTR lpstrHas = name.c_str();
	HANDLE hSemaphore;
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
	int Error = GetLastError();
	int m;
	char* buf = (char*)calloc(BUFFER_SIZE, sizeof(char));
	DWORD nBytesRead;
	while ((m = showMenu()) != 2) {
		switch (m) {
		case 1: {
			DWORD write;
			DWORD dwwr;
			do
			{
				char name[255];
				if (fgets(name, sizeof name, stdin) == NULL)
					break;
				if (strcmp(name, "\n") == 0)
					break;
				buf = strcat(buf, name);
			} while (1);
			if (WriteFile(hf, buf, strlen(buf), &dwwr, NULL) != 0)
			{
				cout << "Отправлено" << endl;
				ReleaseSemaphore(hSemaphore, 1, NULL);
			}
			else {
				cout << "Ошибка: не получилось отправить письмо!" << endl;
			}
			break;
		}
		}

	}
	free(buf);
	return (EXIT_SUCCESS);
}

