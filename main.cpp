// 项目入口
#include "parser/Parser.h"
#include "executor/Executor.h"
#include <iostream>


int main() {
    // std::string cmd = "CREATE TABLE users (id INT, name VARCHAR(100));";
    bool end_symbol = false;
    std::string command("");
    // 输入exit或ctrl+d退出
    while (!end_symbol && !std::cin.rdstate()) {
        std::cout << "SQL> ";
        bool cmd_more = true, success = true;
        std::string cmd("");
        std::getline(std::cin, cmd);
        if (cmd == "exit") {
            break;
        } 
        // 检查末尾是否有分号
        if (cmd.back() == ';') {
            cmd.pop_back();
            cmd_more = false;
        }
        // 将分号之前的内容用空格连接起来
        command += cmd.insert(0, " ");
        if (!cmd_more) {
            auto ast = Parser::ParseCreateTable(cmd);
            Executor executor;
            executor.Execute(ast);
            // success = Execute(command);
            command.clear();
        }
        if (!success && std::cin.rdstate() == 0) {
            std::cerr << "Input command is unrealized or wrong!" << std::endl;
        }
    }
    if (std::cin.rdstate()) {
        std::cerr << "LiteSQL has logged out!\n";
    }

    return 0;
}