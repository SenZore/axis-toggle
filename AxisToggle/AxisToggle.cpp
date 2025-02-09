#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <string>

bool IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }

    return isAdmin;
}

bool IsNetshInPath() {
    char buffer[512];
    DWORD result = SearchPathA(NULL, "netsh.exe", NULL, 512, buffer, NULL);
    return result > 0;
}

void AddNetshToPath() {
    std::string path;
    char buffer[512];
    DWORD length = GetEnvironmentVariableA("PATH", buffer, 512);
    if (length > 0 && length < 512) {
        path = buffer;
        if (path.find("C:\\Windows\\System32") == std::string::npos) {
            path += ";C:\\Windows\\System32";
            SetEnvironmentVariableA("PATH", path.c_str());
        }
    }
}

int main()
{
    if (!IsRunningAsAdmin()) {
        std::cout << "Program ini harus dijalankan sebagai administrator.\n";
        system("pause");
        return 1;
    }

    std::cout << "Pastikan Netsh Sudah Termasuk di System Env Anda, Jika Belum, Maka Program ini akan memberi PATH yang dibutuhkan kepada Env Anda.\n";
    std::cout << "Tekan Enter untuk melanjutkan...\n";
    std::cin.get();

    if (!IsNetshInPath()) {
        AddNetshToPath();
        std::cout << "Netsh telah ditambahkan ke PATH.\n";
    }
    else {
        std::cout << "Netsh sudah ada di PATH.\n";
    }

    int choice;
    std::cout << "Axis Unlimited Toggle Created by senz\n";
    std::cout << "Masukkan pilihan Anda (64 untuk Unlimited, 128 untuk Mematikan Unlimited): ";
    std::cin >> choice;

    if (choice == 64) {
        system("C:\\Windows\\System32\\netsh.exe int ipv4 set glob defaultcurhoplimit=64");
        system("C:\\Windows\\System32\\netsh.exe int ipv6 set glob defaultcurhoplimit=64");
        std::cout << "Mode Unlimited Sudah Aktif.\n";
    }
    else if (choice == 128) {
        system("C:\\Windows\\System32\\netsh.exe int ipv4 set glob defaultcurhoplimit=128");
        system("C:\\Windows\\System32\\netsh.exe int ipv6 set glob defaultcurhoplimit=128");
        std::cout << "Mode Unlimited Sudah Di Matikan.\n";
    }
    else {
        std::cout << "Pilihan tidak valid.\n";
    }

    std::cout << "Tekan Enter untuk keluar dari program.\n";
    system("pause");

    return 0;
}