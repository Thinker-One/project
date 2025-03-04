#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>

class ChatWindow : public QMainWindow {
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    void setupUI();

};

#endif