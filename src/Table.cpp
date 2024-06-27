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
     * 通过删除csv文件和对应的配置文件删除table
     */
    if (IsExistedTable()) {
        fs::remove(table_path_);
        fs::remove(config_name_.str());
        std::cerr << "'" << table_name_
                  << "' remove successfully." << endl;
    } else {  
        std::cerr << "Can't found table: '" << table_name_
                  << "'!" << endl;
    }
}

void Table::Create() {
    /**
     * 通过创建csv文件的创建Table
     */
    if (IsExistedTable() || !SplitBrackets(column_info_) || !ValidName()) {
        return;
    }
    SplitColumn();
    // 创建对应配置文件，保存列信息
    CreateFile();
}

bool Table::IsExistedTable() {

    if (fs::exists(config_name_.str()) || fs::exists(table_path_)) {
        std::cerr << "table: '" << table_name_
                  << "' existed." << endl;
        return true;
    }
    return false;
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

bool Table::SplitBrackets(std::string &info) {
    /**
     * 验证列信息是否被小括号包围
     * CREATE TABLE user (id INT AUTO_INCREMENT PRIMARY KEY, name VARCHAR(100), email VARCHAR(100))
     * 如果有两个括号，怎么办（括号匹配？）
     */
    if (info.front() == '(' && info.back() == ')') {
        info = info.substr(1, info.size()-2);
        
        return true;
    }
    std::cerr << "column info: " << info
              << " is ilegal." << endl;
    return false;
}

void Table::Split() {
    /**
     * 将column_info_分割成三部分，同时去除所有括号，检查括号是否匹配
     */
    std::string keyword;
    bool title_tag = false, keyword_tag = false, values_tag = false, first_tag = true;
    std::stack<char> brackets;
    bool success_ = true;

    for (const char ch : column_info_) {
        if (ch == '(') {
            brackets.push(ch);
            if (first_tag) { // title 开始
                title_tag = true;
            } else { // keyword 结束, values 开始
                keyword_tag = false;
                values_tag = true;
            }
            continue;
        } else if (ch == ')') {
            brackets.pop();
            if (first_tag) { // titile 结束，keyword 开始
                title_tag = false;
                keyword_tag = true;
                first_tag = false;
            } 
            continue;
        }
        // 根据tag写入值
        if (title_tag) {
            titles_ += ch;
        } else if (keyword_tag) {
            keyword += ch;
        } else if (values_tag) {
            values_ += ch;
        }
    }
    if (!brackets.empty()) {
        std::cerr << "brackets isn't right!" << endl;
        success_ = false;
    }
    // 去除空格
    keyword.erase(0, keyword.find_first_not_of(" \n\r\t"));
    keyword.erase(keyword.find_last_not_of(" \n\r\t") + 1);
    if (keyword != "VALUES") {
        success_ = false;
    }
    // cout << "Title: " << titles_ << endl; 
    // cout << "keyword: " << keyword << endl;
    // cout << "Values: " << values_ << endl;
}

void Table::CollectInfo() {
    /**
     * 将分割好的info组织成键值对的形式column_info
     */
    std::vector<std::string> titles, values;
    std::string title, value, token;
    std::stringstream title_stream(titles_);
    std::stringstream values_stream(values_);

    // Split titles by commas
    while (std::getline(title_stream, token, ',')) {
        // Remove leading/trailing spaces
        token.erase(token.find_last_not_of(" \n\r\t") + 1);
        token.erase(0, token.find_first_not_of(" \n\r\t"));
        titles.push_back(token);
    }
    // Split values by commas
    while (std::getline(values_stream, token, ',')) {
        // Remove leading/trailing spaces
        token.erase(token.find_last_not_of(" \n\r\t") + 1);
        token.erase(0, token.find_first_not_of(" \n\r\t"));
        values.push_back(token);
    }

    for (size_t i = 0; i < values.size(); ++i) {
        size_t mode = titles.size();
        std::string key = titles[i%mode];
        column_map_[key].push_back(values[i]);
    }

    for (const auto &pair : column_map_) {
        cout << pair.first << ": ";
        for (const auto &val : pair.second) {
            cout << val << " ";
        }
        cout << endl;
    }
}

bool Table::IsRightInsertInfo() {
    /**
     * 通过比较config.json文件和column_map_判断插入信息是否正确
     *  - 类型是否正确
     *  - title是否存在
     */
    std::ifstream config_file(config_name_.str());
    json config_json = json::parse(config_file);
    for (const auto &item : column_map_) {
        if (!config_json.contains(item.first)) { // 检查title是否在原表中存在
            std::cerr << "This column name: '" << item.first
                      << "' don't belong to the table: " << table_name_ << endl;
            return false;
        }
        // 检查输入数据类型
        std::string type = config_json[item.first];
        if (type == "INT") {
            for (const auto& value : item.second) {
                try {
                    int num = std::stoi(value);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: " << e.what() << std::endl;
                    return false;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Out of range: " << e.what() << std::endl;
                    return false;
                }
            }
        } else if (type.find("VARCHAR") != std::string::npos) {
            // 正则表达式获取配置文件中VARCHAR的长度
            std::regex regex_pattern(R"(\((\d+)\))");
            std::smatch match;
            if (std::regex_search(type, match, regex_pattern)) {
                if (match.size() == 2) {
                    // 取收个匹配到的对象
                    std::string matched_number = match[1];
                    try {
                        int number = std::stoi(matched_number);
                        std::cout << "Matched number: " << number << std::endl;
                        // 检查所有插入的内容是否超过预定义的长度
                        for (const auto& value : item.second) {
                            if (value.size() > number) {
                                std::cerr << "insert value: " << value
                                        << " exceed max length:" << number << endl;
                                return false;
                            }
                        }
                    } catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid argument: " << e.what() << std::endl;
                        return false;
                    } catch (const std::out_of_range& e) {
                        std::cerr << "Out of range: " << e.what() << std::endl;
                        return false;
                    }
                }
            } else {
                std::cout << "No match found." << std::endl;
            }
        } else if (type == "FLOAT") {
            for (const auto& value : item.second) {
                try {
                    float num = std::stof(value);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: " << e.what() << std::endl;
                    return false;
                } catch (const std::out_of_range& e) {
                    std::cerr << "Out of range: " << e.what() << std::endl;
                    return false;
                }
            }
        }
    }
    return true;
}

void Table::InsertValues() {
    std::ofstream table(table_path_, std::ios::app);
    // 构造一个insert本次追加的内容字符串
    std::ostringstream content;
    std::ifstream config_file(config_name_.str());
    json config_json = json::parse(config_file);
    auto it = column_map_.begin();
    // cout << it->second.size() << endl;
    for (size_t i = 0; i < it->second.size(); ++i) { // 遍历插入的条数
        for (auto &item : config_json.items()) {
            std::string key = item.key();
            if (column_map_.find(key) == column_map_.end()) { // 写入第i条到content
                content << " ,"; 
            } else {
                content << column_map_[key][i] << ",";
            }
        }
        content << endl; // 写入endl
    }
    table << content.str() << endl;
    cout << "insert values" << endl;
}

bool Table::Insert() {
    /**
     * 分割column_info_为titles keyword 和 values
     * 将titles和values存入map，检查map无误后写入table
     */
    // 检查Insert是否在开头
    cout << "insert---------" << endl;
    Split(); // 分割
    CollectInfo();
    if (operator_ != "INSERT" || !success_ || !IsRightInsertInfo()) {
        return false;
    }
    InsertValues();
    return true;
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
        // std::cerr << "Table name: '" << table_name_
        //         << "' is not standardized." << endl;
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