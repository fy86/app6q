#include "numeditor.h"

numEditor::numEditor(QObject *parent) :
    QObject(parent)
{
    paraType=myqt::paraTypeInt64;
    paraName = myqt::paraNameRxFreq;

    m_step64 = 1;
    m_posCursor = 0;
    m_stepDouble = 1.;

    m_max64=14999999999;
    m_min64=14000000000;

    m_maxPos = 10;
    m_minPos = 0;
}
void numEditor::checkCursor()
{
    m_nLenInt = QString("%1").arg(m_i64).size();
    if(m_maxPos>=m_nLenInt) m_maxPos = m_nLenInt-1;
    if(m_minPos>=m_nLenInt) m_minPos = m_nLenInt-1;
    if(m_posCursor>m_maxPos) m_posCursor=m_maxPos;
    setStep();
}

void numEditor::inc()
{
    m_i64 += m_step64;
    if(m_i64>m_max64) m_i64 =m_max64;
    checkCursor();
}
void numEditor::dec()
{
    m_i64 -= m_step64;
    if(m_i64<m_min64) m_i64 = m_min64;
    checkCursor();

}

void numEditor::setStep()
{
    const QLocale & locale = QLocale::c();
    QString s;

    int i;
    switch(paraType){
    case myqt::paraTypeInt64:
        m_step64 = 1;
        for(i=0;i<m_posCursor;i++){
            m_step64 *= 10;
        }
        s=locale.toString(m_step64);
        //qDebug(" step64(%d): %s",m_posCursor,s.toLatin1().data());
        break;
    default:
        break;
    }
}
void numEditor::moveCursor(int n)
{

    m_posCursor = modn(m_posCursor+n,m_nLenInt);
    setStep();
}
int numEditor::modn(int n, int nmax)
{
    int ret=n;
    while(ret<0)ret+=nmax;
    ret %= nmax;
    return ret;
}
int numEditor::getCursor()
{
    int ret;
    const QLocale & locale = QLocale::c();
    QString s=locale.toString(m_step64);
    ret=s.size()-1;

    //qDebug(" getCursor: %d",ret);
    return ret;

}

void numEditor::setNum64(qint64 i64, qint64 max64, qint64 min64, int posMax, int posMin, int pos)
{
    paraType = myqt::paraTypeInt64;

    //qDebug(" ===================== qint64.size : %s %d",str.toLatin1().data(),str.size());

    m_i64old = i64;
    m_i64 = i64;

    //QString str=QString("%1").arg(m_i64);
    m_nLenInt = QString("%1").arg(m_i64).size();

    m_posCursor = modn(pos,m_nLenInt);

    m_max64 = max64;
    m_min64 = min64;
    m_maxPos = modn(posMax,m_nLenInt);
    m_minPos = modn(posMin,m_nLenInt);
    setStep();
#if 0
    qDebug(" set64 i64:%s %s %s - posMax:%d posMin:%d pos:%d - cursor:%d",
           QString("%1").arg(m_i64).toLatin1().data(),
           QString("%1").arg(m_max64).toLatin1().data(),
           QString("%1").arg(m_min64).toLatin1().data(),
           m_maxPos,m_minPos,m_posCursor,getCursor());
#endif
}


