#include "../include/Table.h"

// 调试
#include <iostream>
using std::cout;
using std::endl;
using json = nlohmann::json;

namespace fs = std::experimental::filesystem;

bool Table::Run() {
    /** 根据对应命令执行相应的操作
     */
    // 检验输入的命令是否规范
    if (!success_) {
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

void Table::UnRealizedCmd() {
    /**
     * 如果没有实现或命令输入错误
     */
    success_ = false;
    std::cerr << "cmd: " << cmd_
              << "\nunknown command!" << endl;
}

void Table::Delete() {
    /**
     * 通过删除文件夹的方式删除数据库
     */
    std::string dir = "../Table/" + table_name_;
}

void Table::Create() {
    /**
     * 通过创建csv文件的创建Table
     */
    if (!VaildColumnName() || !ValidColumnInfo() || !ValidName()) {
        return;
    }
    SplitColumn();
    // 创建对应配置文件，保存列信息
    CreateFile();
}

bool Table::VaildColumnName() {

    if (fs::exists(config_name_.str()) || fs::exists(table_path_)) {
        std::cerr << "table: '" << table_name_
                  << "' has existed." << endl;
        return false;
    }
    return true;
}

void Table::CreateFile() {
    /**
     * 根据column_vec_信息创建文件
     *  整数类型：BIT、BOOL、TINY INT、SMALL INT、MEDIUM INT、 INT、 BIG INT
     *  浮点数类型：FLOAT、DOUBLE、DECIMAL
     *  字符串类型：CHAR、VARCHAR、TINY TEXT、TEXT、MEDIUM TEXT、LONGTEXT、TINY BLOB、BLOB、MEDIUM BLOB、LONG BLOB
     *  日期类型：Date、DateTime、TimeStamp、Time、Year
     *  其他数据类型：BINARY、VARBINARY、ENUM、SET、Geometry、Point、MultiPoint、LineString、MultiLineString、Polygon、GeometryCollection等
     * 
     * 先实现：INT-int FLOAT-float VARCHAR(n)-array[n]
     */
    std::map<std::string, std::string> config_map;
    for (const auto &line : column_vec_) {
        // 现阶段进获取类型和列名，不获取主键等其他信息
        std::istringstream iss(line);
        std::string column_name, column_type;
        iss >> column_name >> column_type;
        config_map[column_name] = column_type;
    }
    // 创建配置文件
    json config_json = config_map;
    std::string config_folder = DB_+ "/config";
    if (!fs::exists(config_folder)) {
        fs::create_directories(config_folder);
    }
    std::ofstream o(config_name_.str());
    if (o.is_open()) {
        o << std::setw(4) << config_json << endl;
        o.close();
        cout << "written success!" << endl;
    } else {
        std::cerr << "failed to open :" << config_name_.str() << endl;
    }
    // 创建csv文件
    std::ofstream table;
    table.open(table_path_, std::ios::out | std::ios::trunc);
    for (auto &col : config_map) {
        table << col.first << ",";
    }
    table << endl;
}

bool Table::ValidColumnInfo() {
    /**
     * 验证列信息是否被小括号包围
     * CREATE TABLE user (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(100), email VARCHAR(100))
     * 如果有两个括号，怎么办（括号匹配？）
     */
    if (column_info_.front() == '(' && column_info_.back() == ')') {
        column_info_ = column_info_.substr(1, column_info_.size()-2);
        
        return true;
    }
    std::cerr << "column info: " << column_info_
              << " is ilegal." << endl;
    return false;
    
}

void Table::SplitColumn() {
    /**
     * 分割列信息，存入column_vec_中
     */
    size_t start = 0, end = 0;
    while ((end = column_info_.find(",", start)) != std::string::npos) {
        column_vec_.push_back(column_info_.substr(start, end-start));
        start = end + 1;
    }
    column_vec_.push_back(column_info_.substr(start));
}

Table::Table(const std::string &cmd, const std::string &DB)
    : cmd_(cmd), DB_(DB) {
    // 检查数据库
    if (DB.empty()) {
        std::cerr << "unselect database!!!" << endl;
        success_ = false;
    }
    // 成员变量赋值
    //  拆分cmd
    std::istringstream is(cmd);
    is >> operator_; // 赋值操作符
    std::string table;
    is >> table;
    is >> table_name_;
    std::string config_folder = DB_+ "/config/";
    config_name_ << config_folder << table_name_ << "_config.json";
    table_path_ = DB_ + "/" + table_name_ + ".csv";
    // 赋值列信息
    std::getline(is >> std::ws, column_info_); // std::ws去掉第一个空格
    // 检查Table的位置
    if (table != "TABLE"){
        std::cerr << "Table name: '" << table_name_
                << "' is not standardized." << endl;
        success_ = false;
    }
    // 使用lambda函数绑定
    cmd_map_["CREATE"] = [this]() {this->Create();};
    cmd_map_["DROP"] = [this]() {this->Delete();};
    success_ = true;
}

Table::~Table() {
    
}

bool Table::ValidName() {
    /**
     * 如果包含标点符号，返回false，否则返回true
     */
    for (const char c : table_name_){
        if (std::ispunct(static_cast<unsigned char>(c))) {
            // cout << "有标点符号" << endl;
            return false;
        }
    }
    return true;
}

bool Table::ValidLength() {
    /**
     * 统计剩余文本的长度
     */
    std::string::size_type pos = 0;
    // 检验是否包含多余的空格
    while ((pos = table_name_.find_first_of(" ", pos)) != std::string::npos) {
        return false;
    }
    return true;
}