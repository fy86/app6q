#ifndef OBJSTATUS_H
#define OBJSTATUS_H

#include <QObject>
#include "objpara.h"

class objstatus : public objPara
{
    Q_OBJECT
public:
    explicit objstatus(QObject *parent = 0);

    virtual void load();
    virtual void save();

    void setInvalid();
#if 0
    QString strTxRate();
    QString strRxRate();

    QString strTxFreq(qint64 lfreq);
    QString strRxFreq(qint64 lfreq);

    QString strTxFreq();
    QString strRxFreq();
#endif

signals:

public slots:

};

#endif // OBJSTATUS_H
