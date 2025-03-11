#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QSplitter>
#include <QListWidget>


#include "common.h"

class ChatWindow : public QMainWindow {
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    void setChatUI();
    void connectSignals();
    void addNavItems(QListWidget *navList);
    

private slots:
    void sendMessage();

private:
    QLineEdit *inputBox;
    QPushButton *sendButton;
    
};

#endif