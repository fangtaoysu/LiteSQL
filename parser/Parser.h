// 语法分析器 - 将sql语句转化为对应的AST节点
#pragma once
#include "ASTNode.h"
#include <sstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <memory>


class Parser {
public:
    static std::unique_ptr<ASTNode> ParseCreateTable(const std::string& command) {
        std::istringstream iss_(command);
        std::string keyword_, table_keyword_, table_name_, columns_part_;
        iss_ >> keyword_ >> table_keyword_ >> table_name_;

        if (keyword_ != "CREATE" || table_keyword_ != "TABLE") {
            throw std::runtime_error("Invalod CREATE TABLE command");
        }

        // 提取括号中的列定义
        size_t start = command.find('(');
        size_t end = command.find(')');
        if (start == std::string::npos || end == std::string::npos || start >= end) {
            throw std::runtime_error("Invalid column definition");
        }

        columns_part_ = command.substr(start + 1, end - start - 1);
        std::vector<std::pair<std::string, std::string>> columns;

        // 解析列定义
        // TODO 加入懒匹配，完善输入检查
        std::istringstream col_stream(columns_part_);
        std::string column;
        while (std::getline(col_stream, column, ',')) {
            std::istringstream col_def(column);
            std::string col_name, col_type;
            col_def >> col_name >> col_type;
            columns.emplace_back(col_name, col_type);
        }

        // 构建对应的AST节点
        return std::make_unique<CreateTableNode>(table_name_, columns);
    }
};
