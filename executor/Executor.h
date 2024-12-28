// AST执行器 - 负责调用AST节点的Execute方法，完成操作
#pragma once
#include "../parser/ASTNode.h"
#include <memory>

class Executor {
public:
    void Execute(std::unique_ptr<ASTNode>& ast) {
        ast->Execute();
    }
};