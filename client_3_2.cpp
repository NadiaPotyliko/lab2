#include <windows.h>
#include <stdio.h>
#include <conio.h>

int main( int argc, char *argv[] )
{
  HANDLE hMailslot;
  char   szMailslotName[256];
  char   szBuf[512];
  DWORD  cbWritten;

  printf("Mailslot KLIENT\n");

   if(argc > 1)
    sprintf(szMailslotName, "\\\\%s\\mailslot\\$Channel$",
      argv[1]);
  
  else
    strcpy(szMailslotName, "\\\\.\\mailslot\\$Channel$");

   hMailslot = CreateFile(
    szMailslotName, GENERIC_WRITE,
    FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    
   if(hMailslot == INVALID_HANDLE_VALUE)
  {
    fprintf(stdout,"CreateFile: Error %ld\n", 
      GetLastError());
    getch();
    return 0;
  }

   fprintf(stdout,"\nConnected. Enter 'exit' to terminate\n"); 
  
  while(1)
  {
    printf("cmd>");
    gets(szBuf);
    
    if(!WriteFile(hMailslot, szBuf, strlen(szBuf) + 1,
      &cbWritten, NULL))
      break;
    
    if(!strcmp(szBuf, "exit"))
      break;
  }
  CloseHandle(hMailslot);
  return 0;
}
