#ifndef MANAGER_H
#define MANAGER_H

#include "Pipe.h"
#include "KS.h"
#include <string>
#include <unordered_map>
#include <vector>

class Manager {
private:
    std::unordered_map<int, Pipe> pipes;
    std::unordered_map<int, KS> stations;
    int nextPipeId;
    int nextKSId;

    int chooseType(const std::string& action) const;   

    void editObject();
    void editPipe();
    void editKS();
    void deleteObject();
    void searchObjects() const;
    std::vector<int> filterPipes() const;
    std::vector<int> filterKS() const;
    std::vector<int> selectPipes() const;

public:
    Manager();

    void addObject();
    void viewAll() const;
    void manageObjects();   
    void batchEditPipes();
    void saveData() const;
    void loadData();
    void restoreFromLog();
};

#endif