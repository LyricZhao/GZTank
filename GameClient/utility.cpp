#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <string.h>

#include "utility.h"

std:: vector<std:: string> simLs(std:: string path) {
    std:: vector<std:: string> files;

    DIR *dir;
    struct dirent *ptr;

    if((dir = opendir(path.c_str())) == NULL) {
        std:: cerr << "opendir error" << std:: endl;
        exit(1);
    }

    while((ptr = readdir(dir)) != NULL) {
        if(strcmp(ptr -> d_name,  ".") == 0) continue;
        if(strcmp(ptr -> d_name, "..") == 0) continue;

        if(ptr -> d_type == 2) files.push_back(ptr -> d_name); // file only
    }
    closedir(dir);

    return files;
}

std:: vector<std:: string> filter(const std:: vector<std:: string> &tfVec, std:: string msStr) {
    std:: vector<std:: string> afVec;

    for(auto str: tfVec) {
        if(str.find(msStr) != std:: string:: npos) {
            afVec.push_back(str);
        }
    }

    return afVec;
}
