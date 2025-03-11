#include "../include/chatWindow.h"

ChatWindow::ChatWindow(QWidget *parent) : QMainWindow(parent) {

    setChatUI();
    connectSignals();
}

ChatWindow::~ChatWindow() {

}

void ChatWindow::setChatUI() {

    setWindowTitle("CHAT");
    resize(910, 600);

    QSplitter *splitter = new QSplitter(this);
    setCentralWidget(splitter);

    // ListWidgetSize: 60 = (5 + 5) * 2 + 40;   网格大小: 40;   itemSize: 35 = 5 * 2 + 25;  图片大小: 25;
    QListWidget *navBar = new QListWidget(splitter);
    navBar->setFixedWidth(60);
    navBar->setGridSize(QSize(40, 40));
    navBar->setIconSize(QSize(25, 25));
    navBar->setSpacing(0);
    navBar->setViewMode(QListView::IconMode);
    navBar->setMovement(QListView::Static);
    navBar->setStyleSheet(R"(
        QListWidget {
            background: #2e2e2e;
            border: none;
            outline: none;
            padding: 5px;
            margin: 5px;
        }
        QListWidget::item {
            background: #2e2e2e;
            padding: 5px;
            margin: 0;
        }
        QListWidget::item:selected {
            background: #07c160;
            border-radius: 5px;
        }
    )");
    addNavItems(navBar);

    QWidget *chatWidget = new QWidget(splitter);
    QVBoxLayout *chatLayout = new QVBoxLayout(chatWidget);

    QTextEdit *chatContent = new QTextEdit(chatWidget);
    chatContent->setReadOnly(true);
    chatLayout->addWidget(chatContent);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputBox = new QLineEdit(chatWidget);
    sendButton = new QPushButton("发送", chatWidget);
    inputLayout->addWidget(inputBox);
    inputLayout->addWidget(sendButton);
    chatLayout->addLayout(inputLayout);
    
}

void ChatWindow::connectSignals() {

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);
    connect(inputBox, &QLineEdit::returnPressed, this, &ChatWindow::sendMessage);

}

void ChatWindow::addNavItems(QListWidget *navBar) {

    QListWidgetItem *chatItem = new QListWidgetItem(QIcon(":/icons/chat.svg"), "", navBar);
    QListWidgetItem *ContactsItem = new QListWidgetItem(QIcon(":/icons/contacts.png"), "", navBar);
    QListWidgetItem *collectItem = new QListWidgetItem(QIcon(":/icons/collect.png"), "", navBar);
    QListWidgetItem *chatFilesItem = new QListWidgetItem(QIcon(":/icons/chatFiles.png"), "", navBar);
    QListWidgetItem *friendsCircleItem = new QListWidgetItem(QIcon(":/icons/friendsCircle.png"), "", navBar);
    QListWidgetItem *videoNumItem = new QListWidgetItem(QIcon(":/icons/video.png"), "", navBar);
    QListWidgetItem *takeALookItem = new QListWidgetItem(QIcon(":/icons/look.png"), "", navBar);
    QListWidgetItem *searchItem = new QListWidgetItem(QIcon(":/icons/search.png"), "", navBar);

    chatItem->setSizeHint(QSize(35, 35));
    ContactsItem->setSizeHint(QSize(35, 35));
    collectItem->setSizeHint(QSize(35, 35));
    chatFilesItem->setSizeHint(QSize(35, 35));
    friendsCircleItem->setSizeHint(QSize(35, 35));
    videoNumItem->setSizeHint(QSize(35, 35));
    takeALookItem->setSizeHint(QSize(35, 35));
    searchItem->setSizeHint(QSize(35, 35));

    chatItem->setToolTip("聊天");
    ContactsItem->setToolTip("通讯录");
    collectItem->setToolTip("收藏");
    chatFilesItem->setToolTip("聊天文件");
    friendsCircleItem->setToolTip("朋友圈");
    videoNumItem->setToolTip("视频号");
    takeALookItem->setToolTip("看一看");
    searchItem->setToolTip("搜一搜");
}

void ChatWindow::sendMessage() {
    qDebug() << "发送消息";
}