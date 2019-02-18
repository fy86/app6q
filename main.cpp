#include <QCoreApplication>
#include <QUdpSocket>
#include <QTextCodec>

#include <objfb.h>
#include <threadspi.h>


void testGBK()
{

}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    threadSPI threadspi;
    objFB fb;

    QObject::connect(&fb,SIGNAL(sigFlush()),&threadspi,SLOT(slotSend()));
    QObject::connect(&fb,SIGNAL(sigQ2(char,char)),&threadspi,SLOT(slotSend2(char,char)));

    threadspi.start();

    fb.demo16();

    QUdpSocket udp;

    qDebug("hello");

    return a.exec();
}
