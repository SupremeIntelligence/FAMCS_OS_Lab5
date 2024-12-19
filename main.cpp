#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

struct Employee {
    int num;
    char name[10];
    double hours;
};

std::vector<Employee> employees;

void createFile() {
    int count;
    std::cout << "Введите количество сотрудников: ";
    std::cin >> count;

    for (int i = 0; i < count; ++i) {
        Employee emp;
        std::cout << "Введите ID, имя и часы для сотрудника " << i + 1 << ": ";
        std::cin >> emp.num >> emp.name >> emp.hours;
        employees.push_back(emp);
    }
}

void printEmployees() {
    for (const auto& emp : employees) {
        std::cout << "ID: " << emp.num << ", Имя: " << emp.name << ", Часы: " << emp.hours << std::endl;
    }
}

void handleClientRequests() {
    const char* pipeName = R"(\\.\pipe\MyNamedPipe)";
    HANDLE hPipe = CreateNamedPipeA(
        pipeName,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        1024, 1024, 0, nullptr);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Не удалось создать канал. Код ошибки: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "Ожидание подключения клиентов..." << std::endl;
    if (ConnectNamedPipe(hPipe, nullptr) || GetLastError() == ERROR_PIPE_CONNECTED) {
        std::cout << "Клиент подключен." << std::endl;

        char buffer[1024];
        DWORD bytesRead;
        while (true) {
            if (ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, nullptr)) {
                std::string request(buffer, bytesRead);
                if (request == "exit") {
                    break;
                }

                // Обработка запросов клиента.
                std::cout << "Получен запрос: " << request << std::endl;
            }
        }
    }

    CloseHandle(hPipe);
    std::cout << "Сервер завершил работу." << std::endl;
}

int main() {
    createFile();
    printEmployees();
    handleClientRequests();
    printEmployees();
    return 0;
}
