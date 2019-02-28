#include <QCoreApplication>
#include <QUdpSocket>
#include <QTextCodec>

#include <objfb.h>
#include <threadspi.h>
#include <threadkey.h>

#include "objui.h"

void testGBK()
{
    QString stra="啊a";
    QTextCodec *gbk=QTextCodec::codecForName("gbk");
    QTextCodec *utf8=QTextCodec::codecForName("utf-8");
    QString str8=utf8->toUnicode(stra.toLocal8Bit().data());
    QByteArray baGBK=gbk->fromUnicode(str8);
    int len=baGBK.size();
    int i;
    qDebug("size %d",len);
    for(i=0;i<len;i++)qDebug("%02x",0x0ff & baGBK.at(i));

}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    threadSPI threadspi;
    threadkey key0;
    threadkey key1;
    //objFB fb;

    objui fb;

    QObject::connect(&fb,SIGNAL(sigFlush()),&threadspi,SLOT(slotSend()));
    QObject::connect(&fb,SIGNAL(sigQ2(char,char)),&threadspi,SLOT(slotSend2(char,char)));

    QObject::connect(&key1,SIGNAL(sigKey(int)),&fb,SLOT(slotKey(int)));

    threadspi.start();

    //fb.demo16();

    key1.openDev("/dev/input/event1");
    key1.start();

    QUdpSocket udp;

    qDebug("hello");

    fb.OLED_Init();

    //testGBK();
    //fb.testbase();
    //fb.strXY(QString("啊在上面的搜索框"),20,20);

    //fb.show0001();
    //fb.buildMachine();


    fb.initMachine();

    return a.exec();
}
