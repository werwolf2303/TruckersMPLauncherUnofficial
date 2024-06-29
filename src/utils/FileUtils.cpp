//
// Created by werwolf2303 on 6/28/24.
//

#include "FileUtils.h"

#include <openssl/md5.h>
#include <iomanip>
#include <sstream>
#include <boost/iostreams/device/mapped_file.hpp>

std::string FileUtils::calculateMD5(std::filesystem::path filePath) {
    unsigned char result[MD5_DIGEST_LENGTH];
    boost::iostreams::mapped_file_source src(filePath);
    MD5((unsigned char*)src.data(), src.size(), result);
    std::ostringstream sout;
    sout<<std::hex<<std::setfill('0');
    for(auto c: result) sout<<std::setw(2)<<(int)c;
    return sout.str();
}

bool FileUtils::pathExists(const std::filesystem::path& p, std::filesystem::file_status s)
{
    return (std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(p));
}
