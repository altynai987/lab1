#include "Manager.h"
#include "Logger.h"
#include "Input.h"
#include <iostream>
#include <fstream>
#define NOMINMAX
#include <windows.h>

class redirect_output_wrapper {
private:
    std::ostream& stream;
    std::streambuf* const old_buf;

public:
    redirect_output_wrapper(std::ostream& src)
        : old_buf(src.rdbuf()), stream(src) {
    }

    ~redirect_output_wrapper() {
        stream.rdbuf(old_buf);
    }

    void redirect(std::ostream& dest) {
        stream.rdbuf(dest.rdbuf());
    }
};

void showMenu() {
    std::cout << "\n===== МЕНЮ =====" << std::endl;
    std::cout << "1. Добавить (трубу / КС)" << std::endl;
    std::cout << "2. Просмотр всех объектов" << std::endl;
    std::cout << "3. Редактировать / удалить / поиск" << std::endl;
    std::cout << "4. Пакетное редактирование труб" << std::endl;
    std::cout << "5. Сохранить" << std::endl;
    std::cout << "6. Загрузить" << std::endl;
    std::cout << "7. Восстановить данные из файла лога" << std::endl;
    std::cout << "0. Выход" << std::endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    // Создаём логгер и открываем файл с уникальным именем
    Logger logger;
    logger.open("log");

    // Перенаправляем cerr в лог-файл
    redirect_output_wrapper cerr_out(std::cerr);
    if (logger.isOpen()) {
        cerr_out.redirect(logger.getStream());
    }

    Manager manager;

    while (true) {
        showMenu();
        int choice = getInt(MENU_PROMPT, 0, 7);
        std::cerr << "Выбран пункт меню: " << choice << std::endl;

        switch (choice) {
        case 1: manager.addObject(); break;
        case 2: manager.viewAll(); break;
        case 3: manager.manageObjects(); break;
        case 4: manager.batchEditPipes(); break;
        case 5: manager.saveData(); break;
        case 6: manager.loadData(); break;
        case 7: manager.restoreFromLog(); break;
        case 0:
            std::cerr << "Программа завершена" << std::endl;
            std::cout << "Выход" << std::endl;
            return 0;
        }
    }
}