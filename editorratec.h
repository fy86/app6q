#ifndef EDITORRATEC_H
#define EDITORRATEC_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QTextStream>

class editorRateC : public QObject
{
    Q_OBJECT
public:
    explicit editorRateC(QObject *parent = 0);

    int m_nRate;
    QList<int> m_listRate;
    int m_nLen;
    int m_nIdx;

    int m_vRate;
    void loadv();
    bool isValidRate();

    int setRate(int rate);
    int getRate();
    void inc();
    void dec();




signals:

public slots:

};

#endif // EDITORRATEC_H
