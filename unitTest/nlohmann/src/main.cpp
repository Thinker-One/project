#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    // 创建JSON对象
    json employee = {
        {"name", "John Doe"},
        {"age", 35},
        {"position", "Senior Developer"},
        {"skills", {"C++", "Python", "JavaScript"}},
        {"contact", {
            {"email", "john.doe@example.com"},
            {"phone", "123-456-7890"}
        }},
        {"projects", {
            {
                {"name", "Project A"},
                {"duration", 6},
                {"completed", true}
            },
            {
                {"name", "Project B"},
                {"duration", 3},
                {"completed", false}
            }
        }}
    };

    // 修改数据
    employee["age"] = 36;
    employee["skills"].push_back("Java");
    employee["contact"]["address"] = "123 Main St, New York";

    // 序列化并输出
    std::cout << employee.dump(4) << std::endl;

    // 写入文件
    std::ofstream o("employee.json");
    o << std::setw(4) << employee << std::endl;

    // 从文件读取
    std::ifstream i("employee.json");
    json employee_copy;
    i >> employee_copy;

    // 访问数据
    std::cout << "Name: " << employee_copy["name"] << std::endl;
    std::cout << "First skill: " << employee_copy["skills"][0] << std::endl;

    return 0;
}