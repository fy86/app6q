#include "objbase.h"

objBase::objBase(QObject *parent) :
    QObject(parent)
{
    m_pUTF8 = QTextCodec::codecForName("utf-8");
    m_pGBK = QTextCodec::codecForName("GBK");
    m_nDebug=0;
    m_semKey.release();
    m_semLog.release();

    m_secs0 = QDateTime::currentDateTime().toTime_t();

}

void objBase::str2GBK(QString str, QByteArray &ba)
{
    QString str8=m_pUTF8->toUnicode(str.toLocal8Bit().data());
    ba=m_pGBK->fromUnicode(str8);

}
void objBase::testbase()
{
    QString stra="啊在上面的搜索框";
    QByteArray ba;//baGBK=gbk->fromUnicode(str8);
    str2GBK(stra,ba);
    int len=ba.size();
    int i;
    qDebug("base size %d",len);
    for(i=0;i<len;i++)qDebug("%02x",0x0ff & ba.at(i));

}

void objBase::writeLog(QByteArray ba)
{
    int i,len;
    QString fn("/dev/shm/log6q.txt");
    QString fnbak("/dev/shm/log6q.bak");

    if(!m_semLog.tryAcquire())return;

    if(m_listLog.size()>400)m_listLog.removeFirst();
    m_listLog.append(QTime::currentTime().toString("hh:mm::ss ").toLatin1() + ba);
    len=m_listLog.size();

    QFile fbak(fnbak);
    fbak.remove();
    QFile f0(fn);
    f0.rename(fnbak);

    QFile f(fn);
    if(f.open(QIODevice::ReadWrite)){
        for(i=0;i<len;i++){
            f.write(m_listLog.at(i));
        }
        f.close();
    }

    m_semLog.release();
}
