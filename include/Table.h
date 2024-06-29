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
#include <stack>
#include <regex>

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
    std::map<std::string, std::vector<std::string>> column_map_;
    void Split(std::string & str);
    bool success_;
    std::string column_info_;
    std::string titles_;
    std::string values_;
    bool ValidName();
    void Create();
    void Delete();
    void UnRealizedCmd();
    std::vector<std::string> column_vec_;
    void SplitColumn();
    bool SplitBrackets(std::string &);
    bool IsExistedTable();
    void CreateFile();
    void InsertValues();
    bool ValidInsert();
    void Split();
    void CollectInfo();
    bool IsRightInsertInfo();
   
public:
    Table(const std::string &cmd, const std::string &DB);
    ~Table();
    bool Run();
    bool Insert();
};

inline void Table::Split(std::string & str) {
    /**
     * 去掉字符串首尾的空格
     */
    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
    str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
}

#endif