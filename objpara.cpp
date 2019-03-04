#include "objpara.h"

objPara::objPara(QObject *parent) :
    QObject(parent)
{
    m_TxFreq    = 14123456789;
    m_maxTxFreq = 14500000000;
    m_minTxFreq = 10000000000;
    m_RxFreq    = 14123456789;
    m_maxRxFreq = 14500000000;
    m_minRxFreq = 10000000000;

    m_TxRate = 1024;
    m_maxTxRate = 4096;
    m_minTxRate = 32;
    m_RxRate = 1024;
    m_maxRxRate = 4096;
    m_minRxRate = 32;

    m_power100 = -2000;
    m_maxPower = -1500;
    m_minPower = -4000;
}
