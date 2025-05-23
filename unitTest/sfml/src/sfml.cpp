#include "sfml.hpp"

int main() {
    // 创建一个 800x600 的窗口，窗口名称为 "SFML Example"
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Example");

    // 创建一个矩形，大小为 200x150，颜色为绿色
    sf::RectangleShape rectangle(sf::Vector2f(200.f, 150.f));
    rectangle.setFillColor(sf::Color::Green);  // 填充颜色为绿色
    rectangle.setPosition(300.f, 225.f);  // 设置矩形的位置（窗口中央）

    // 主程序循环
    while (window.isOpen()) {
        sf::Event event;
        
        // 处理窗口事件
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();  // 如果关闭窗口按钮被点击，则关闭窗口
            }
        }

        // 清空窗口，设置背景色为黑色
        window.clear(sf::Color::Black);

        // 绘制矩形
        window.draw(rectangle);

        // 显示绘制内容
        window.display();
    }

    return 0;
}