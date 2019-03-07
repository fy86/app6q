#ifndef OBJPARA_H
#define OBJPARA_H

#include <QObject>
#include <QDateTime>

class objPara : public QObject
{
    Q_OBJECT
public:
    explicit objPara(QObject *parent = 0);

    bool m_bIdle;
    bool m_bConnected;

    qint64 m_TxFreq;
    qint64 m_RxFreq;
    int m_TxRate;
    int m_RxRate;

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

    qint64 m_TDMfreq;
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

    bool m_bDevModeBridge;
    bool m_bDevModeRouter;

    bool m_bModeP2P;
    bool m_bModeCentral;


    qint64 m_startSecs;
signals:

public slots:

};

#endif // OBJPARA_H
