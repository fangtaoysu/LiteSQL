#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include <sstream>
#include <experimental/filesystem>
#include <map>
#include <functional>


class Database
{
private:
    /* data */
    std::string cmd_;
    std::string operator_;
    std::string db_name_;
    std::map<std::string, std::function<void()>> cmd_map_;
    bool success_;
    bool ValidName();
    bool ValidLength();
    void CreateFolder();
    void DeleteFolder();
    void UnRealizedCmd();
    
public:
    Database(const std::string &cmd);
    ~Database();
    bool Run();
    std::string UsingDB();
};

#endif