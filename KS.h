#ifndef KS_H
#define KS_H

#include <string>
#include <iostream>
#include <fstream>

class KS {
private:
    int id = 0;
    std::string name = "";
    int totalShops = 0;
    int workingShops = 0;
    int stationClass = 0;

public:
    KS();
    KS(int id);

    int getId() const;
    const std::string& getName() const;
    int getTotalShops() const;
    int getWorkingShops() const;
    int getStationClass() const;
    double getUnusedPercent() const;

    bool startShop();   
    bool stopShop();    

    bool save(std::ofstream& fout) const;
    bool load(std::ifstream& fin);

    bool matchesName(const std::string& query) const;
    bool matchesUnusedPercent(double minPercent) const;

    friend std::istream& operator>>(std::istream& in, KS& ks);
    friend std::ostream& operator<<(std::ostream& out, const KS& ks);
};

#endif