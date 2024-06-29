#ifndef RECORD_H
#define RECORD_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <numeric>
#include <experimental/filesystem>
#include "json.hpp"

using json = nlohmann::json;

class Record
{
private:
    /* data */
    std::string cmd_;
    std::string db_path_;
    bool success_;
    std::string table_path_;
    std::string config_path_;

    bool IsRightSelect();
    std::vector<bool> ConstructColumnIndices(const std::string &select_colum, json &config_json);
    void ExtractSQLComponents(std::string& select_part);
    std::string ConstructWire(json &config_json, const std::vector<bool> &column_indices, std::vector<std::size_t> &max_lengths);
    void Split(std::string & str);
    void PrintFormat(const std::vector<bool> &column_indices, const std::vector<size_t> &spaces, const std::string& wire);
    // std::vector<std::string> SplitStringByCommas(const std::string &str);

public:
    Record(const std::string& cmd, const std::string& DATABASE);
    ~Record();
    bool Select();
};

inline void Record::Split(std::string & str) {
    /**
     * 去掉字符串首尾的空格
     */
    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
    str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);
}


#endif