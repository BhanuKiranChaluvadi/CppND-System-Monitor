#include <string>
#include "ProcessParser.h"

using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
private:
    string pid;
    string user;
    string cmd;
    string cpu;
    string mem;
    string upTime;

public:

    Process() {}
    Process(string pid){
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        this->cmd = ProcessParser::getCmd(pid);
	    this->cpu = ProcessParser::getVmSize(pid);
        this->mem = ProcessParser::getCpuPercent(pid);
        this->upTime = ProcessParser::getProcUpTime(pid);
    }
    void setPid(int pid);
    string getPid()const;
    string getUser()const;
    string getCmd()const;
    int getCpu()const;
    int getMem()const;
    string getUpTime()const;
    string getProcess();
};

void Process::setPid(int pid){
    this->pid = pid;
}
string Process::getPid()const {
    return this->pid;
}
string Process::getUser() const {
    return this->user;
}
string Process::getCmd() const {
    return this->cmd;
}
int Process::getCpu() const {  
    return stoi(this->cpu);
}
int Process::getMem() const {
    return stoi(this->mem);
}
string Process::getUpTime() const {
    return this->upTime;
}

string Process::getProcess(){
    if(!ProcessParser::isPidExisting(this->pid)) {
        return "";
    }
        
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    string value = this->pid 
                + "   " 
                + this->user 
                + "   "
                + this->cpu.substr(0, 5) 
                + "   "
                + this->mem.substr(0,5) 
                + "   "
                + this->upTime.substr(0,5) 
                + "   "
                + this->cmd.substr(0, 30)
                + "...";

    return value;
}