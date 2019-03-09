#ifndef MYQT_H
#define MYQT_H

#include <QObject>

class myqt : public QObject
{
    Q_OBJECT
public:
    explicit myqt(QObject *parent = 0);

    enum appConst{
        paraTypeInt,
        paraTypeDouble,
        paraTypeInt64,

        paraNameTxFreq,
        paraNameRxFreq,
        paraNameTxRate,
        paraNameRxRate,
    };

signals:

public slots:


};

#endif // MYQT_H
