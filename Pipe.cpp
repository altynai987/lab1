#include "Pipe.h"
#include "Input.h"

Pipe::Pipe() : id(0), name(""), length(0), diameter(0), inRepair(false) {}

Pipe::Pipe(int id) : id(id), name(""), length(0), diameter(0), inRepair(false) {}

int Pipe::getId() const { return id; }
const std::string& Pipe::getName() const { return name; }
double Pipe::getLength() const { return length; }
int Pipe::getDiameter() const { return diameter; }
bool Pipe::getInRepair() const { return inRepair; }

void Pipe::toggleRepair() {
    inRepair = !inRepair;
}

std::istream& operator>>(std::istream& in, Pipe& p) {
    p.name = getNonEmptyString("Введите название трубы: ", in);
    p.length = getPositiveDouble("Введите длину трубы (км): ", in);
    p.diameter = getInt("Введите диаметр трубы (мм): ", 1, INT_MAX, in);
    p.inRepair = false;
    return in;
}

std::ostream& operator<<(std::ostream& out, const Pipe& p) {
    out << "\n--- Труба [ID=" << p.id << "] ---" << std::endl;
    out << "Название: " << p.name << std::endl;
    out << "Длина: " << p.length << " км" << std::endl;
    out << "Диаметр: " << p.diameter << " мм" << std::endl;
    out << "В ремонте: " << (p.inRepair ? "Да" : "Нет") << std::endl;
    return out;
}

bool Pipe::save(std::ofstream& fout) const {
    fout << id << "\n" << name << "\n" << length << "\n" << diameter << "\n"
        << (inRepair ? 1 : 0) << "\n";
    return !fout.fail();
}

bool Pipe::load(std::ifstream& fin) {
    int repairInt;
    fin >> id;
    fin.ignore(10000, '\n');
    std::getline(fin, name);
    fin >> length >> diameter >> repairInt;
    inRepair = (repairInt == 1);
    return !fin.fail();
}

bool Pipe::matchesName(const std::string& query) const {
    return name.find(query) != std::string::npos;
}

bool Pipe::matchesRepair(bool repairStatus) const {
    return inRepair == repairStatus;
}