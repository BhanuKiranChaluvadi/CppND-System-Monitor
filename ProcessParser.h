#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>     // basics- directory access - opendir(), readdir(), closedir()
#include <time.h>
#include <unistd.h>
#include "constants.h"


using namespace std;

class ProcessParser{
private:
    std::ifstream stream;
public:
    static string getCmd(string pid);
    static vector<string> getPidList();
    static std::string getVmSize(string pid);
    static std::string getCpuPercent(string pid);
    static long int getSysUpTime();
    static std::string getProcUpTime(string pid);
    static string getProcUser(string pid);
    static vector<string> getSysCpuPercent(string coreNumber = "");
    static float getSysRamPercent();
    static string getSysKernelVersion();
    static int getNumberOfCores();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static string getOSName();
    static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
    static bool isPidExisting(string pid);
};

// TODO: Define all of the above functions below:
string ProcessParser::getCmd(string pid) {
    
}

vector<string> ProcessParser::getPidList() {
    // TODO: fiter non pid file in the proc folder.
    vector <string> pidList;
    DIR *pdir = nullptr;
    pdir = opendir(Path::basePath().c_str());

    struct dirent *pent = nullptr;

    if(pdir == nullptr) {
        cout << "\nERROR! directory pointer could not be initialised correctly";
        exit (3);
    }
    while (pent = readdir(pdir)) {
        if(pent == nullptr) {
            cout << "\nERROR! pent could not be initialised correctly";
            exit (3);
        }
        
        pidList.push_back(pent->d_name );
         
    }

    // finally, let's close the directory
    closedir (pdir);

    return pidList;
}

string  ProcessParser::getVmSize(string pid){

}

string ProcessParser::getCpuPercent(string pid) {

}

long int ProcessParser::getSysUpTime() {

}

string ProcessParser::getProcUpTime(string pid) {

}

string ProcessParser::getProcUser(string pid) {

}


vector<string> ProcessParser::getSysCpuPercent(string coreNumber = "") {

}

float ProcessParser::getSysRamPercent() {

}

string ProcessParser::getSysKernelVersion() {

}

int ProcessParser::getNumberOfCores() {

}

int ProcessParser::getTotalThreads() {
    
}
int ProcessParser::getTotalNumberOfProcesses() {
    
}
int ProcessParser::getNumberOfRunningProcesses() {

}

string ProcessParser::getOSName(){

}

string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2){

}

bool ProcessParser::isPidExisting(string pid){

}