#include <QCoreApplication>

#include "sthrm2.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    STHrm2 *hrm=new STHrm2();
    hrm->start();

    return a.exec();
}
