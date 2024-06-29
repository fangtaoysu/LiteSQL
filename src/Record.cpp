#include "../include/Record.h"

using std::cout;
using std::endl;
namespace fs = std::experimental::filesystem;

Record::Record(const std::string& cmd, const std::string& DATABASE)
    : cmd_(cmd), db_path_(DATABASE), success_(true) {
}

Record::~Record() {
    
}

bool Record::Select() {
    /**
     * 1. 重新定位读入的信息，并检查命令的位置
     * 2. 读取配置文件，获取每列的长度
     * 3. 根据列长度打印符号
     */
    std::string select_part;
    ExtractSQLComponents(select_part);
    if (!success_) {
        return false;
    }
    // 读取配置文件，获取每列长度
    std::ifstream config_file(config_path_);
    json config_json = json::parse(config_file);
    std::vector<bool> column_indices = ConstructColumnIndices(select_part, config_json);
    if (column_indices.empty()) { // 构造失败
        return success_;
    }
    std::vector<size_t> max_lengths;
    std::string wire = ConstructWire(config_json, column_indices, max_lengths);
    if (wire == "+") { // 构造失败
        return success_;
    }
    PrintFormat(column_indices, max_lengths, wire);
    return success_;
}

void Record::ExtractSQLComponents(std::string& select_part) {
    /**
     * 提取列信息和表名
     */
    std::string from_part;
    size_t select_pos = cmd_.find("SELECT");
    size_t from_pos = cmd_.find("FROM");

    // cout << select_pos << "\t" << from_pos << endl;
    if (select_pos == std::string::npos || from_pos == std::string::npos || select_pos >= from_pos) {
        std::cerr << "Invalid query format" << std::endl;
        success_ = false;
        return;
    }
    // 提取子串
    select_part = cmd_.substr(select_pos + 6, from_pos - (select_pos + 6));
    from_part = cmd_.substr(from_pos + 4);
    Split(select_part);
    Split(from_part);
    
    config_path_ = db_path_ + "/config/" + from_part + "_config.json";
    table_path_ = db_path_ + "/" + from_part + ".csv";
    if (!fs::exists(table_path_)) {
        std::cerr << "table: '" << from_part
                  << "' isn't exists." << endl;
    }
}

std::vector<bool> Record::ConstructColumnIndices(const std::string &select_column, json &config_json) {
    /**
     * 构造命令选择的列索引vector
     */
    std::vector<bool> column_indices;
    std::ifstream table_file(table_path_);
    if (!table_file.is_open()) {
        std::cerr << "Failed to open the file: " << table_path_ << std::endl;
        success_ = false;
        return column_indices;
    }
    std::string header_line;
    
    // 读标题行
    if (std::getline(table_file, header_line)) {
        std::istringstream ss(header_line);
        std::string column_name;
        // 遍历每个列标题
        while (std::getline(ss, column_name, ',')) {
            bool in_tag = false;
            std::string token;
            if (select_column == "*") { // 如果是*， 全部选择
                column_indices.push_back(true);
                continue;
            }
            // 将标题行和输入的信息一一比较
            std::istringstream iss_column(select_column);
            while (std::getline(iss_column, token, ',')) {
                Split(token);
                // cout << "token: '" << token
                //      << "'" << endl;
                // cout << "column_name: '" << column_name
                //      << "'" << endl;
                if (column_name == token) {
                    in_tag = true;
                    break;
                }
            }
            column_indices.push_back(in_tag);
        }
    } else {
        std::cerr << "Failed to read the header line from the file: " << table_path_ << std::endl;
        success_ = false;
        return column_indices;
    }
    int total = std::accumulate(column_indices.begin(), column_indices.end(), 0);
    if (total == 0) {
        std::cerr << "cant find these column: " << select_column << endl;
        success_ = false;
        return column_indices;
    }
    return column_indices;
}

std::string Record::ConstructWire(json &config_json, const std::vector<bool> &column_indices, std::vector<std::size_t> &max_lengths) {
    /**
     * 构造打印信息的分割线，并收集打印列的最大长度
     */
    int i = 0;
    // 根据最大长度构造wire
    std::string wire("+");
    for (const auto &[key, value] : config_json.items()) {
        if (value.contains("max_length")) {
            size_t val = value["max_length"];
            max_lengths.push_back(val);
            if (column_indices[i]) { // 如果包含这一列
                std::string symbol(val+2, '-');
                wire.append(symbol + "+");
            }
        } else {
            std::cerr << "config file: '" << config_path_
                      << "' read failed, can't found max_length." << endl;
            success_ = false;
            break;
        }
        ++i;
    }
    // cout << wire << endl;
    return wire;
}

void Record::PrintFormat(const std::vector<bool> &column_indices, const std::vector<size_t> &max_lengths, const std::string &wire) {
    /**
     * 格式化打印信息
     */
    std::string line;
    std::ifstream table_file(table_path_);
    while (std::getline(table_file, line)) {
        std::string value, print_line("|");
        std::istringstream iss(line);
        int i = 0;
        while (std::getline(iss, value, ',')) {
            if (!column_indices[i]) {
                ++i;
                continue;
            }
            if (i < max_lengths.size()) {
                size_t space_length = max_lengths[i] - value.size() + 1;
                std::string left_space(space_length, ' '); // Construct the padding spaces
                std::string left = " " + value + left_space + "|";
                print_line.append(left);
            } else {
                std::cerr << "Index out of range: max_lengths" << std::endl;
                return;
            }
            ++i;
        }
        if (print_line != "|") {
            cout << wire << endl;
            cout << print_line << endl;
        }
    }
    cout << wire << endl;
}