#include <QApplication>

#include "view.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    new MainView();
    return QApplication::exec();
}
