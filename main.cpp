#include <QtGui/QApplication>
#include "pdistanz.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PDistanz w;
    w.show();
    
    return a.exec();
}
