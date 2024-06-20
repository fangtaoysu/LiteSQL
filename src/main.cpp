#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <functional>
#include <ios>
#include <cctype>
#include <experimental/filesystem>
#include "../include/Database.h"
// #include "../include/Table.h"
// #include "../include/Record.h"

using std::cin;
using std::cout;
using std::endl;

// g++ main.cpp Database.cpp -o main -lstdc++fs

bool Execute(const std::string &cmd) {
    /**
     * 将命令字符映射到对应的函数中，并调用该函数
     */
    bool success = true;
    if (cmd.find("DATABASE") != std::string::npos) {
        // 调用database的类逻辑
        Database db(cmd);
        success = db.Run();
    } else if (cmd.find("Table") != std::string::npos) {
        // 调用Table的类逻辑
        ;
    } else {
        // 调用Record的类逻辑，同时检查命令是否符合规范
        ;
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