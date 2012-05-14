#include <QtGui/QApplication>
#include <QtGui/QFont>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QFont font("wenquanyi", 12, QFont::Normal);
    QApplication app(argc, argv);
    app.setFont(font);
    Dialog w;
    w.show();
    return app.exec();
}
