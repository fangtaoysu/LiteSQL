# LiteSQL

用C++实现一个轻量级的类MySql数据库
> 更详细的项目介绍在docs文件夹下的文档中

## 使用

```shell
# 1. git clone repo
git clone git@github.com:fangtaoysu/LiteSQL.git

# 2. build programming 
mkdir build
cd build
make

# 3. run
./main
```

Then, you will use this database in terminus;😉😉

## 命令（已实现）

```mysql
CREATE DATABASE db_name;
DROP DATABASE db_name;
USE db_name;

CREATE TABLE table_name;
DROP TABLE table_name;

INSERT INTO table_name (column_names) VALUES (values);
exit
```

## 计划

- [x] 6/18: 构建`main`函数框架
- [x] 6/19: 实现`DATABSET`的增删逻辑
- [x] 6/20: 重构`DATABASE`的增删逻辑的结构
- [x] 6/21-6/23: 实现创建表`CREATE TABLE`
- [x] 6/25: 实现命令以分号结尾
- [x] 6/25: 实现表的删除`DROP TABLE`
- [x] 6/25-27: 实现表的插入`INSERT INTO`
- [ ] 6/27: 实现`SHOW COLUMN FROM table_name`,显示`TABLE`内容
- [ ] 6/28 增加查找功能 `SELECT FROM WHERE`
