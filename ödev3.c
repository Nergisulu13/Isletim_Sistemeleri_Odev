#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Dosya olusturma islemi
void dosya_olustur()
{
    FILE *file = fopen("dosya.txt", "w");
    if (file == NULL)
    {
        perror("Dosya olusturulalamadi.");
        exit(1);
    }
    else
    {
        fprintf(file, "child process 1 bu dosyayi olusturdu.\n");
        fclose(file);
        printf("1. child process: Dosya basariyla olusturuldu.\n");
    }
}

// Dosyaya veri ekleme islemi
void dosyaya_veri_ekle()
{
    FILE *file = fopen("dosya.txt", "a");
    if (file == NULL)
    {
        perror("Dosyaya veri eklenemedi");
        exit(1);
    }
    else
    {
        fprintf(file, "bu metni 2.child process dosyaya eklemistir.\n");
        fclose(file);
        printf("2. child process: Dosyaya yeni veri basariyla eklendi.\n");
    }
}

// Dosya icerigini okuma islemi
void dosya_oku()
{
    char buffer[256];
    FILE *file = fopen("dosya.txt", "r");
    if (file == NULL)
    {
        perror("Dosya acilamadi");
        exit(1);
    }
    else
    {
        printf("3. child process: Dosya icerigi asagida gösterilmektedir:\n");
        while (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            printf("%s", buffer);
        }
        fclose(file);
    }
}

int main()
{
    HANDLE hThread1, hThread2, hThread3;
    DWORD threadId1, threadId2, threadId3;

    // 1. child: Dosya olusturma
    hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dosya_olustur, NULL, 0, &threadId1);
    if (hThread1 == NULL)
    {
        printf("1. child islemi olusturulalamadi.\n");
        return 1;
    }

    // 2. child: Dosyaya veri ekleme
    hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dosyaya_veri_ekle, NULL, 0, &threadId2);
    if (hThread2 == NULL)
    {
        printf("2. child islemi olusturulalamadi.\n");
        return 1;
    }

    // 3. child: Dosya icerigini okuma
    hThread3 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)dosya_oku, NULL, 0, &threadId3);
    if (hThread3 == NULL)
    {
        printf("3. child islemi olusturulalamadi.\n");
        return 1;
    }

    // Ana islem, tum child islemlerinin bitmesini bekliyor
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    WaitForSingleObject(hThread3, INFINITE);

    // Handle'lari kapat
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hThread3);

    printf("Ana islem: Tum child islemleri basariyla tamamladi.\n");

    return 0;
}

