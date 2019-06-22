#ifndef EDITORRATEC_H
#define EDITORRATEC_H

#include <QObject>
#include <QList>

class editorRateC : public QObject
{
    Q_OBJECT
public:
    explicit editorRateC(QObject *parent = 0);

    int m_nRate;
    QList<int> m_listRate;
    int m_nLen;
    int m_nIdx;

    int setRate(int rate);
    int getRate();
    void inc();
    void dec();


signals:

public slots:

};

#endif // EDITORRATEC_H
