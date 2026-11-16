#include "Manager.h"
#include "Input.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

template <class T>
int addNew(unordered_map<int, T>& items, int& nextId, const string& what) {
    T obj(nextId);
    cin >> obj;
    items.emplace(nextId, obj);
    cerr << "Добавлена " << what << " ID=" << nextId << endl;
    return nextId++;
}

template <class T>
typename unordered_map<int, T>::iterator findObject(unordered_map<int, T>& items,
    const string& prompt, const string& what) {
    int id = getInt(prompt, 1);
    auto it = items.find(id);
    if (it == items.end()) {
        cout << "Не найдена " << what << " с ID=" << id << endl;
        cerr << "Ошибка: не найдена " << what << " с ID=" << id << endl;
    }
    return it;
}

template <class T>
void eraseObject(unordered_map<int, T>& items, const string& prompt, const string& what) {
    auto it = findObject(items, prompt, what);
    if (it == items.end()) return;
    cerr << "Удалена " << what << " ID=" << it->first << endl;
    items.erase(it);
    cout << "Удалена " << what << endl;
}

template <class T>
void printAll(const unordered_map<int, T>& items, const string& emptyMsg) {
    if (items.empty()) cout << emptyMsg << endl;
    for (const auto& item : items) cout << item.second;
}

template <class T>
void printByIds(const unordered_map<int, T>& items, const vector<int>& ids) {
    if (ids.empty()) cout << "Ничего не найдено" << endl;
    for (int id : ids) cout << items.at(id);
}

template <class T, class Pred>
vector<int> findIds(const unordered_map<int, T>& items, Pred pred) {
    vector<int> ids;
    for (const auto& item : items)
        if (pred(item.second)) ids.push_back(item.first);
    return ids;
}

template <class T>
vector<int> findByName(const unordered_map<int, T>& items) {
    string query = getNonEmptyString("Введите часть названия: ");
    return findIds(items, [&query](const T& obj) { return obj.matchesName(query); });
}

template <class T>
int nextFreeId(const unordered_map<int, T>& items) {
    int next = 1;
    for (const auto& item : items) next = max(next, item.first + 1);
    return next;
}

template <class T>
void writeObjects(ofstream& fout, const unordered_map<int, T>& items) {
    fout << items.size() << "\n";
    for (const auto& item : items) item.second.save(fout);
}

template <class T>
void readObjects(ifstream& fin, unordered_map<int, T>& items) {
    items.clear();
    int count;
    fin >> count;
    fin.ignore(10000, '\n');
    for (int i = 0; i < count && fin; i++) {
        T obj;
        obj.load(fin);
        items[obj.getId()] = obj;
    }
}

Manager::Manager() : nextPipeId(1), nextKSId(1) {
    cerr << "Программа запущена" << endl;
}

int Manager::chooseType(const string& action) const {
    cout << action << ": 1 - труба, 2 - КС, 0 - назад" << endl;
    return getInt("Выбор: ", 0, 2);
}

void Manager::addObject() {
    int type = chooseType("Добавить");
    if (type == 1) {
        int id = addNew(pipes, nextPipeId, "труба");
        cout << "Труба добавлена с ID=" << id << endl;
    }
    else if (type == 2) {
        int id = addNew(stations, nextKSId, "КС");
        cout << "КС добавлена с ID=" << id << endl;
    }
}

void Manager::viewAll() const {
    printAll(pipes, "Труб нет");
    printAll(stations, "КС нет");
}

void Manager::manageObjects() {
    cout << "Действие: 1 - редактировать, 2 - удалить, 3 - поиск, 0 - назад" << endl;
    int action = getInt("Выбор: ", 0, 3);
    if (action == 1) editObject();
    else if (action == 2) deleteObject();
    else if (action == 3) searchObjects();
}

void Manager::editObject() {
    int type = chooseType("Редактировать");
    if (type == 1) editPipe();
    else if (type == 2) editKS();
}

void Manager::editPipe() {
    auto it = findObject(pipes, "Введите ID трубы: ", "труба");
    if (it == pipes.end()) return;
    it->second.toggleRepair();
    cout << "Статус ремонта изменён: " << (it->second.getInRepair() ? "в ремонте" : "не в ремонте") << endl;
    cerr << "Изменён статус ремонта трубы ID=" << it->first << endl;
}

void Manager::editKS() {
    auto it = findObject(stations, "Введите ID КС: ", "КС");
    if (it == stations.end()) return;
    KS& ks = it->second;
    int action = getInt("1 - запустить цех, 0 - остановить цех: ", 0, 1);
    bool ok = (action == 1) ? ks.startShop() : ks.stopShop();
    if (ok) cout << "Работает цехов: " << ks.getWorkingShops() << " из " << ks.getTotalShops() << endl;
    else cout << (action == 1 ? "Все цеха уже работают" : "Все цеха уже остановлены") << endl;
    cerr << "Изменена КС ID=" << ks.getId() << ", в работе: " << ks.getWorkingShops() << endl;
}

void Manager::deleteObject() {
    int type = chooseType("Удалить");
    if (type == 1) eraseObject(pipes, "Введите ID трубы: ", "труба");
    else if (type == 2) eraseObject(stations, "Введите ID КС: ", "КС");
}

vector<int> Manager::filterPipes() const {
    cout << "Фильтр: 1 - по названию, 2 - по признаку \"в ремонте\", 0 - назад" << endl;
    int choice = getInt("Выбор: ", 0, 2);
    if (choice == 1) return findByName(pipes);
    if (choice == 2) {
        bool repair = (getInt("В ремонте (1 - да, 0 - нет): ", 0, 1) == 1);
        return findIds(pipes, [repair](const Pipe& p) { return p.matchesRepair(repair); });
    }
    return vector<int>();
}

vector<int> Manager::filterKS() const {
    cout << "Фильтр: 1 - по названию, 2 - по проценту незадействованных цехов, 0 - назад" << endl;
    int choice = getInt("Выбор: ", 0, 2);
    if (choice == 1) return findByName(stations);
    if (choice == 2) {
        double pct = getDouble("Минимальный процент незадействованных цехов (0..100): ", 0, 100);
        return findIds(stations, [pct](const KS& ks) { return ks.matchesUnusedPercent(pct); });
    }
    return vector<int>();
}

void Manager::searchObjects() const {
    int type = chooseType("Поиск");
    if (type == 1) printByIds(pipes, filterPipes());
    else if (type == 2) printByIds(stations, filterKS());
}

vector<int> Manager::selectPipes() const {
    cout << "Выбор труб: 1 - все, 2 - найденные по фильтру, 3 - по выбору (ID), 0 - отмена" << endl;
    int choice = getInt("Выбор: ", 0, 3);
    if (choice == 1) return findIds(pipes, [](const Pipe&) { return true; });
    if (choice == 2) return filterPipes();
    if (choice != 3) return vector<int>();

    vector<int> ids;
    istringstream iss(getString("Введите ID через пробел: "));
    int id;
    while (iss >> id) {
        if (pipes.count(id) == 0) cout << "Не найдена труба с ID=" << id << endl;
        else if (find(ids.begin(), ids.end(), id) == ids.end()) ids.push_back(id);
    }
    return ids;
}

void Manager::batchEditPipes() {
    vector<int> ids = selectPipes();
    if (ids.empty()) {
        cout << "Ничего не выбрано" << endl;
        return;
    }
    cout << "Выбрано труб: " << ids.size() << endl;
    int action = getInt("Что сделать? 1 - переключить статус ремонта, 2 - удалить, 0 - отмена: ", 0, 2);

    for (int id : ids) {
        if (action == 1) pipes.at(id).toggleRepair();
        else if (action == 2) pipes.erase(id);
        if (action != 0) cerr << "Пакетно: труба ID=" << id << (action == 1 ? " - статус ремонта изменён" : " - удалена") << endl;
    }
    if (action != 0) cout << "Обработано труб: " << ids.size() << endl;
}

void Manager::saveData() const {
    string filename = getNonEmptyString("Введите имя файла для сохранения: ");
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Ошибка: не удалось открыть файл " << filename << endl;
        cerr << "Ошибка: не удалось открыть файл " << filename << endl;
        return;
    }
    writeObjects(fout, pipes);
    writeObjects(fout, stations);
    cerr << "Сохранение в файл: " << filename << endl;
    cout << "Данные сохранены в " << filename << endl;
}

void Manager::loadData() {
    string filename = getNonEmptyString("Введите имя файла для загрузки: ");
    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Ошибка: файл " << filename << " не найден" << endl;
        cerr << "Ошибка: файл " << filename << " не найден" << endl;
        return;
    }
    readObjects(fin, pipes);
    readObjects(fin, stations);
    nextPipeId = nextFreeId(pipes);
    nextKSId = nextFreeId(stations);
    cerr << "Загрузка из файла: " << filename << endl;
    cout << "Загружено труб: " << pipes.size() << ", КС: " << stations.size() << endl;
}

void Manager::restoreFromLog() {
    string filename = getNonEmptyString("Введите имя файла лога: ");
    int count = 0;
    if (!loadInputsFromLog(filename, count)) {
        cout << "Ошибка: файл лога не найден или в нём нет записей ввода" << endl;
        cerr << "Ошибка загрузки лога: " << filename << endl;
        return;
    }
    cerr << "Загружен лог: " << filename << ", значений: " << count << endl;
    cout << "Из лога прочитано значений: " << count << ". Записанные действия будут выполнены." << endl;
}