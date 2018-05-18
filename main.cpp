#include <QCoreApplication>
#include "pswemulator.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    PswEmulator* pswEmulator = new PswEmulator();
    PswEmulator pswEmulator;

    return a.exec();
}
