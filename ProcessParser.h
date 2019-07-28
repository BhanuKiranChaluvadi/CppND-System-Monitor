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
    static float ProcessParser::getSysActiveCpuTime(vector<string> values);
    static float ProcessParser::getSysIdleCpuTime(vector<string>values);
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

string ProcessParser::getCmd(string pid) {
    
    string path = Path::basePath() + pid + Path::cmdPath();
    std::ifstream stream;
    Util::getStream(path, stream);
    string line;
    getline(stream, line);
    return line;
}

vector<string> ProcessParser::getPidList() {
    // TODO: fiter non pid file in the proc folder.
    vector <string> pidList;
    DIR *pdir = nullptr;
    pdir = opendir(Path::basePath().c_str());
    if(pdir == nullptr) {
        throw std::runtime_error(std::strerror(errno));
    }

    struct dirent *dirp = nullptr;
    while (dirp = readdir(pdir)) {
        // is this a directory
        if(dirp->d_type != DT_DIR)
            continue;
        // is every character of a name is digit ?
        if(all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](const char& c){return isdigit(c);}))
            pidList.push_back(dirp->d_name );  
    }

    // finally, let's close the directory
    if(closedir(pdir))
        throw std::runtime_error(std::strerror(errno));
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
    string name = "Uid";
    string result = "";
    // path
    string path = Path::basePath() + pid + Path::statusPath();
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    while(getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result = values[1];            
            break;
        }
    }
    stream.close();
    string user;
    string users_path = "/etc/passwd" ;
    Util::getStream(users_path, stream);
    while(getline(stream, line)) {
        if (line.find(result) != std::string::npos) {
            user = line.substr(0, line.find(":"));
            return user;
        }   
    }
    return "";
}


vector<string> ProcessParser::getSysCpuPercent(string coreNumber = "") {
    string path = Path::basePath() + "/" + Path::statPath();
    string name = "cpu" + coreNumber;
    // stream data
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    while(getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);          
            return values;
        }
    }
    return (vector<string>());
}

float ProcessParser::getSysActiveCpuTime(vector<string> values)
{
    return (stof(values[S_USER]) +
            stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) +
            stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL]) +
            stof(values[S_GUEST]) +
            stof(values[S_GUEST_NICE]));
}

float ProcessParser::getSysIdleCpuTime(vector<string>values)
{
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}

float ProcessParser::getSysRamPercent() {
    string path = Path::basePath() + "/" + Path::memInfoPath();
    string name1 = "MemAvailable";
    string name2 = "MemFree";
    string name3 = "Buffers";

    float totalMem = 0;
    float freeMem = 0;
    float buffers = 0;

    // stream data
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    while(getline(stream, line)) {
        if (totalMem != 0 && freeMem != 0)
            break;
        if (line.compare(0, name1.size(), name1) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end); 
            totalMem = stof(values[1]);
        }
        else if (line.compare(0, name2.size(), name2) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            freeMem = stof(values[1]);
        }
        else if (line.compare(0, name3.size(), name3) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end); 
            buffers = stof(values[1]);
        }
    }
    //calculating usage:
    return float(100.0 * (1-(freeMem/(totalMem-buffers))));
}

string ProcessParser::getSysKernelVersion() {
    string path = Path::basePath() + "/" + Path::versionPath();
    string name = "Linux version ";
    string result = "";
    // stream data
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    while(getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result = values[2];
            return result;
        }
    }
    return result;
}

int ProcessParser::getNumberOfCores() {
    string name = "cpu cores";
    string result = "";
    // path
    string path = Path::basePath() +"cpuinfo";
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    while(getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result = values[2];            
            return stoi(result);
        }
    }
    return 0;
}

int ProcessParser::getTotalThreads() {
    vector<string> pidList = ProcessParser::getPidList();
    string name = "Threads";
    int result = 0;
    for_each(begin(pidList), end(pidList), [&result, name](const string  &pid){
        string path = Path::basePath() + pid + Path::statusPath();
        ifstream stream;
        Util::getStream(path, stream);
        string line;
        while(getline(stream, line)) {
            if (line.compare(0, name.size(), name) == 0) {
                istringstream buf(line);
                istream_iterator<string> beg(buf), end;
                vector<string> values(beg, end);
                result += stoi(values[1]);
                break;          
            }
        }
    });
    return result;
}
int ProcessParser::getTotalNumberOfProcesses() {
    string path = Path::basePath() + "/" + Path::statPath();
    string name = "processes";
    int result = 0;
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    while(getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result = stoi(values[1]);
            return result;
        }
    }
    return result;
}
int ProcessParser::getNumberOfRunningProcesses() {
    string path = Path::basePath() + "/" + Path::statPath();
    string name = "procs_running";
    int result = 0;
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    while(getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            result += stoi(values[1]);
            break;
        }
    }
    return result;
}

string ProcessParser::getOSName(){
    string path = "/etc/os-release";
    string name = "PRETTY_NAME=";
    string result = "";
    ifstream stream;
    Util::getStream(path, stream);
    string line;
    while(getline(stream, line)) {
        if (line.compare(0, name.size(), name) == 0) {
            // remove quotes
            result = line.substr (name.size()+1);
            result.erase(std::remove(begin(result), end(result), '"'), result.end());
            return result;
        }
    }
    return "";
}

string ProcessParser::PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2){
    float activeTime =  ProcessParser::getSysActiveCpuTime(values2) - ProcessParser::getSysActiveCpuTime(values1);
    float idleTime =  ProcessParser::getSysIdleCpuTime(values2) - ProcessParser::getSysIdleCpuTime(values1);
    float totalTime = activeTime + idleTime;
    float result = 100.0*(activeTime / totalTime);
    return to_string(result);
}

bool ProcessParser::isPidExisting(string pid){

}
