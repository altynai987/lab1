#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
using namespace std;
int getInt() {
    int x;
    while (true) {
        cin >> x;
        if (cin && cin.peek() == '\n') {
            cin.ignore(10000, '\n');
            return x;
        }
        cout << "Ошибка ввода. Введите целое число: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

int getPositiveInt() {
    int x;
    do {
        x = getInt();
        if (x <= 0) cout << "Число должно быть положительным: ";
    } while (x <= 0);
    return x;
}

int getNonNegativeInt() {
    int x;
    do {
        x = getInt();
        if (x < 0) cout << "Число должно быть неотрицательным: ";
    } while (x < 0);
    return x;
}

double getPositiveDouble() {
    double x;
    while (true) {
        cin >> x;
        if (cin && cin.peek() == '\n' && x > 0) {
            cin.ignore(10000, '\n');
            return x;
        }
        cout << "Ошибка ввода. Введите положительное число: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

string getString() {
    string s;
    getline(cin, s);
    if (!s.empty() && s.back() == '\r') {
        s.pop_back();
    }
    return s;
}

struct Pipe {
    string name;
    double length;
    int diameter;
    bool inRepair;
};

struct KS {
    string name;
    int totalShops;
    int workingShops;
    int stationClass;
};

Pipe createPipe() {
    Pipe p;
    cout << "Введите название трубы: ";
    p.name = getString();
    cout << "Введите длину трубы (км): ";
    p.length = getPositiveDouble();
    cout << "Введите диаметр трубы (мм): ";
    p.diameter = getPositiveInt();
    p.inRepair = false;
    return p;
}

KS createKS() {
    KS cs;
    cout << "Введите название КС: ";
    cs.name = getString();
    cout << "Введите всего цехов: ";
    cs.totalShops = getPositiveInt();
    cout << "Введите цехов в работе (0.." << cs.totalShops << "): ";
    do {
        cs.workingShops = getNonNegativeInt();
        if (cs.workingShops > cs.totalShops) {
            cout << "Цехов в работе не может быть больше общего количества ("
                << cs.totalShops << "). Повторите: ";
        }
    } while (cs.workingShops > cs.totalShops);
    cout << "Введите класс станции: ";
    cs.stationClass = getPositiveInt();
    return cs;
}

void printPipe(const Pipe& p) {
    cout << "\nТруба" << endl;
    cout << "Название: " << p.name << endl;
    cout << "Длина: " << p.length << " км" << endl;
    cout << "Диаметр: " << p.diameter << " мм" << endl;
    cout << "В ремонте: " << (p.inRepair ? "Да" : "Нет") << endl;
}

void printKS(const KS& cs) {
    cout << "\nКС" << endl;
    cout << "Название: " << cs.name << endl;
    cout << "Всего цехов: " << cs.totalShops << endl;
    cout << "В работе: " << cs.workingShops << endl;
    cout << "Класс: " << cs.stationClass << endl;
}

void saveData(const Pipe& p, bool pipeExists, const KS& cs, bool ksExists) {
    ofstream fout("data.txt");
    if (!fout.is_open()) {
        cout << "Ошибка: не удалось открыть файл для записи" << endl;
        return;
    }

    if (pipeExists) {
        fout << "PIPE\n";
        fout << p.name << "\n";
        fout << p.length << "\n";
        fout << p.diameter << "\n";
        fout << (p.inRepair ? 1 : 0) << "\n";
    }
    else {
        fout << "NO_PIPE\n";
    }

    if (ksExists) {
        fout << "KS\n";
        fout << cs.name << "\n";
        fout << cs.totalShops << "\n";
        fout << cs.workingShops << "\n";
        fout << cs.stationClass << "\n";
    }
    else {
        fout << "NO_KS\n";
    }

    fout.close();
    if (fout.fail()) {
        cout << "Ошибка при записи в файл" << endl;
    }
    else {
        cout << "Данные сохранены в data.txt" << endl;
    }
}

void loadData(Pipe& p, KS& cs, bool& pipeExists, bool& ksExists) {
    ifstream fin("data.txt");
    if (!fin.is_open()) {
        cout << "Ошибка: файл data.txt не найден" << endl;
        return;
    }

    string line;
    p.name = ""; p.length = 0; p.diameter = 0; p.inRepair = false;
    cs.name = ""; cs.totalShops = 0; cs.workingShops = 0; cs.stationClass = 0;
    pipeExists = false;
    ksExists = false;

    if (!getline(fin, line)) {
        cout << "Ошибка: файл пуст" << endl;
        fin.close();
        return;
    }
    if (!line.empty() && line.back() == '\r') line.pop_back();

    if (line != "PIPE") {
        cout << "Ошибка: некорректный формат файла (труба)" << endl;
        fin.close();
        return;
    }

    if (!getline(fin, p.name)) {
        cout << "Ошибка чтения имени трубы" << endl;
        fin.close();
        return;
    }
    if (!p.name.empty() && p.name.back() == '\r') p.name.pop_back();

    if (!(fin >> p.length)) {
        cout << "Ошибка чтения длины трубы" << endl;
        fin.close();
        return;
    }
    if (!(fin >> p.diameter)) {
        cout << "Ошибка чтения диаметра трубы" << endl;
        fin.close();
        return;
    }
    int repair;
    if (!(fin >> repair)) {
        cout << "Ошибка чтения статуса ремонта" << endl;
        fin.close();
        return;
    }
    p.inRepair = (repair == 1);
    pipeExists = true;
    fin.ignore(10000, '\n'); 
    if (!getline(fin, line)) {
        cout << "Предупреждение: данные КС отсутствуют в файле" << endl;
        fin.close();
        return;
    }
    if (!line.empty() && line.back() == '\r') line.pop_back();

    if (line != "KS") {
        cout << "Ошибка: некорректный формат файла (КС)" << endl;
        fin.close();
        return;
    }

    if (!getline(fin, cs.name)) {
        cout << "Ошибка чтения имени КС" << endl;
        fin.close();
        return;
    }
    if (!cs.name.empty() && cs.name.back() == '\r') cs.name.pop_back();

    if (!(fin >> cs.totalShops)) {
        cout << "Ошибка чтения количества цехов" << endl;
        fin.close();
        return;
    }
    if (!(fin >> cs.workingShops)) {
        cout << "Ошибка чтения количества работающих цехов" << endl;
        fin.close();
        return;
    }
    if (!(fin >> cs.stationClass)) {
        cout << "Ошибка чтения класса станции" << endl;
        fin.close();
        return;
    }
    if (cs.workingShops > cs.totalShops) {
        cout << "Предупреждение: в файле цехов в работе больше, чем всего. Исправлено." << endl;
        cs.workingShops = cs.totalShops;
    }
    if (cs.totalShops < 0 || cs.workingShops < 0 || cs.stationClass < 0) {
        cout << "Ошибка: некорректные данные КС в файле" << endl;
        fin.close();
        return;
    }
    ksExists = true;

    fin.close();
    cout << "Данные загружены из data.txt" << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Pipe myPipe;
    KS myCS;
    bool pipeExists = false;
    bool ksExists = false;

    while (true) {
        cout << "\nМЕНЮ " << endl;
        cout << "1. Добавить трубу" << endl;
        cout << "2. Добавить КС" << endl;
        cout << "3. Просмотр всех объектов" << endl;
        cout << "4. Редактировать трубу (статус ремонта)" << endl;
        cout << "5. Редактировать КС (запуск/останов цеха)" << endl;
        cout << "6. Сохранить" << endl;
        cout << "7. Загрузить" << endl;
        cout << "0. Выход" << endl;
        cout << "Введите номер действия: ";

        int choice = getNonNegativeInt();

        switch (choice) {
        case 1:
            myPipe = createPipe();
            pipeExists = true;
            cout << "Труба добавлена" << endl;
            break;

        case 2:
            myCS = createKS();
            ksExists = true;
            cout << "КС добавлена" << endl;
            break;

        case 3:
            if (pipeExists) printPipe(myPipe);
            else cout << "Труба еще не создана" << endl;
            if (ksExists) printKS(myCS);
            else cout << "КС еще не создана" << endl;
            break;

        case 4:
            if (pipeExists) {
                myPipe.inRepair = !myPipe.inRepair;
                cout << "Статус ремонта изменён: "
                    << (myPipe.inRepair ? "в ремонте" : "не в ремонте") << endl;
            }
            else {
                cout << "Сначала создайте трубу (пункт 1)" << endl;
            }
            break;

        case 5:
            if (ksExists) {
                cout << "1 - запустить цех, 0 - остановить цех: ";
                int action = getNonNegativeInt();
                if (action == 1) {
                    if (myCS.workingShops < myCS.totalShops) {
                        myCS.workingShops++;
                        cout << "Цех запущен. Работает: " << myCS.workingShops
                            << " из " << myCS.totalShops << endl;
                    }
                    else {
                        cout << "Все цеха уже работают" << endl;
                    }
                }
                else if (action == 0) {
                    if (myCS.workingShops > 0) {
                        myCS.workingShops--;
                        cout << "Цех остановлен. Работает: " << myCS.workingShops
                            << " из " << myCS.totalShops << endl;
                    }
                    else {
                        cout << "Все цеха уже остановлены" << endl;
                    }
                }
                else {
                    cout << "Неверная команда. Введите 0 или 1" << endl;
                }
            }
            else {
                cout << "Сначала создайте КС (пункт 2)" << endl;
            }
            break;

        case 6:
            if (!pipeExists && !ksExists) {
                cout << "Нечего сохранять - объекты не созданы" << endl;
            }
            else {
                saveData(myPipe, pipeExists, myCS, ksExists);
            }
            break;

        case 7:
            loadData(myPipe, myCS, pipeExists, ksExists);
            break;

        case 0:
            cout << "Выход из программы" << endl;
            return 0;

        default:
            cout << "Неверный пункт меню" << endl;
            break;
        }
    }

    return 0;
}