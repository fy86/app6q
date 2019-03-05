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
    m_minTxRate = 16;
    m_RxRate = 1024;
    m_maxRxRate = 4096;
    m_minRxRate = 16;

    m_power100 = -2000;
    m_maxPower = -1500;
    m_minPower = -4000;

    /////////////////////////////////////////////////////////////////////////////////////
    m_TDMfreq    = 14000123456;
    m_maxTDMfreq = 14500000000;
    m_minTDMfreq = 10000000000;

    strcpy(m_pNumber,"12345678");
    m_number = 12345678;

    m_TxRateCentral = 2048;
    m_maxTxRateCentral = 4096;
    m_minTxRateCentral = 16;
    m_RxRateCentral = 2048;
    m_maxRxRateCentral = 4096;
    m_minRxRateCentral = 16;

    m_power100Central = -2222;
    m_maxPowerCentral = -1500;
    m_minPowerCentral = -4000;

    m_bModeP2P = false;
    m_bModeCentral  = true;

}
