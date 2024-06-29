//
// Created by werwolf2303 on 6/28/24.
//

#ifndef LAUNCHER_H
#define LAUNCHER_H
#include <string>


class Launcher {
public:
    void downloadTMPFiles(std::string prefixPath);
    void verifyFiles(std::string prefixPath);
    void checkFoldersAndFiles(std::string prefixPath);
    void downloadProton(std::string prefixPath);
    void downloadTMPCLI(std::string prefixPath);
};



#endif //LAUNCHER_H
