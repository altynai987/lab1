#include <iostream>
#include <string>
#include <fstream>
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

    void input() {
        cout << "Введите название трубы: ";
        name = getString();
        cout << "Введите длину трубы (км): ";
        length = getPositiveDouble();
        cout << "Введите диаметр трубы (мм): ";
        diameter = getPositiveInt();
        inRepair = false;
    }

    void print() const {
        cout << "\nТруба" << endl;
        cout << "Название: " << name << endl;
        cout << "Длина: " << length << " км" << endl;
        cout << "Диаметр: " << diameter << " мм" << endl;
        cout << "В ремонте: " << (inRepair ? "Да" : "Нет") << endl;
    }

    void toggleRepair() {
        inRepair = !inRepair;
        cout << "Статус ремонта изменён: "
            << (inRepair ? "в ремонте" : "не в ремонте") << endl;
    }

    bool save(ofstream& fout) const {
        fout << name << "\n" << length << "\n" << diameter << "\n"
            << (inRepair ? 1 : 0) << "\n";
        return !fout.fail();
    }

    bool load(ifstream& fin) {
        string line;
        if (!getline(fin, name) || name.empty()) return false;
        if (!name.empty() && name.back() == '\r') name.pop_back();
        if (!(fin >> length) || length <=0 ) return false;
        if (!(fin >> diameter) || diameter <= 0) return false;
        string repairStr;
        if (!(fin >> repairStr)) return false;
        if (repairStr != "0" && repairStr != "1") return false;
        inRepair = (repairStr == "1");
        return true;
    }
};

struct KS {
    string name;
    int totalShops;
    int workingShops;
    int stationClass;

    void input() {
        cout << "Введите название КС: ";
        name = getString();
        cout << "Введите всего цехов: ";
        totalShops = getPositiveInt();
        cout << "Введите цехов в работе (0.." << totalShops << "): ";
        do {
            workingShops = getNonNegativeInt();
            if (workingShops > totalShops) {
                cout << "Цехов в работе не может быть больше общего количества ("
                    << totalShops << "). Повторите: ";
            }
        } while (workingShops > totalShops);
        cout << "Введите класс станции: ";
        stationClass = getPositiveInt();
    }

    void print() const {
        cout << "\nКС" << endl;
        cout << "Название: " << name << endl;
        cout << "Всего цехов: " << totalShops << endl;
        cout << "В работе: " << workingShops << endl;
        cout << "Класс: " << stationClass << endl;
    }

    void manageShop() {
        cout << "1 - запустить цех, 0 - остановить цех: ";
        int action = getNonNegativeInt();

        if (action == 1) {
            if (workingShops < totalShops) {
                workingShops++;
                cout << "Цех запущен. Работает: " << workingShops
                    << " из " << totalShops << endl;
            }
            else cout << "Все цеха уже работают" << endl;
        }
        else if (action == 0) {
            if (workingShops > 0) {
                workingShops--;
                cout << "Цех остановлен. Работает: " << workingShops
                    << " из " << totalShops << endl;
            }
            else cout << "Все цеха уже остановлены" << endl;
        }
        else cout << "Неверная команда. Введите 0 или 1" << endl;
    }

    bool save(ofstream& fout) const {
        fout << name << "\n" << totalShops << "\n" << workingShops << "\n"
            << stationClass << "\n";
        return !fout.fail();
    }

    bool load(ifstream& fin) {
        string line;
        if (!getline(fin, name) || name.empty()) return false;
        if (!name.empty() && name.back() == '\r') name.pop_back();
        if (!(fin >> totalShops) || totalShops<=0) return false;
        if (!(fin >> workingShops)) return false;
        if (!(fin >> stationClass)) return false;
        if (workingShops > totalShops) return false;
        if (totalShops < 0 || workingShops < 0 || stationClass <= 0) return false;
        return true;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    Pipe myPipe;
    KS myCS;
    bool pipeExists = false;
    bool ksExists = false;

    while (true) {
        cout << "\nМЕНЮ" << endl;
        cout << "1. Добавить трубу" << endl;
        cout << "2. Добавить КС" << endl;
        cout << "3. Просмотр всех объектов" << endl;
        cout << "4. Редактировать трубу (статус ремонта)" << endl;
        cout << "5. Редактировать КС (запуск/остановка цеха)" << endl;
        cout << "6. Сохранить" << endl;
        cout << "7. Загрузить" << endl;
        cout << "0. Выход" << endl;
        cout << "Введите номер действия: ";

        int choice = getNonNegativeInt();

        switch (choice) {
        case 1:
            myPipe.input();
            pipeExists = true;
            cout << "Труба добавлена" << endl;
            break;

        case 2:
            myCS.input();
            ksExists = true;
            cout << "КС добавлена" << endl;
            break;

        case 3:
            if (pipeExists) myPipe.print();
            else cout << "Труба еще не создана" << endl;
            if (ksExists) myCS.print();
            else cout << "КС еще не создана" << endl;
            break;

        case 4:
            if (pipeExists) myPipe.toggleRepair();
            else cout << "Сначала создайте трубу" << endl;
            break;

        case 5:
            if (ksExists) myCS.manageShop();
            else cout << "Сначала создайте КС" << endl;
            break;

        case 6:
            if (!pipeExists && !ksExists) {
                cout << "Нечего сохранять - объекты не созданы" << endl;
            }
            else {
                ifstream check("data.txt");
                if (!check.is_open()) {
                    cout << "Ошибка: файл не существует, новый не создаем" << endl;
                }
                else {
                    check.close();
                    ofstream fout("data.txt");
                    if (!fout.is_open()) {
                        cout << "Ошибка: не удалось открыть файл для записи" << endl;
                    }
                    else {
                        fout << (pipeExists ? 1 : 0) << "\n";
                        if (pipeExists) myPipe.save(fout);
                        fout << (ksExists ? 1 : 0) << "\n";
                        if (ksExists) myCS.save(fout);
                        fout.close();
                        cout << "Данные сохранены в data.txt" << endl;
                    }
                }
            }
            break;

        case 7: {
            ifstream fin("data.txt");
            if (!fin.is_open()) {
                cout << "Ошибка: файл не найден" << endl;
            }
            else {
                Pipe tempPipe;
                KS tempStation;
                bool tempHasPipe = false;
                bool tempHasStation = false;
                bool loadSuccess = true;

                int pipeCount;
                if (!(fin >> pipeCount)) loadSuccess = false;
                else {
                    fin.ignore(10000, '\n');
                    if (pipeCount == 1) {
                        if (!tempPipe.load(fin)) loadSuccess = false;
                        else tempHasPipe = true;
                    }
                    else if (pipeCount != 0) loadSuccess = false;
                }

                if (loadSuccess) {
                    int stationCount;
                    if (!(fin >> stationCount)) loadSuccess = false;
                    else {
                        fin.ignore(10000, '\n');
                        if (stationCount == 1) {
                            if (!tempStation.load(fin)) loadSuccess = false;
                            else tempHasStation = true;
                        }
                        else if (stationCount != 0) loadSuccess = false;
                    }
                }

                if (loadSuccess) {
                    pipeExists = tempHasPipe;
                    ksExists = tempHasStation;
                    if (tempHasPipe) myPipe = tempPipe;
                    else myPipe = Pipe();
                    if (tempHasStation) myCS = tempStation;
                    else myCS = KS();
                    cout << "Данные загружены из data.txt" << endl;
                }
                else cout << "Ошибка чтения файла" << endl;
            }
            break;
        }

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