#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <any>
#include <functional>
#include <ios>
#include <cctype>
#include <experimental/filesystem>

#include <experimental/filesystem>
#include "../include/Database.h"
#include "../include/Table.h"
// #include "../include/Record.h"

using std::cin;
using std::cout;
using std::endl;
namespace fs = std::experimental::filesystem;

std::string DATABASE;

// g++ main.cpp Database.cpp -o main -lstdc++fs

std::string UsingDB(std::string db_name) {
    /**
     * 检查这个文件夹是否存在 - 暂时没有调用的思路
     */
    std::string dir = "../database/" + db_name;
    if (fs::exists(dir)) {
        return dir;
    }
    std::cerr << "DATABASE '" << dir
              << "' does not exist." << endl;
    return "";
}

bool Execute(const std::string &cmd) {
    /**
     * 将命令字符映射到对应的函数中，并调用该函数
     */
    bool success = true;
    if (cmd.find("USE") != std::string::npos) {
        std::istringstream iss(cmd);
        std::string use, db_name;
        iss >> use >> db_name;
        if (use == "USE") {
            DATABASE = UsingDB(db_name);
        } else {
            success = false;
        }
    } else if (cmd.find("DATABASE") != std::string::npos) {
        // 调用database的类逻辑
        Database db(cmd);
        success = db.Run();
        DATABASE = UsingDB(db.GetDB());
    } else if (cmd.find("TABLE") != std::string::npos) {
        // 调用Table的类逻辑
        Table table(cmd, DATABASE);
        success = table.Run();
    } else {
        // 调用Record的类逻辑，同时检查命令是否符合规范
        ;
    }
    return success;
}

int main() {
    bool more = true;
    std::string command("");
    // 如果输入exit或者ctrl+d，则退出
    while (more && !std::cin.rdstate()) {
        bool cmd_more = true, success = true;
        std::string cmd;
        // 获取一行输入
        std::getline(std::cin, cmd);
        if (cmd == "exit") {
            break;
        }
        // 检查末尾是否有分号
        // cout << "end is: " << cmd.back() << endl;
        if (cmd.back() == ';') {
            cmd.pop_back();
            cmd_more = false;
        }
        // 将之前的输入和本次输入拼接起来
        command += cmd;
        // 此处先使用bool+cerr反馈命令执行结果
        if (!cmd_more) {
            success = Execute(command);
            command.clear();
        }
        if (!success && std::cin.rdstate() == 0) {
            std::cerr << "Input command is unrealized or wrong!" << endl;
        }
    }
    if (std::cin.rdstate()) {
        std::cerr << "LiteSQL have logged out!" << endl; 
    }
    return 0;
}