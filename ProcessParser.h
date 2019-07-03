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
#include <dirent.h>
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