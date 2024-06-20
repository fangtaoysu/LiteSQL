#include "../include/Database.h"

// 调试
#include <iostream>
using std::cout;
using std::endl;

namespace fs = std::experimental::filesystem;

bool Database::Run() {
    /** 1. 检查数据库名称
     *   2. 将数据库名称传递给对应操作处理函数
     */
    // 检验输入的命令是否规范
    success_ = success_ & ValidName() & ValidLength();
    if (!success_) {
        std::cerr << "database name: '" << db_name_
                  << "' is not standardized." << endl;
        return false;
    }
    // 执行命令
    auto it = cmd_map_.find(operator_);
    if (it != cmd_map_.end()) {
        it->second();
    } else {
        UnRealizedCmd();
    }
    return success_;
}

std::string Database::UsingDB() {
    /**
     * 检查这个文件夹是否存在 - 暂时没有调用的思路
     */
    std::string dir = "../database/" + db_name_;
    if (fs::exists(dir)) {
        return dir;
    }
    std::cerr << "DATABASE '" << db_name_
              << "' does not exist." << endl;
    return "";
}

void Database::UnRealizedCmd() {
    success_ = false;
    std::cerr << "cmd: " << cmd_
              << "\nunknown command!" << endl;
}

void Database::DeleteFolder() {
    /**
     * 通过删除文件夹的方式删除数据库
     */
    std::string dir = "../database/" + db_name_;
    if (fs::exists(dir)) {
        fs::remove_all(dir);
        cout << "DATABASE '" << db_name_
             << "' deleted successfully." << endl;
    } else {
        std::cerr << "DATABASE '" << db_name_
                  << "' does not exist." << endl;
    }
}

void Database::CreateFolder() {
    /**
     * 通过创建文件夹的方式创建数据库
     */
    std::string dir = "../database/" + db_name_;
    if (fs::exists(dir)) {
        std::cerr << "DATABASE: '" << db_name_ << 
                  "' already exists." << endl;
        
    } else {
        fs::create_directory(dir);
        cout << "DATABASE: '" << db_name_ << 
             "' already create." << endl;
    }
}

Database::Database(const std::string &cmd) : cmd_(cmd) {
    std::istringstream is(cmd);
    is >> operator_; // 赋值操作符
    std::string db;
    is >> db; 
    std::getline(is >> std::ws, db_name_); // 赋值数据库名称
    success_ = (db == "DATABASE"); // 检查DATABASE的位置
    // 使用lambda函数绑定
    cmd_map_["CREATE"] = [this]() {this->CreateFolder();};
    cmd_map_["DROP"] = [this]() {this->DeleteFolder();};
}

Database::~Database() {
    
}

bool Database::ValidName() {
    /**
     * 如果包含标点符号，返回false，否则返回true
     */
    for (const char c : db_name_){
        if (std::ispunct(static_cast<unsigned char>(c))) {
            // cout << "有标点符号" << endl;
            return false;
        }
    }
    return true;
}

bool Database::ValidLength() {
    /**
     * 统计剩余文本的长度
     */
    std::string::size_type pos = 0;
    // 检验是否包含多余的空格
    while ((pos = db_name_.find_first_of(" ", pos)) != std::string::npos) {
        return false;
    }
    return true;
}