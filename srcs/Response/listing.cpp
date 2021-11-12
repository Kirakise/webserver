#include "Response.hpp"
#include <sstream>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream> 
#include <vector>
#include <dirent.h> 
#include <stdio.h>
#include <regex>
#include <unordered_map>

std::unordered_map<std::string, std::string> MIMES;

std::string readFile(std::string& filename) {
    std::ifstream f;
    f.open(filename, std::ios::in);
    std::stringstream ss;
    ss << f.rdbuf();
    f.close();
    return ss.str();
}

std::string formatdate(time_t val)
{
        char buff[200];
        char *str = buff;
        strftime(str, 36, "%d.%m.%Y %H:%M:%S", localtime(&val));
        return std::string(str);
}

off_t GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

long GetFileModTime(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_mtime : - 1;
}

int isDirectory(std::string path) {
   struct stat statbuf;
   if (stat(path.c_str(), &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

std::string removeExtra(std::string filename){
    std::string::iterator it = filename.end();
    it--;
    while (it != filename.begin() && *it != '/')
        it--;
    if (it == filename.begin() && *it != '/')
        return filename;
    else
        return filename.substr(it + 1 - filename.begin());
}

std::string addFile(std::string filename){
    std::string s = "<script>addRow(\"" + removeExtra(filename) + "\",\"" + removeExtra(filename) + "\"," + \
    std::to_string(isDirectory(filename)) + "," + std::to_string(GetFileSize(filename)) + \
    ",\"" + std::to_string(GetFileSize(filename)/8) + " Bytes\"," + std::to_string(GetFileModTime(filename)) + \
    ",\"" + formatdate(GetFileModTime(filename)) + "\");</script>\n";
    return s;
}

std::vector<std::string> listFiles(std::string path)
{
    DIR *d;
    std::vector <std::string> v;
    struct dirent *dir;
    d = opendir(path.c_str());
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            v.push_back(path + dir->d_name);
    }
    closedir(d);
    }
    return v;
}



std::string getListingResponse(std::string path)
{
    std::vector <std::string> v = listFiles(path);
    std::string s = "res/listing.html";
    std::string ret(readFile(s) + "\n");
    size_t index = 0;
    index = ret.find("/Users/rcaraway/webserver/", index);
    if (index != std::string::npos)
        ret.replace(index, 26, path);
    for (int i = 0; i < v.size(); i++)
        ret.append(addFile(v[i]));
    return ret;
}