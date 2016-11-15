#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>
//#ifdef _DEBUG
//#include "vld.h"
//#endif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath("./plugins");

    QTextCodec *codec = QTextCodec::codecForName("System");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    MainWindow w;
    w.show();
    return a.exec();
}
