// 抽象语法树节点
#pragma once // 头文件保护
#include <string>
#include <vector>
#include <iostream>


// 抽象基类，所有AST节点的基类
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void Execute() = 0; // 执行该节点逻辑
};

class CreateTableNode : public ASTNode {
    /**CREATE TABLE 的节点 */
    std::string table_name_;
    std::vector<std::pair<std::string, std::string>> columns_; // 列名和类型
public:
    CreateTableNode(const std::string& name, const std::vector<std::pair<std::string, std::string>> cols)
        : table_name_(name), columns_(cols) {}
    void Execute() override {
        std::cout << "Executing CREATE TABLE command:\n";
        std::cout << "Table Name: " << table_name_ << "\nColumns:\n";
        for (const auto& col : columns_) {
            std::cout << "  - " << col.first << " " << col.second << "\n";

        }
    }
}; 