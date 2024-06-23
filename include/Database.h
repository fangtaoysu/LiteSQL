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
    std::string db_dir_;
    std::map<std::string, std::function<void()>> cmd_map_;
    bool success_;
    bool ValidName();
    bool ValidLength();
    void Create();
    void Delete();
    void UnRealizedCmd();
public:
    Database(const std::string &cmd);
    ~Database();
    bool Run();
    std::string GetDB() {
        return db_name_;
    }
};

#endif