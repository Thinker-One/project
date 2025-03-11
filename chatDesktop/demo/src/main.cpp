#include "../include/loginWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QScopedPointer<LoginWindow> lgw(new LoginWindow);
    lgw->login(app);
    qDebug() << app.styleSheet();
    return app.exec();
}