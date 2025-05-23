#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QTextEdit>
#include <QSplitter>
#include <QListWidget>
#include <QDateTime>

#include "common.h"
#include "customWidget.h"
#include "chatWindow.h"

class HomePage : public QMainWindow {
    Q_OBJECT

public:
    HomePage(QWidget *parent = nullptr);
    ~HomePage();

private:
    void setHomePage();
    void setNavBarList();
    void setChatList();
    void setContactList();
    void setNavBarStackedWidget();
    void setChatWindowStackedWidget();
    
    void addNavBarItems();
    void addChatListItems();
    void addContactListItems();
    QString truncateText(const QString &text, int maxWidth, const QFont &font);
    void connectSignals();

private:
    QSplitter *splitter;
    QListWidget *navBar;
    QListWidget *chatList;
    QListWidget *contactList;
    QStackedWidget *chatListStacked;
    QStackedWidget *chatWindowStacked;
    
};

#endif