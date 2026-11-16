#ifndef PIPE_H
#define PIPE_H

#include <string>
#include <iostream>
#include <fstream>

class Pipe {
private:
    int id = 0;
    std::string name = "";
    double length = 0.0;
    int diameter = 0;
    bool inRepair = false;

public:
    Pipe();
    Pipe(int id);

    int getId() const;
    const std::string& getName() const;
    double getLength() const;
    int getDiameter() const;
    bool getInRepair() const;

    void toggleRepair();
    bool save(std::ofstream& fout) const;
    bool load(std::ifstream& fin);
    bool matchesName(const std::string& query) const;
    bool matchesRepair(bool repairStatus) const;

    friend std::istream& operator>>(std::istream& in, Pipe& p);
    friend std::ostream& operator<<(std::ostream& out, const Pipe& p);
};

#endif