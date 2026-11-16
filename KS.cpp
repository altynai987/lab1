#include "KS.h"
#include "Input.h"

KS::KS() : id(0), name(""), totalShops(0), workingShops(0), stationClass(0) {}

KS::KS(int id) : id(id), name(""), totalShops(0), workingShops(0), stationClass(0) {}

int KS::getId() const { return id; }
const std::string& KS::getName() const { return name; }
int KS::getTotalShops() const { return totalShops; }
int KS::getWorkingShops() const { return workingShops; }
int KS::getStationClass() const { return stationClass; }

double KS::getUnusedPercent() const {
    if (totalShops == 0) return 0;
    return (double)(totalShops - workingShops) / totalShops * 100.0;
}

bool KS::startShop() {
    if (workingShops >= totalShops) return false;
    workingShops++;
    return true;
}

bool KS::stopShop() {
    if (workingShops <= 0) return false;
    workingShops--;
    return true;
}

std::istream& operator>>(std::istream& in, KS& ks) {
    ks.name = getNonEmptyString("Введите название КС: ", in);
    ks.totalShops = getInt("Введите всего цехов: ", 1, INT_MAX, in);
    ks.workingShops = getInt("Введите цехов в работе (0.." + std::to_string(ks.totalShops) + "): ",
        0, ks.totalShops, in);
    ks.stationClass = getInt("Введите класс станции: ", 1, INT_MAX, in);
    return in;
}

std::ostream& operator<<(std::ostream& out, const KS& ks) {
    out << "\n--- КС [ID=" << ks.id << "] ---" << std::endl;
    out << "Название: " << ks.name << std::endl;
    out << "Всего цехов: " << ks.totalShops << std::endl;
    out << "В работе: " << ks.workingShops << std::endl;
    out << "Незадействовано: " << ks.getUnusedPercent() << "%" << std::endl;
    out << "Класс: " << ks.stationClass << std::endl;
    return out;
}

bool KS::save(std::ofstream& fout) const {
    fout << id << "\n" << name << "\n" << totalShops << "\n" << workingShops << "\n"
        << stationClass << "\n";
    return !fout.fail();
}

bool KS::load(std::ifstream& fin) {
    fin >> id;
    fin.ignore(10000, '\n');
    std::getline(fin, name);
    fin >> totalShops >> workingShops >> stationClass;
    return !fin.fail();
}

bool KS::matchesName(const std::string& query) const {
    return name.find(query) != std::string::npos;
}

bool KS::matchesUnusedPercent(double minPercent) const {
    return getUnusedPercent() >= minPercent;
}