#include "statuspara.h"

statuspara::statuspara(QObject *parent) :
    QObject(parent)
{
    m_enableUpdate = true;
}

void statuspara::update64(qint64 i64)
{
    if(m_enableUpdate) m_v64 = i64;
}

void statuspara::enableUpdate(bool en)
{
    m_enableUpdate = en;
}
