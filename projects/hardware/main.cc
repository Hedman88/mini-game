//------------------------------------------------------------------
// (Copyright) Harry Hedman 2021
//------------------------------------------------------------------
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <limits.h>
#include <sys/sysinfo.h>
#include <fstream>
#include <sys/utsname.h>
#include <math.h>
#include <sys/wait.h>
#include <thread>
#include <vector>

int iModeFunc(){
    // Processors
    std::fstream cpuinfo;
    std::string line;
    cpuinfo.open("/proc/cpuinfo", std::fstream::in);
    if(cpuinfo.is_open()){
        while(getline(cpuinfo, line)){
            if(line.find("model name") != std::string::npos){
                std::cout << line << std::endl; 
                break;
            }
        }
        cpuinfo.close();
    }else{
        std::cout << "Failed to open file." << std::endl;
    }

    printf("Number of processor cores: %d \n", get_nprocs());

    // Hostname
    char hostname[HOST_NAME_MAX+1];
    gethostname(hostname, HOST_NAME_MAX+1);
    std::cout << "Hostname: " << hostname << std::endl;

    // Hardware platform
    utsname hardware_platform;
    uname(&hardware_platform);
    std::cout << "Hardware platform??: " << hardware_platform.machine << std::endl;

    // Total memory
    struct sysinfo info;
    sysinfo(&info);
    std::cout << "Total RAM: " << info.totalram << std::endl;
    
    return 0;
}

unsigned long long SumCalc(){
    unsigned long long sum = 0;
    for(int i = 0; i < 20000; i++){
        for(int j = 0; j < i; j++){
            sum += sqrt(i*j);
        }
    }
    std::cout << "Sum of calculations: " << sum << std::endl;
    return sum;
}

int fModeFunc(int X){
    if(X == 0){
        return -1;
    }

    int cpid;
    for(int i = 0; i < X; i++){
        cpid = fork();
        if(cpid == 0){
            std::cout << getpid() << std::endl;
            SumCalc();
            return 1;
        }
    }

    std::cout << "Parent is finished, now assuming wait" << std::endl;
    waitpid(cpid, nullptr, 0);
    std::cout << "Parent finished waiting" << std::endl;
    return 0;
}

int tModeFunc(int X){
    if(X == 0){
        return -1;
    }
    std::vector<std::thread> threads;
    for(int i = 0; i < X; i++){
        threads.push_back(std::thread(SumCalc));
    }

    while(!threads.empty()){
        std::cout << "joining thread" << std::endl;
        threads.back().join();
        threads.pop_back();
    }
    return 0;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        std::cout << "One argument is required" << std::endl;
        return -1;
    }

    if(strcmp(argv[1], "-i") == 0){
        if(iModeFunc() == 0){
            std::cout << "Success!" << std::endl;
        }
    }

    if(strcmp(argv[1], "-f") == 0){
        if(argc < 3){
            std::cout << "Not enough arguments..." << std::endl;
            return -1;
        }
        
        if(fModeFunc(atoi(argv[2])) == 0){
            std::cout << "Success!" << std::endl;
        }
    }

    if(strcmp(argv[1], "-t") == 0){
        if(argc < 3){
            std::cout << "Not enough arguments..." << std::endl;
            return -1;
        }

        if(tModeFunc(atoi(argv[2])) == 0){
            std::cout << "Success!" << std::endl;
        }
    }

    /*
    switch(argc){
        case 1:
            std::cout << "At least one argument is required!" << std::endl;
            break;
        case 2:
            std::cout << argv[1] << std::endl;
            if(strcmp(argv[1], "-i") == 0){
                std::cout << "Running -i mode" << std::endl;
                iModeFunc();
            }else{
                std::cout << "Invalid argument (1 inputted)" << std::endl;
            }
            break;
        case 3:
            if(strcmp(argv[1], "-f") == 0){
                std::cout << argv[2] << "Running -f mode" << std::endl;
                fModeFunc();
            }else if(strcmp(argv[1], "-t") == 0){
                std::cout << argv[2] << "Running -t mode" << std::endl;
                tModeFunc();
            }else{
                std::cout << "Invalid argument (2 inputted)" << std::endl;
            }
            break;
        case 4:
            if(strcmp(argv[1], "-i") == 0){
                std::cout << "Running -i mode" << std::endl;
                iModeFunc();
                if(strcmp(argv[2], "-f") == 0){
                    std::cout << "Running -f mode" << std::endl;
                    fModeFunc();
                }else if(strcmp(argv[2], "-t") == 0){
                    std::cout << "Running -t mode" << std::endl;
                    tModeFunc();
                }
            }else{
                std::cout << "Invalid arguments (3 inputted)" << std::endl;
            }
            break;
        case 5:
            if(strcmp(argv[1], "-f") == 0){
                std::cout << "Running -f mode" << std::endl;
                iModeFunc();
                if(strcmp(argv[3], "-t") == 0){
                    std::cout << "Running -t mode" << std::endl;
                    tModeFunc();
                }   
            }else{
                std::cout << "Invalid arguments (4 inputted)" << std::endl;
            }
            break;
        case 6:
            if(strcmp(argv[1], "-i") == 0 && strcmp(argv[2], "-f") == 0 && strcmp(argv[4], "-t") == 0){
                std::cout << "running all modes" << std::endl;
                iModeFunc();
                fModeFunc();
                tModeFunc();
            }else{
                std::cout << "Invalid arguments (5 inputted)" << std::endl;
            }
            break;
        default:
            std::cout << "Invalid arguments, hardware (-i), forks (-f X), threads (-t X)" << std::endl;
    }
    */
    return 0;
}

