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
#include "../include/Record.h"

using std::cin;
using std::cout;
using std::endl;
namespace fs = std::experimental::filesystem;

std::string DATABASE;

std::string UsingDB(std::string db_name) {
    /**
     * 为全局变量DATABASE赋值
     */
    std::string dir = "../database/" + db_name;
    if (fs::exists(dir)) {
        std::cerr << "DATABASE changed" << endl;
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
    } else if (cmd.find("SELECT") != std::string::npos) {
        Record record(cmd, DATABASE);
        success = record.Select();
    } else if (cmd.find("DATABASE") != std::string::npos) {
        Database db(cmd);
        success = db.Run();
        if (success) {
            DATABASE = UsingDB(db.GetDB());
        }
    } else if (cmd.find("TABLE") != std::string::npos) {
        Table table(cmd, DATABASE);
        success = table.Run();
    } else if (cmd.find("INSERT INTO") != std::string::npos) {
        Table table(cmd, DATABASE);
        success = table.Insert();
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
        std::getline(std::cin, cmd);
        if (cmd == "exit") { // 检测exit
            break;
        }
        // 检查末尾是否有分号
        if (cmd.back() == ';') {
            cmd.pop_back();
            cmd_more = false;
        }
        // 将分号之前的所有内容通过空格连接起来
        command += cmd.insert(0, " ");
        if (!cmd_more) {
            success = Execute(command);
            command.clear();
        }
        if (!success && std::cin.rdstate() == 0) {
            std::cerr << "Input command is unrealized or wrong!" << endl;
        }
    }
    if (std::cin.rdstate()) { // 如果检测到本次终端的输入流结束
        std::cerr << "LiteSQL have logged out!" << endl; 
    }
    return 0;
}