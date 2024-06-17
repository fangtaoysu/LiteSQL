# 安装

```cpp
1. **Prerequisites**
    - Ensure you have CMake installed. You can download it from [cmake.org](https://cmake.org/).
    - Ensure you have a C++ compiler (e.g., GCC).

2. **Clone the Repository**
    ```sh
    git clone https://github.com/yourusername/yourdatabase.git
    cd yourdatabase
```

3. **Build the Project**

   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

```
## 启动服务

1. **Start the Server**
    
    ```sh
    ./yourdatabase_server
```

## 使用数据库

### Connecting to the Database

- Connect to the database using the provided client application:

  ```sh
  ./yourdatabase_client
  ```

### Creating a Database

- To create a new database:

  ```sql
  CREATE DATABASE mydatabase;
  ```

### Creating a Table

- To create a new table in a database:

  ```sql
  CREATE TABLE mytable (
      id INT PRIMARY KEY,
      name VARCHAR(100),
      age INT
  );
  ```

### Inserting Data

- To insert data into a table:

  ```sql
  INSERT INTO mytable (id, name, age) VALUES (1, 'Alice', 30);
  ```

### Querying Data

- To query data from a table:

  ```sql
  SELECT * FROM mytable WHERE age > 25;
  ```

### Updating Data

- To update data in a table:

  ```sql
  UPDATE mytable SET age = 31 WHERE id = 1;
  ```

### Deleting Data

- To delete data from a table:

  ```sql
  DELETE FROM mytable WHERE id = 1;
  ```

### Transactions

- To use transactions:

  ```sql
  BEGIN TRANSACTION;
  INSERT INTO mytable (id, name, age) VALUES (2, 'Bob', 25);
  COMMIT;
  ```

### Additional Commands

- For more commands and usage examples, refer to the full documentation in the `docs` directory.

## 项目目录结构

```cpp

```