#include <windows.h>
#include <stdio.h>
#include <tchar.h>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // STARTUPINFO ve PROCESS_INFORMATION yapılarını sıfırla
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Çalıştırılacak komut: Dosyanın içeriğini yazdır
    TCHAR command[] = _T("cmd.exe /C type dosya.txt");

    printf("Parent process basladi. Child process olusturuluyor...\n");

    // Child process oluştur
    if (!CreateProcess(
            NULL,       // Uygulama adı
            command,    // Komut
            NULL,       // Process güvenlik tanımlayıcı
            NULL,       // Thread güvenlik tanımlayıcı
            FALSE,      // Handle'ların devredilmesi
            0,          // Bayraklar
            NULL,       // Ortam değişkenleri
            NULL,       // Çalışma dizini
            &si,        // Başlangıç bilgisi
            &pi))       // Process bilgisi
    {
        printf("Child process olusturulurken hata olustu (%d).\n", GetLastError());
        return 1;
    }

    printf("Child process baslatildi. PID: %lu\n", pi.dwProcessId);

    // Child process'in tamamlanmasını bekle
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Çıkış durumunu kontrol et
    DWORD exitCode;
    if (GetExitCodeProcess(pi.hProcess, &exitCode))
    {
        if (exitCode == 0)
        {
            printf("Child process basariyla tamamlandi.\n");
        }
        else
        {
            printf("Child process hata ile tamamlandi. Cikis kodu: %lu\n", exitCode);
        }
    }
    else
    {
        printf("Cikis kodu alinamadi (%d).\n", GetLastError());
    }

    // Handle'ları serbest bırak
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    printf("Parent process tamamlandi.\n");
    return 0;
}