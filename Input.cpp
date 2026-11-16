#include "Input.h"
#include <cstdlib>
#include <deque>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

const string INPUT_MARK = "[ввод] ";     
const string REPLAY_MARK = "[повтор] ";  
const string SEPARATOR = "| ";           
const int MAX_LOADED_LOGS = 50;          
deque<string> replayQueue;               
int loadedLogs = 0;

string getString(const string& prompt, istream& in) {
    cout << prompt;
    string s;
    bool replayed = (&in == &cin && !replayQueue.empty());

    if (replayed) {
        s = replayQueue.front();
        replayQueue.pop_front();
        cout << s << endl;       
    }
    else if (!getline(in, s)) {
        cerr << "Конец ввода, программа завершена" << endl;
        exit(0);
    }
    cerr << (replayed ? REPLAY_MARK : INPUT_MARK) << prompt << SEPARATOR << s << endl;
    return s;
}

string getNonEmptyString(const string& prompt, istream& in) {
    while (true) {
        string s = getString(prompt, in);
        if (!s.empty()) return s;
        cout << "Строка не должна быть пустой" << endl;
    }
}

int getInt(const string& prompt, int minValue, int maxValue, istream& in) {
    while (true) {
        istringstream iss(getString(prompt, in));
        int x;
        char extra;
        if ((iss >> x) && !(iss >> extra) && x >= minValue && x <= maxValue)
            return x;

        cout << "Ошибка ввода. Введите целое число";
        if (maxValue != INT_MAX) cout << " от " << minValue << " до " << maxValue;
        else if (minValue != INT_MIN) cout << " не меньше " << minValue;
        cout << endl;
    }
}

double getDouble(const string& prompt, double minValue, double maxValue, istream& in) {
    while (true) {
        istringstream iss(getString(prompt, in));
        double x;
        char extra;
        if ((iss >> x) && !(iss >> extra) && x >= minValue && x <= maxValue)
            return x;
        cout << "Ошибка ввода. Введите число от " << minValue << " до " << maxValue << endl;
    }
}
double getPositiveDouble(const string& prompt, istream& in) {
    while (true) {
        istringstream iss(getString(prompt, in));
        double x;
        char extra;
        if ((iss >> x) && !(iss >> extra) && x > 0)
            return x;
        cout << "Ошибка ввода. Введите положительное число" << endl;
    }
}
bool loadInputsFromLog(const string& filename, int& count) {
    if (loadedLogs >= MAX_LOADED_LOGS) return false;
    ifstream fin(filename);
    if (!fin.is_open()) return false;

    vector<string> prompts;
    vector<string> values;
    string line;
    while (getline(fin, line)) {
        if (line.compare(0, INPUT_MARK.size(), INPUT_MARK) != 0) continue;   
        size_t sep = line.find(SEPARATOR, INPUT_MARK.size());
        if (sep == string::npos) continue;
        prompts.push_back(line.substr(INPUT_MARK.size(), sep - INPUT_MARK.size()));
        values.push_back(line.substr(sep + SEPARATOR.size()));
    }

    if (!values.empty() && prompts.back() == MENU_PROMPT && values.back() == "0") {
        prompts.pop_back();
        values.pop_back();
    }
    if (values.empty()) return false;

    replayQueue.insert(replayQueue.begin(), values.begin(), values.end());
    loadedLogs++;
    count = (int)values.size();
    return true;
}