#include "objpara.h"

objPara::objPara(QObject *parent) :
    QObject(parent)
{
    m_freqOffset = 0;

    m_vRate = 36;

    m_cwDevID = "cwdev_id: ";

    m_strCallID="190072";

    m_testFreq = 1200000000;// hz
    m_testPwr = -4500;//dbm
    m_testON = false;

    m_callCMD = CMD_DONOTHING;

    m_cuMode = CUState_unknown;
    m_cuNetState = CUState_unknown;
    m_cuUsageState = CUState_unknown;
    m_sessionState = SESSIONstate_closed;

    m_recvSync = false;

    m_swBUC10m=1;// unknown   1:on -1:off
    m_swBUCpwr=1;
    m_swLNBpwr=1;//
    m_swLNB10m = 1;

    //////             000   000
    m_localFreq    = 13050000000;// hz
    m_maxLocalFreq =  1000000000;
    m_minLocalFreq = 99000000000;

    m_fSNR = 0.;
    m_TxFreq    =  1200000000;
    m_maxTxFreq =  2150000000;// 9.9 GHz
    m_minTxFreq =   950000000;// 900 MHz
    m_RxFreq    =  1200000000;
    m_maxRxFreq =  2150000000;
    m_minRxFreq =   950000000;

    //m_BUCfreq = 13050000000;//13g
    //m_LNBfreq = 11300000000;// 13g
    m_BUCfreq = 1000;//13g
    m_LNBfreq = 1000;// 13g

    m_txRate = 1152;
    m_maxTxRate = 4096;
    m_minTxRate = 16;
    m_rxRate = 1152;
    m_maxRxRate = 4096;
    m_minRxRate = 16;

    m_txRateC = 1024;
    m_rxRateC = 1024;


    m_power100 = -1100;
    m_maxPower = -1100;
    m_minPower = -4500;

    /////////////////////////////////////////////////////////////////////////////////////
    m_TDMfreq1   = 0;
    m_TDMfreq2   = 0;
    m_maxTDMfreq = 14500000000;
    m_minTDMfreq = 00000000000;

    strcpy(m_pNumber,"12345678");
    m_number = 12345678;

    //m_TxRateCentral = 2048;
    //m_maxTxRateCentral = 4096;
    //m_minTxRateCentral = 16;
    //m_RxRateCentral = 2048;
    //m_maxRxRateCentral = 4096;
    //m_minRxRateCentral = 16;

    m_power100Central = 0;// dB
    m_maxPowerCentral = 40;
    m_minPowerCentral = -40;

    m_devMode = DevMode_bridge;

    m_workMode = Mode_central;// default

    m_status = Status_idle;

    m_startSecs = QDateTime::currentDateTime().toTime_t();

    m_rxPSK = Mod_8psk34;
    m_txPSK = Mod_8psk34;

    //loadv();
    load();
#if 0
    int i,j,k;
    for(i=0;i<16;i++){
        qDebug("%d %lld %lld %lld %lld",i,getRate(i,0,0),getRate(i,1,0),getRate(i,0,1),getRate(i,1,1));
    }
#endif
    for(int i=0;i<32;i++)m_pnRate0[i]=-1;
    m_pnRate0[0]=32;
    m_pnRate0[1]=64;
    m_pnRate0[2]=128;
    m_pnRate0[3]=256;
    m_pnRate0[4]=384;
    m_pnRate0[5]=512;
    m_pnRate0[6]=768;
    m_pnRate0[7]=1024;
    m_pnRate0[8]=1536;
    m_pnRate0[9]=2048;
    m_pnRate0[10]=3072;
    m_pnRate0[11]=4096;
    m_pnRate0[12]=6144;
    m_pnRate0[13]=8192;
    m_pnRate0[14]=12288;
    m_pnRate0[15]=16384;
}
void objPara::setArrayRate(enumPara psk)
{
    int i;
    for(i=0;i<32;i++)m_pnRate[i]=-1;
    switch(psk){
    case enumPara::Mod_qpsk12:
        for(i=0;i<16;i++)m_pnRate[i]=m_pnRate0[i];
        break;
    case enumPara::Mod_qpsk34:
    case enumPara::Mod_8psk12:
        for(i=0;i<16;i++)m_pnRate[i]=m_pnRate0[i]+(m_pnRate0[i]>>1);
        break;
    default:// mod_8psk34
        for(i=0;i<16;i++)m_pnRate[i]=(m_pnRate0[i]<<1)+(m_pnRate0[i]>>2);
        break;
    }
}

// type:0:qpsk::1:8psk   r:0:1/2::1::3/4
qint64 objPara::getRate(int sn, int type, int r)
{
    int ret;
    qint64 ret64;
    QList<int> r0;
    r0 << 32 << 64 <<128 << 256 << 384 << 512 << 768 << 1024 << 1536 << 2048 << 3072 << 4096 << 6144 << 8192 << 12288 << 16384;
    if(sn<0) ret=r0[0];
    else if(sn>=r0.size()) ret=r0[r0.size()-1];
    else ret=r0[sn];
    switch(type){
    case 0:
        ret=ret<<1;
        break;
    default:
        ret=(ret<<1)+ret;
        break;
    }
    switch(r){
    case 0:
        ret=ret>>1;
        break;
    default:
        ret=(ret<<1)+ret;
        ret=ret>>2;
        break;
    }
    ret64=ret;
    return ret64;

}

QString objPara::strTXrate()
{
    QString str="发送速率:  -- k";
    if(m_txRate<0) return str;
    //if(m_TxRate<=m_maxTxRate && m_TxRate>=m_minTxRate){
        str = QString("发送速率: %1k").arg(m_txRate);
    //}
    return str;
}
QString objPara::strRXrate()
{
    QString str="接收速率:  -- k";
    if(m_rxRate<0) return str;

    //if(m_RxRate<=m_maxRxRate && m_RxRate>=m_minRxRate){
        str = QString("接收速率: %1k").arg(m_rxRate);
    //}
    return str;
}
QString objPara::strTXrateC()
{
    QString str="发送速率:  -- k";
    if(m_txRateC<0) return str;
    //if(m_TxRate<=m_maxTxRate && m_TxRate>=m_minTxRate){
        str = QString("发送速率: %1k").arg(m_txRateC);
    //}
    return str;
}
QString objPara::strRXrateC()
{
    QString str="接收速率:  -- k";
    if(m_rxRateC<0) return str;

    //if(m_RxRate<=m_maxRxRate && m_RxRate>=m_minRxRate){
        str = QString("接收速率: %1k").arg(m_rxRateC);
    //}
    return str;
}
void objPara::loadv()
{
    QFile fcmdline("/opt/satcs/conf/rixi_config.json");
    if(fcmdline.open(QIODevice::ReadOnly)){
        //qDebug("    open.ok  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx load >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ");
        QTextStream in(&fcmdline);
        while(!in.atEnd()){
            QString line=in.readLine();
            //qDebug(" line : %s",line.toLatin1().data());
            if(line.contains("hardware_version")){
                if(line.contains("3")){
                    m_vRate = 4;
                }
                else m_vRate = 36;
                break;
            }
        }
        fcmdline.close();
    }

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
            if(0==sl.at(0).compare(QString("testfreq"),Qt::CaseInsensitive)){
                m_testFreq=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("testpwr"),Qt::CaseInsensitive)){
                m_testPwr=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("txfreq_p2p"),Qt::CaseInsensitive)){
                m_TxFreq=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("rxfreq_p2p"),Qt::CaseInsensitive)){
                m_RxFreq=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("txrate_p2p"),Qt::CaseInsensitive)){
                m_txRate=sl.at(1).toInt(&b);
            }
            else if(0==sl.at(0).compare(QString("rxrate_p2p"),Qt::CaseInsensitive)){
                m_rxRate=sl.at(1).toInt(&b);
            }

            else if(0==sl.at(0).compare(QString("txrate_c"),Qt::CaseInsensitive)){
                m_txRateC=sl.at(1).toInt(&b);
            }
            else if(0==sl.at(0).compare(QString("rxrate_c"),Qt::CaseInsensitive)){
                m_rxRateC=sl.at(1).toInt(&b);
            }

            else if(0==sl.at(0).compare(QString("power_p2p"),Qt::CaseInsensitive)){
                m_power100=sl.at(1).toLongLong(&b);
            }
#if 0
            else if(0==sl.at(0).compare(QString("bucfreq_p2p"),Qt::CaseInsensitive)){
                m_BUCfreq=sl.at(1).toLongLong(&b);
            }
            else if(0==sl.at(0).compare(QString("lnbfreq_p2p"),Qt::CaseInsensitive)){
                m_LNBfreq=sl.at(1).toLongLong(&b);
            }
#endif
            else if(0==sl.at(0).compare(QString("devmode_p2p"),Qt::CaseInsensitive)){
                if(0==sl.at(1).compare(QString("bridge"),Qt::CaseInsensitive)){
                    m_devMode = objPara::DevMode_bridge;
                }
                else {
                    m_devMode = objPara::DevMode_router;
                }
            }
            else if(0==sl.at(0).compare(QString("txPSK"),Qt::CaseInsensitive)){
                m_txPSK=(enumPara)sl.at(1).toInt(&b);
            }
            else if(0==sl.at(0).compare(QString("rxPSK"),Qt::CaseInsensitive)){
                m_rxPSK=(enumPara)sl.at(1).toInt(&b);
            }
            else if(0==sl.at(0).compare(QString("callid"),Qt::CaseInsensitive)){
                m_strCallID = sl.at(1);
            }
            else if(0==sl.at(0).compare(QString("workmode"),Qt::CaseInsensitive)){
                if(0==sl.at(1).compare(QString("p2p"),Qt::CaseInsensitive)){
                    m_workMode = objPara::Mode_p2p;
                }
                else {
                    m_workMode = objPara::Mode_central;
                }
            }
        }
        f.close();
    }
    //qDebug("    XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx load >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ");

    QFile fcmdline("/proc/cmdline");
    if(fcmdline.open(QIODevice::ReadOnly)){
        //qDebug("    open.ok  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx load >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ");
        QTextStream in(&fcmdline);
        //while(!in.atEnd()){
            QString line=in.readLine();
            //qDebug(" line : %s",line.toLatin1().data());
            QStringList sl=line.split(' ',QString::SkipEmptyParts,Qt::CaseInsensitive);
            int len=sl.size();
            for(int i=0;i<len;i++){
                //qDebug("cwdev_id:::::::::::::::::::::             %s",sl.at(i).toLatin1().data());
                if(sl.at(i).contains("cwdev_id")){
                    m_cwDevID = sl.at(i);
                    m_cwDevID.replace(QString("="),QString(": "));
                    break;
                }
            }
        //}
        fcmdline.close();
    }
}

void objPara::save()
{
    char buf[100];
    QFile f("/root/qt/para.txt");
    if(f.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        sprintf(buf,"testfreq %lld\n",m_testFreq);
        f.write(buf,strlen(buf));
        sprintf(buf,"testpwr %d\n",m_testPwr);
        f.write(buf,strlen(buf));

        sprintf(buf,"txfreq_p2p %lld\n",m_TxFreq);
        f.write(buf,strlen(buf));
        sprintf(buf,"rxfreq_p2p %lld\n",m_RxFreq);
        f.write(buf,strlen(buf));
        sprintf(buf,"txrate_p2p %d\n",m_txRate);
        f.write(buf,strlen(buf));
        sprintf(buf,"rxrate_p2p %d\n",m_rxRate);
        f.write(buf,strlen(buf));

        sprintf(buf,"txrate_c %d\n",m_txRateC);
        f.write(buf,strlen(buf));
        sprintf(buf,"rxrate_c %d\n",m_rxRateC);
        f.write(buf,strlen(buf));

        sprintf(buf,"power_p2p %lld\n",m_power100);
        f.write(buf,strlen(buf));
#if 0
        sprintf(buf,"bucfreq %lld\n",m_BUCfreq);
        f.write(buf,strlen(buf));
        sprintf(buf,"lnbfreq %lld\n",m_LNBfreq);
        f.write(buf,strlen(buf));
#endif
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
        if(m_workMode==objPara::Mode_p2p){
            sprintf(buf,"workmode p2p\n");
            f.write(buf,strlen(buf));
        }
        else{
            sprintf(buf,"workmode central\n");
            f.write(buf,strlen(buf));
        }
        sprintf(buf,"txPSK %d\n",m_txPSK);
        f.write(buf,strlen(buf));
        sprintf(buf,"rxPSK %d\n",m_rxPSK);
        f.write(buf,strlen(buf));
        sprintf(buf,"callid %s\n",m_strCallID.toLatin1().data());
        f.write(buf,strlen(buf));

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


