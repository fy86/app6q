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

struct st_numEditor {
    enum myqt::appConst paraName;
    enum myqt::appConst paraType;
    qint64 i64;
    int num;
    float f;
    double d;
    int left;
    int top;
    int right;
    int bottom;
    int posCursor;
    int stepInt;
    double stepDouble;

};

#endif // MYQT_H
