#include "objpara.h"

objPara::objPara(QObject *parent) :
    QObject(parent)
{
    //////             000   000
    m_localFreq    = 13050000000;// hz
    m_maxLocalFreq =  1000000000;
    m_minLocalFreq = 99000000000;

    m_fSNR = 0.;
    m_TxFreq    =  1200000000;
    m_maxTxFreq =  9999000000;// 9.9 GHz
    m_minTxFreq =   999000000;// 999 MHz
    m_RxFreq    =  1200000000;
    m_maxRxFreq =  9999000000;
    m_minRxFreq =   999000000;

    m_BUCfreq = 13050000000;//13g
    m_LNBfreq = 11300000000;// 13g

    m_TxRate = 1024;
    m_maxTxRate = 4096;
    m_minTxRate = 16;
    m_RxRate = 1024;
    m_maxRxRate = 4096;
    m_minRxRate = 16;

    m_power100 =  8000;
    m_maxPower = 20000;
    m_minPower =-20000;

    /////////////////////////////////////////////////////////////////////////////////////
    m_TDMfreq1   = 0;
    m_TDMfreq2   = 0;
    m_maxTDMfreq = 14500000000;
    m_minTDMfreq = 00000000000;

    strcpy(m_pNumber,"12345678");
    m_number = 12345678;

    m_TxRateCentral = 2048;
    m_maxTxRateCentral = 4096;
    m_minTxRateCentral = 16;
    m_RxRateCentral = 2048;
    m_maxRxRateCentral = 4096;
    m_minRxRateCentral = 16;

    m_power100Central = 0;// dB
    m_maxPowerCentral = 20;
    m_minPowerCentral = -20;

    m_devMode = DevMode_bridge;

    m_workMode = Mode_p2p;

    m_status = Status_idle;

    m_startSecs = QDateTime::currentDateTime().toTime_t();

    load();

}
QString objPara::strTxRate()
{
    QString str="发送速率:  -- k";
    if(m_TxRate<0) return str;
    //if(m_TxRate<=m_maxTxRate && m_TxRate>=m_minTxRate){
        str = QString("发送速率: %1 k").arg(m_TxRate);
    //}
    return str;
}
QString objPara::strRxRate()
{
    QString str="接收速率:  -- k";
    if(m_RxRate<0) return str;

    //if(m_RxRate<=m_maxRxRate && m_RxRate>=m_minRxRate){
        str = QString("接收速率: %1 k").arg(m_RxRate);
    //}
    return str;
}

void objPara::load()
{
    bool b;
    QFile f("/root/qt/para.txt");
    if(f.open(QIODevice::ReadOnly)){
        QTextStream in(&f);
        while(!in.atEnd()){
            QString line=in.readLine();
            QStringList sl=line.split(' ',QString::SkipEmptyParts,Qt::CaseInsensitive);
            if(sl.size()<2)continue;
            if(0==sl.at(0).compare(QString("txfreq_p2p"),Qt::CaseInsensitive)){
                m_TxFreq=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("rxfreq_p2p"),Qt::CaseInsensitive)){
                m_RxFreq=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("txrate_p2p"),Qt::CaseInsensitive)){
                m_TxRate=sl.at(1).toInt(&b);
            }
            else if(0==sl.at(0).compare(QString("rxrate_p2p"),Qt::CaseInsensitive)){
                m_RxRate=sl.at(1).toInt(&b);
            }
            else if(0==sl.at(0).compare(QString("power_p2p"),Qt::CaseInsensitive)){
                m_power100=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("bucfreq_p2p"),Qt::CaseInsensitive)){
                m_BUCfreq=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("lnbfreq_p2p"),Qt::CaseInsensitive)){
                m_LNBfreq=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("devmode_p2p"),Qt::CaseInsensitive)){
                if(0==sl.at(1).compare(QString("bridge"),Qt::CaseInsensitive)){
                    m_devMode = objPara::DevMode_bridge;
                }
                else {
                    m_devMode = objPara::DevMode_router;
                }
            }
        }
        f.close();
    }
}

void objPara::save()
{
    char buf[100];
    QFile f("/root/qt/para.txt");
    if(f.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        sprintf(buf,"txfreq_p2p %lld\n",m_TxFreq);
        f.write(buf,strlen(buf));
        sprintf(buf,"rxfreq_p2p %lld\n",m_RxFreq);
        f.write(buf,strlen(buf));
        sprintf(buf,"txrate_p2p %d\n",m_TxRate);
        f.write(buf,strlen(buf));
        sprintf(buf,"rxrate_p2p %d\n",m_RxRate);
        f.write(buf,strlen(buf));
        sprintf(buf,"power_p2p %lld\n",m_power100);
        f.write(buf,strlen(buf));

        sprintf(buf,"bucfreq %lld\n",m_BUCfreq);
        f.write(buf,strlen(buf));
        sprintf(buf,"lnbfreq %lld\n",m_LNBfreq);
        f.write(buf,strlen(buf));
        switch(m_devMode){
        case objPara::DevMode_bridge:
            sprintf(buf,"devmode_p2p bridge\n");
            f.write(buf,strlen(buf));
            break;
        default:
            sprintf(buf,"devmode_p2p router\n");
            f.write(buf,strlen(buf));
            break;

        }

        f.close();
    }
}

QString objPara::strRxFreq(qint64 lfreq)
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
QString objPara::strTxFreq(qint64 lfreq)
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
QString objPara::strTxFreq()
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

QString objPara::strRxFreq()
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


