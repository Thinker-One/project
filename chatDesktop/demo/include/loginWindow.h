#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QApplication>

#include "chatWindow.h"

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void setupUI();
    void connectSignals();

private:
    QLabel *titleLb;
    QLabel *userNameLb;
    QLabel *passwdLb;

    QLineEdit *userNameEd;
    QLineEdit *passwdEd;

    QPushButton *loginBt;
    QPushButton *exitBt;
    QPushButton *registerBt;
};

#endif