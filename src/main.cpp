#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <functional>
#include <ios>
#include <cctype>
#include <experimental/filesystem>


using std::cin;
using std::cout;
using std::endl;

namespace fs = std::experimental::filesystem;

// g++ main.cpp -o main -lstdc++fs

bool ValidLength(std::string &text, int n) {
    /**
     * 统计剩余文本的长度
     */
    int count = 0;
    std::string::size_type pos = 0;
    while ((pos = text.find_first_of(" ", pos)) != std::string::npos) {
        ++pos;
        ++count;
    }
    // cout << count << endl;
    return count == (n - 1);
}

bool VaildName(std::string &text) {
    /**
     * 如果包含标点符号，返回false，否则返回true
     */
    for (const char c : text){
        if (std::ispunct(static_cast<unsigned char>(c))) {
            cout << "有标点符号" << endl;
            return false;
        }
    }
    return true;
}

void CreateFolder(std::string &name) {
    /**
     * 通过创建文件夹的方式创建数据库
     */
    std::string dir = "../database/" + name;
    if (fs::exists(dir)) {
        std::cerr << "DATABASE: " << dir << 
                  " already exists." << endl;
    } else {
        fs::create_directory(dir);
        cout << "DATABASE: " << name << 
             "already create." << endl;
    }
}

bool CreateDatabase(std::istringstream &db_name) {
    /**
     * 相当于创建该名字的文件夹
     */
    // 获取剩余字符串
    std::string remaining;
    std::getline(db_name >> std::ws, remaining);
    bool success = ValidLength(remaining, 1) & VaildName(remaining);;
    if (!success) { // 命令有误
        std::cerr << "database name is wrong: " << remaining << endl;
        return success;
    }
    CreateFolder(remaining);
    return success;

}

bool CreateCmd(std::istringstream &input) {
    /**
     * 数据库CREATE命令
     *  CREATE DATASET my_database; // 创建一个my_database文件夹
     *  CREATE TABLE my_table;      // 创建一个my_table的json文件
     */
    bool success = true;
    std::string second_cmd;
    input >> second_cmd;
    std::map<std::string, std::function<bool(std::istringstream &)>> cmd_map;
    cmd_map["DATABASE"] = CreateDatabase;
    // cmd_map["TABLE"] = CreateTable;
    auto it = cmd_map.find(second_cmd);
    if (it != cmd_map.end()) {
        success = it->second(input);
    } else {
        success = false;
    }
    return success;
}

bool Execute(const std::string &cmd) {
    /**
     * 将命令字符映射到对应的函数中，并调用该函数
     */
    bool success = true;
    std::istringstream input(cmd);
    std::string first_cmd;
    input >> first_cmd;
    // 使用map<string, function>
    // bool 是返回值，std::istringstream &是参数类型
    std::map<std::string, std::function<bool(std::istringstream &)>> cmd_map;
    cmd_map["CREATE"] = CreateCmd;
    // cmd_map["DROP"] = DropCmd;
    // cmd_map["ALTER"] = AlertCmd;
    // cmd_map["UPDATE"] = UpdateCmd;
    // cmd_map["DELETE"] = DeleteCmd;
    // cmd_map["SELECT"] = SelectCmd;
    // cmd_map["GRANT"] = GrantCmd;
    // cmd_map["REVOKE"] = RevokeCmd;
    // 查找命令
    auto it = cmd_map.find(first_cmd);
    if (it != cmd_map.end()) {
        // 此处可能需要增加try catch
        success = it->second(input); // 调用执行函数
    } else {
        success = false;
    }
    return success;
}

int main() {
    bool more = true;
    std::string cmd;
    // 如果输入exit或者ctrl+d，则退出
    while (more && !std::cin.rdstate()) {
        // 获取一行输入
        std::getline(std::cin, cmd);
        if (cmd == "exit") {
            break;
        }
        // 此处先使用bool+cerr反馈命令执行结果
        bool success = Execute(cmd);
        if (!success && std::cin.rdstate() == 0) {
            std::cerr << "Input command is unrealized or wrong!" << endl;
        }
    }
    if (std::cin.rdstate()) {
        std::cerr << "LiteSQL have logged out!" << endl; 
    }
    return 0;
}