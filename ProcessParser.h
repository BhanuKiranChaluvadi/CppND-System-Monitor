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
#include <cerrno>       // Error handeling library
#include <cstring>
#include <dirent.h>     // basics- directory access - opendir(), readdir(), closedir()
#include <time.h>
#include <unistd.h>     // unix standard header - threads
#include "constants.h"
#include "util.h"


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
    // TODO: open file and read all the lines.
    string path = Path::basePath() + pid + Path::cmdPath();
    std::ifstream stream;
    Util::getStream(path, stream);

    stream.close();

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
    string name = "VmData";
    string vmSize;
    float result;
    // get the stream
    string path = Path::basePath() + pid + Path::statusPath();
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    // read each line 
    while(getline(stream, line))  {
        // compare if name exsists in first fee spots.
        if (line.compare(0, name.size(), name) == 0) {
            // split the line sting into individua strings
            istringstream iss(line);
            vector<string> words(istream_iterator<string>{iss}, 
                                        istream_iterator<string>());
            // DEBUG
            // std::for_each(std::begin(words), std::end(words), [](std::string &word){ std::cout << word<< std::endl;}) ;
            
            // convertion kB --> GB
            result = stof(words[1])/float(1024) ; 
            break;
        }
    }
    vmSize = to_string(result);
    return vmSize;
}

string ProcessParser::getCpuPercent(string pid) {
    // path to file
    string path = Path::basePath() + pid + "/" + Path::statPath();
    // get file and read line.
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    getline(stream, line);
    // split the line based on spaces
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);
    // up time
    float utime = stof(ProcessParser::getProcUpTime(pid));

    float stime = stof(values[14]);
    float cutime = stof(values[15]);
    float cstime = stof(values[16]);
    float starttime = stof(values[21]);

    float uptime = ProcessParser::getSysUpTime();
    
    float freq = sysconf(_SC_CLK_TCK);

    float total_time = utime + stime + cutime + cstime; 
    float seconds = uptime - (starttime/freq);
    float result = 100.0*((total_time)/seconds);

    return to_string(result);
}

long int ProcessParser::getSysUpTime() {
    // path
    string path = Path::basePath() + "/" + Path::upTimePath();
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    getline(stream, line);
    // split the line based on spaces
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);
    return stoi(values[0]);
}

string ProcessParser::getProcUpTime(string pid) {
    // path
    string path = Path::basePath() + pid + "/" + Path::statPath();
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    getline(stream, line);
    // split the line based on spaces
    istringstream buf(line);
    istream_iterator<string> beg(buf), end;
    vector<string> values(beg, end);
    // Using sysconf to get clock ticks of the host machine
    float starttime = stof(values[13]);
    float freq = sysconf(_SC_CLK_TCK);
    float result = starttime / freq ;

    return to_string(result);
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
