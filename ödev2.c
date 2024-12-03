#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    TCHAR command[] = TEXT("C:\\Windows\\System32\\cmd.exe /C type dosya.txt");

    // Child process oluşturulması
    if (!CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        printf("Process baslatilamadi. Hata kodu: %d \n", GetLastError());
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE); // Parent process'in child process'inin bitmesini beklemesi

    // Child process'in çıkış durumunun kontrol edilmesi
    DWORD exitCode;
    if (GetExitCodeProcess(pi.hProcess, &exitCode))
    {
        if (exitCode == 0)
        {
            printf("Child process duzgun sekilde sonlandi.\n");
        }
        else
        {
            printf("Child process bir hata ile sonlandi. cikis kodu: %lu\n", exitCode);
        }
    }
    else
    {
        printf("Exit kodu alinamadi. Hata kodu: %d \n", GetLastError()); // exit code alınamazsa
    }

    // Parent process child handle'larını kapatıyor
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Dosya açma ve hata kontrolü
    FILE *file = fopen("dosya.txt", "r");

    if (file == NULL)
    {
        // Dosya bulunamadıysa abort() kullanarak çıkıyoruz
        printf("\nDosya bulunamadi, abort() kullaniliyor!!!\n");
        abort(); // Hata durumunda child process zorla sonlanır
    }
    else
    {
        // Dosya başarıyla açıldıysa düzgün çıkış yapıyoruz
        printf("\nDosya basariyla acildi, exit(0) kullaniliyor!!!\n");
        fclose(file);
        exit(0); // Child process düzgün şekilde sonlanır
    }

    return 0;
}