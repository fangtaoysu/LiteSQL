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
    bool IsRightName(const std::string &name) const;
    bool IsNoSpace(const std::string &str) const;
    void Create() const;
    void Delete() const;
    void UnRealizedCmd() const;
public:
    Database(const std::string &cmd);
    ~Database();
    bool Run();
    std::string GetDB() const{
        return db_name_;
    }
};

#endif