#include <windows.h>
#include <iostream>
#include <string>

void sendRequest(const std::string& request) {
    const char* pipeName = R"(\\.\pipe\MyNamedPipe)";
    HANDLE hPipe = CreateFileA(
        pipeName,
        GENERIC_READ | GENERIC_WRITE,
        0, nullptr, OPEN_EXISTING, 0, nullptr);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Не удалось подключиться к каналу. Код ошибки: " << GetLastError() << std::endl;
        return;
    }

    DWORD bytesWritten;
    if (!WriteFile(hPipe, request.c_str(), request.size(), &bytesWritten, nullptr)) {
        std::cerr << "Не удалось отправить запрос. Код ошибки: " << GetLastError() << std::endl;
    }

    CloseHandle(hPipe);
}

int main() {
    while (true) {
        std::cout << "Выберите операцию: 1 - Чтение, 2 - Запись, 3 - Выход: ";
        int choice;
        std::cin >> choice;
        if (choice == 3) {
            sendRequest("exit");
            break;
        }

        std::string request;
        if (choice == 1) {
            std::cout << "Введите ID для чтения: ";
            int id;
            std::cin >> id;
            request = "read " + std::to_string(id);
        }
        else if (choice == 2) {
            std::cout << "Введите ID для модификации: ";
            int id;
            std::cin >> id;
            request = "write " + std::to_string(id);
        }
        sendRequest(request);
    }
    return 0;
}