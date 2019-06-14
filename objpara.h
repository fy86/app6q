#ifndef OBJPARA_H
#define OBJPARA_H

#include <QObject>
#include <QDateTime>
#include <QLocale>
#include <QFile>
#include <QTextStream>
#include <QStringList>

class objPara : public QObject
{
    Q_OBJECT
public:
    explicit objPara(QObject *parent = 0);
    virtual void load();
    virtual void save();

    enum enumPara{
        Status_idle,
        Status_connecting,
        Status_callSuccess,
        Status_connected,
        Status_disconnecting,
        Status_waiting_tdm,
        Status_logining,
        Status_logouting,
        Status_online_idle,
        Status_online_p2p_call,
        Status_online_ncc_plan,

        Mode_p2p,
        Mode_central,
        Mode_unknown,

        DevMode_bridge,
        DevMode_router,

        Mod_qpsk12,
        Mod_qpsk34,
        Mod_8psk12,
        Mod_8psk34
    };

    int m_swBUCpwr;
    int m_swLNBpwr;
    int m_swBUC10m;
    int m_swLNB10m;

    enumPara m_rxPSK;
    enumPara m_txPSK;

    enumPara m_status;// idle , connecting , connected
    enumPara m_devMode;// bridge, router
    enumPara m_workMode;// p2p , central

    qint64 m_localFreq;
    qint64 m_maxLocalFreq;
    qint64 m_minLocalFreq;

    qint64 m_TxFreq;
    qint64 m_RxFreq;

    qint64 m_BUCfreq;
    qint64 m_LNBfreq;

    int m_TxRate;// kHz ?
    int m_RxRate;
    int m_pnRate[64];
    int m_pnRate0[32];

    qint64 m_maxTxFreq;
    qint64 m_minTxFreq;
    qint64 m_maxRxFreq;
    qint64 m_minRxFreq;

    qint64 m_maxTxRate;
    qint64 m_minTxRate;
    qint64 m_maxRxRate;
    qint64 m_minRxRate;

    qint64 m_power100;
    qint64 m_maxPower;
    qint64 m_minPower;

    qint64 m_TDMfreq1;
    qint64 m_TDMfreq2;
    qint64 m_maxTDMfreq;
    qint64 m_minTDMfreq;

    char m_pNumber[20];
    qint64 m_number;
    int m_nLenNumber;

    int m_TxRateCentral;
    int m_RxRateCentral;
    qint64 m_maxTxRateCentral;
    qint64 m_minTxRateCentral;
    qint64 m_maxRxRateCentral;
    qint64 m_minRxRateCentral;

    qint64 m_power100Central;
    qint64 m_maxPowerCentral;
    qint64 m_minPowerCentral;

    //bool m_bDevModeBridge;
    //bool m_bDevModeRouter;

    //bool m_bModeP2P;
    //bool m_bModeCentral;


    qint64 m_startSecs;

    bool m_bEnableMenuCtrl[4];// central
    bool m_bEnableMenuCtrlP2P[2];

    float m_fSNR;
#if 0
    QString strTxFreq(qint64 lfreq);
    QString strRxFreq(qint64 lfreq);
#endif
    QString strTxRate();
    QString strRxRate();

    QString strTxFreq(qint64 lfreq);
    QString strRxFreq(qint64 lfreq);

    QString strTxFreq();
    QString strRxFreq();

    qint64 getRate(int sn,int type,int r);

    void setArrayRate(enumPara psk);

signals:

public slots:

};

#endif // OBJPARA_H
