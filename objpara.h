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

signals:

public slots:

};

#endif // OBJPARA_H
