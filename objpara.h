#ifndef OBJPARA_H
#define OBJPARA_H

#include <QObject>

class objPara : public QObject
{
    Q_OBJECT
public:
    explicit objPara(QObject *parent = 0);

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

signals:

public slots:

};

#endif // OBJPARA_H
