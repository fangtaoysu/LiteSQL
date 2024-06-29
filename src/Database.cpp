#include "../include/Database.h"

// 调试
#include <iostream>
using std::cout;
using std::endl;

namespace fs = std::experimental::filesystem;

bool Database::Run() {
    /** 1. 检查数据库名称
     *  2. 将数据库名称传递给对应操作处理函数
     */
    // 检查数据库名称是否正确
    if (!success_) {
        std::cerr << "database name: '" << db_name_
                  << "' is not standardized." << endl;
        return false;
    }
    // 根据操作符operator_执行命令
    auto it = cmd_map_.find(operator_);
    if (it != cmd_map_.end()) {
        it->second();
    } else {
        UnRealizedCmd();
        success_ = false;
    }
    return success_;
}

void Database::UnRealizedCmd() const {
    /**
     * 遇到未实现的命令，提示并结束
     */
    std::cerr << "cmd: " << cmd_
              << "\nunknown command!" << endl;
}

void Database::Delete() const {
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

void Database::Create() const {
    /**
     * 通过创建文件夹的方式创建数据库
     */
        // 在新建数据库之前检查数据库是否存在
    if (operator_ == "CREATE" && fs::exists(db_dir_)) {
        std::cerr << "databse name: '" << db_name_
                  << "' already exist, use " << db_name_ << endl;
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
    std::getline(is >> std::ws, db_name_); // 赋值数据库名称
    db_dir_ = "../database/" + db_name_;
    success_ = (db == "DATABASE"); // 检查DATABASE的位置
    // 使用lambda函数绑定
    cmd_map_["CREATE"] = [this]() {this->Create();};
    cmd_map_["DROP"] = [this]() {this->Delete();};
    success_ = IsRightName(db_name_) & IsNoSpace(db_name_);
}

Database::~Database() {
    
}

bool Database::IsRightName(const std::string &name) const {
    /**
     * 如果包含标点符号，返回false，否则返回true
     */
    for (const char c : name) {
        if (std::ispunct(static_cast<unsigned char>(c))) {
            // cout << "有标点符号" << endl;
            return false;
        }
    }
    return true;
}

bool Database::IsNoSpace(const std::string &str) const {
    /**
     * 检查是否包含空格
     */
    std::string::size_type pos = 0;
    while ((pos = str.find_first_of(" ", pos)) != std::string::npos) {
        return false;
    }
    return true;
}