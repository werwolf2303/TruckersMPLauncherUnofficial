//
// Created by werwolf2303 on 6/28/24.
//

#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <filesystem>


class FileUtils {
public:
    static std::string calculateMD5(std::filesystem::path filePath);
    static bool pathExists(const std::filesystem::path& p, std::filesystem::file_status s = std::filesystem::file_status{});
};



#endif //FILEUTILS_H
