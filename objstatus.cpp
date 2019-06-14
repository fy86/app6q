#include "objstatus.h"

objstatus::objstatus(QObject *parent) :
    objPara(parent)
{

    setInvalid();
    //
    m_status=objPara::Status_idle;  // use m_para.m_status
    m_workMode=objPara::Mode_p2p;

    m_swBUC10m=0;// unknown   1:on -1:off
    m_swBUCpwr=0;
    m_swLNBpwr=0;//
    m_swLNB10m = 0;
}

void objstatus::load()
{

}
void objstatus::save()
{

}

void objstatus::setInvalid()
{
    m_TxFreq = -1;
    m_RxFreq = -1;
    m_TxRate = -1;
    m_RxRate = -1;

    m_power100 = 1;

    m_fSNR = 0.0;
}
#if 0
QString objstatus::strTxRate()
{
    QString str="发送速率:  -- k";
    if(m_TxRate<0) return str;
    //if(m_TxRate<=m_maxTxRate && m_TxRate>=m_minTxRate){
        str = QString("发送速率: %1 k").arg(m_TxRate);
    //}
    return str;
}
QString objstatus::strRxRate()
{
    QString str="接收速率:  -- k";
    if(m_RxRate<0) return str;

    //if(m_RxRate<=m_maxRxRate && m_RxRate>=m_minRxRate){
        str = QString("接收速率: %1 k").arg(m_RxRate);
    //}
    return str;
}
#endif
#if 0
QString objstatus::strTxFreq()
{
    char buf[40];
    QString str=" -- MHz";
    if(m_TxFreq<0) return str;

    //if(m_TxFreq>m_maxTxFreq || m_TxFreq<m_minTxFreq) return str;

    //const QLocale & locale = QLocale::c();
    //str=locale.toString(m_TxFreq).replace(',',' ') + " Hz";
    sprintf(buf,"%.4f MHz",0.000001 * m_TxFreq);
    str=QString(buf);

    return str;
}
#endif
#if 0
QString objstatus::strTxFreq(qint64 lfreq)
{
    qint64 f64=m_TxFreq-lfreq;
    char buf[40];
    QString str=" -- MHz";
    if(f64<0) return str;

    //if(m_TxFreq>m_maxTxFreq || m_TxFreq<m_minTxFreq) return str;

    //const QLocale & locale = QLocale::c();
    //str=locale.toString(m_TxFreq).replace(',',' ') + " Hz";
    sprintf(buf,"%.4f MHz",0.000001 * f64);
    str=QString(buf);

    return str;
}
#endif
#if 0
QString objstatus::strRxFreq()
{
    char buf[40];
    QString str=" -- MHz";
    if(m_RxFreq<0) return str;

    //if(m_RxFreq>m_maxRxFreq || m_RxFreq<m_minRxFreq) return str;

    //const QLocale & locale = QLocale::c();
    //str=locale.toString(m_RxFreq).replace(',',' ') + " Hz";
    sprintf(buf,"%.4f MHz",0.000001 * m_RxFreq);
    str=QString(buf);

    return str;
}
#endif
#if 0
QString objstatus::strRxFreq(qint64 lfreq)
{
    qint64 f64=m_RxFreq-lfreq;
    char buf[40];
    QString str=" -- MHz";
    if(f64<0) return str;

    //if(m_RxFreq>m_maxRxFreq || m_RxFreq<m_minRxFreq) return str;

    //const QLocale & locale = QLocale::c();
    //str=locale.toString(m_RxFreq).replace(',',' ') + " Hz";
    sprintf(buf,"%.4f MHz",0.000001 * f64);
    str=QString(buf);

    return str;
}
#endif
