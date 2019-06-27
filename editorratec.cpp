#include "editorratec.h"

editorRateC::editorRateC(QObject *parent) :
    QObject(parent)
{
    m_listRate.append(64);
    m_listRate.append(128);
    m_listRate.append(256);
    m_listRate.append(512);
    m_listRate.append(768);
    m_listRate.append(1024);
    m_listRate.append(2048);
    m_listRate.append(3072);
    m_listRate.append(4096);
    m_listRate.append(5120);
    m_listRate.append(6144);
    m_listRate.append(7168);
    m_listRate.append(8192);
    m_listRate.append(9216);
    m_listRate.append(10240);

    m_nLen = m_listRate.size();
}

int editorRateC::setRate(int rate)
{
    m_nIdx = m_nLen-1;

    for(int i=0;i<m_nLen;i++){
        if(m_listRate.at(i)>=rate){
            m_nIdx = i;
            break;
        }
    }

    return m_listRate.at(m_nIdx);
}

int editorRateC::getRate()
{
    return m_listRate.at(m_nIdx);
}

void editorRateC::inc()
{
    m_nIdx++;
    if(m_nIdx>=m_nLen) m_nIdx = m_nLen-1;

}

void editorRateC::dec()
{
    m_nIdx--;
    if(m_nIdx<0) m_nIdx=0;

}
