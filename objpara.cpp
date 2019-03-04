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

}
