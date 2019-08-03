#ifndef NUMEDITOR_H
#define NUMEDITOR_H

#include <QObject>
#include <QLocale>


#include "myqt.h"
#include "objpara.h"

class numEditor : public QObject
{
    Q_OBJECT
public:
    explicit numEditor(QObject *parent = 0);

    int m_vRate;
    void loadv();
    int getVrate(int r);
    bool isValidRate();

    enum objPara::enumPara m_nPSK;

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
    void setNumStr(qint64 i64,qint64 max64,qint64 min64,int posMax,int posMin=0,int pos=0);
    void setStep();
    void inc();
    void dec();
    void moveCursor(int n);

    int modn(int n,int nmax);
    int getCursor();
    int getCursorMHz();
    void checkCursor();

    void setLen();

    void strInc();
    void strDec();
    void strMoveCursor(int n);
    int strGetCursor();

    QString getStr643();
    void getStrFreq(char *p);
    void getStrTDM(char *p);

    int m_ArrayRate[20];
    int m_maxIdxRate;
    int m_minIdxRate;
    int m_nIdxRate;

    void setIdxRate(int rate);
    void incIdx();
    void decIdx();

    void add(int n);

    void setPSK(objPara::enumPara psk);
    void pskInc(int one=1);

    void incRatePSK(int one);

    int m_pnRate[64];
    int m_pnRate0[32];

    void setArrayRate(objPara::enumPara psk);


signals:

public slots:

};

#endif // NUMEDITOR_H
