//
// Created by werwolf2303 on 6/28/24.
//

#ifndef LAUNCHER_H
#define LAUNCHER_H
#include <functional>
#include <string>


class Launcher {
public:
    static void downloadTMPFiles(std::string prefixPath, std::function<int(int maxProgress, int minProgress, int actualProgress, std::string progressDescription)> onProgress = nullptr);
    static void verifyFiles(std::string prefixPath, std::function<int(int maxProgress, int minProgress, int actualProgress, std::string progressDescription)> onProgress = nullptr);
    static void checkFoldersAndFiles(std::string prefixPath, std::function<int(int maxProgress, int minProgress, int actualProgress, std::string progressDescription)> onProgress = nullptr);
    static void downloadProton(std::string prefixPath, std::function<int(int maxProgress, int minProgress, int actualProgress, std::string progressDescription)> onProgress = nullptr);
    static void downloadTMPCLI(std::string prefixPath, std::function<int(int maxProgress, int minProgress, int actualProgress, std::string progressDescription)> onProgress = nullptr);
};



#endif //LAUNCHER_H
