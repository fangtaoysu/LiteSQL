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
    if(fs::exists(db_dir_)) {
        std::cerr << "databse name: '" << db_name_
                  << "' already exist, use " << db_name_ << endl;
        return true;
    }
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

void Database::UnRealizedCmd() {
    success_ = false;
    std::cerr << "cmd: " << cmd_
              << "\nunknown command!" << endl;
}

void Database::Delete() {
    /**
     * 通过删除文件夹的方式删除数据库
     */
    std::string dir = "../database/" + db_name_;
    if (fs::exists(db_dir_)) {
        fs::remove_all(db_dir_);
        cout << "DATABASE '" << db_name_
             << "' deleted successfully." << endl;
    } else {
        std::cerr << "DATABASE '" << db_name_
                  << "' does not exist." << endl;
    }
}

void Database::Create() {
    /**
     * 通过创建文件夹的方式创建数据库
     */
    
    if (fs::exists(db_dir_)) {
        std::cerr << "DATABASE: '" << db_name_ << 
                  "' already exists." << endl;
        
    } else {
        fs::create_directory(db_dir_);
        cout << "DATABASE: '" << db_name_ << 
             "' already create." << endl;
    }
}

Database::Database(const std::string &cmd) : cmd_(cmd) {
    std::istringstream is(cmd);
    is >> operator_; // 赋值操作符
    std::string db;
    is >> db; 
    db_dir_ = "../database/" + db_name_;
    std::getline(is >> std::ws, db_name_); // 赋值数据库名称
    success_ = (db == "DATABASE"); // 检查DATABASE的位置
    // 使用lambda函数绑定
    cmd_map_["CREATE"] = [this]() {this->Create();};
    cmd_map_["DROP"] = [this]() {this->Delete();};
    ValidName();
    ValidLength();
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