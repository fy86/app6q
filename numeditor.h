#ifndef NUMEDITOR_H
#define NUMEDITOR_H

#include <QObject>
#include <QLocale>


#include "myqt.h"

class numEditor : public QObject
{
    Q_OBJECT
public:
    explicit numEditor(QObject *parent = 0);

    enum myqt::appConst paraName;
    enum myqt::appConst paraType;
    qint64 m_i64;
    qint64 m_i64old;
    qint64 m_max64;
    qint64 m_min64;

    int m_nLenInt;
    int m_nLenDouble;
    int m_num;
    float m_f;
    double m_d;
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;

    int m_posCursor;
    int m_maxPos;
    int m_minPos;

    qint64 m_step64;
    double m_stepDouble;

    void setNum64(qint64 i64,qint64 max64,qint64 min64,int posMax,int posMin=0,int pos=0);
    void setStep();
    void inc();
    void dec();
    void moveCursor(int n);

    int modn(int n,int nmax);
    int getCursor();
    void checkCursor();

signals:

public slots:

};

#endif // NUMEDITOR_H
