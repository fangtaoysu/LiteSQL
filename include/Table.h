#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <functional>
#include <experimental/filesystem>
#include <map>
#include <vector>
#include <sstream>
#include <json.hpp>
#include <fstream>

class Table
{
private:
    /* data */
    std::string cmd_;
    std::string operator_;
    std::string table_name_;
    std::string DB_;
    std::stringstream config_name_;
    std::string table_path_;
    std::map<std::string, std::function<void()>> cmd_map_;
    bool success_;
    std::string column_info_;
    bool ValidName();
    bool ValidLength();
    void Create();
    void Delete();
    void UnRealizedCmd();
    std::vector<std::string> column_vec_;
    void SplitColumn();
    bool ValidColumnInfo();
    bool VaildColumnName();
    void CreateFile();
public:
    Table(const std::string &cmd, const std::string &DB);
    ~Table();
    bool Run();
};

#endif