
#include <QApplication>
#include <host.h>



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Host host;
    return a.exec();
}
