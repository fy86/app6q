#include "objbase.h"

objBase::objBase(QObject *parent) :
    QObject(parent)
{
    m_pUTF8 = QTextCodec::codecForName("utf-8");
    m_pGBK = QTextCodec::codecForName("GBK");
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
