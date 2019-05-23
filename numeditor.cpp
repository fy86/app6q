#include "numeditor.h"

numEditor::numEditor(QObject *parent) :
    QObject(parent)
{
    m_nPSK = objPara::Mod_8psk34;

    paraType=myqt::paraTypeInt64;
    paraName = myqt::paraNameRxFreq;

    m_step64 = 1;
    m_posCursor = 0;
    m_stepDouble = 1.;

    m_max64=14999999999;
    m_min64=14000000000;

    m_maxPos = 10;
    m_minPos = 0;

    for(int i=0;i<20;i++) m_ArrayRate[i]=1<<i;
    m_maxIdxRate = 12;
    m_minIdxRate = 4;
    m_nIdxRate = 10;

    for(int i=0;i<32;i++)m_pnRate0[i]=-1;
    m_pnRate0[0]=32;
    m_pnRate0[1]=64;
    m_pnRate0[2]=128;
    m_pnRate0[3]=256;
    m_pnRate0[4]=384;
    m_pnRate0[5]=512;
    m_pnRate0[6]=768;
    m_pnRate0[7]=1024;
    m_pnRate0[8]=1536;
    m_pnRate0[9]=2048;
    m_pnRate0[10]=3072;
    m_pnRate0[11]=4096;
    m_pnRate0[12]=6144;
    m_pnRate0[13]=8192;
    m_pnRate0[14]=12288;
    m_pnRate0[15]=16384;


}
void numEditor::setArrayRate(objPara::enumPara psk)
{
    int i;
    for(i=0;i<32;i++)m_pnRate[i]=-1;
    switch(psk){
    case objPara::Mod_qpsk12:
        for(i=0;i<16;i++)m_pnRate[i]=m_pnRate0[i];
        break;
    case objPara::Mod_qpsk34:
    case objPara::Mod_8psk12:
        for(i=0;i<16;i++)m_pnRate[i]=m_pnRate0[i]+(m_pnRate0[i]>>1);
        break;
    default:// mod_8psk34
        for(i=0;i<16;i++)m_pnRate[i]=(m_pnRate0[i]<<1)+(m_pnRate0[i]>>2);
        break;
    }

}
void numEditor::incRatePSK(int one)
{
    bool f=false;
    if(one>0){
        for(int i=0;i<16;i++){
            if(m_pnRate[i]>m_num){
                m_num = m_pnRate[i];
                f=true;
                break;
            }
        }
        if(!f){
            m_num = m_pnRate[0];
        }
    }
    else{
        for(int i=15;i>=0;i--){
            if(m_pnRate[i]<m_num){
                m_num = m_pnRate[i];
                f=true;
                break;
            }
        }
        if(!f) m_num = m_pnRate[15];
    }
}

void numEditor::setPSK(objPara::enumPara psk)
{
    m_nPSK = psk;
}
void numEditor::pskInc(int one)
{
    if(one>0){
    switch (m_nPSK) {
    case objPara::Mod_qpsk12:
        m_nPSK = objPara::Mod_8psk12;
        break;
    case objPara::Mod_8psk12:
        m_nPSK = objPara::Mod_qpsk34;
        break;
    case objPara::Mod_qpsk34:
        m_nPSK = objPara::Mod_8psk34;
        break;
    default:
        m_nPSK = objPara::Mod_qpsk12;
        break;
    }
    }
    else{
        switch (m_nPSK) {
        case objPara::Mod_qpsk12:
            m_nPSK = objPara::Mod_8psk34;
            break;
        case objPara::Mod_8psk12:
            m_nPSK = objPara::Mod_qpsk12;
            break;
        case objPara::Mod_qpsk34:
            m_nPSK = objPara::Mod_8psk12;
            break;
        default:
            m_nPSK = objPara::Mod_qpsk34;
            break;
        }
    }
}

void numEditor::setIdxRate(int rate)
{
    int i;
    for(i=m_minIdxRate;i<=m_maxIdxRate;i++){
        if(m_ArrayRate[i]>=rate){
            m_nIdxRate = i;
            break;
        }
    }
}

void numEditor::checkCursor()
{
    setLen();
    if(m_maxPos>=m_nLenInt) m_maxPos = m_nLenInt-1;
    if(m_minPos>=m_nLenInt) m_minPos = m_nLenInt-1;
    if(m_posCursor>m_maxPos) m_posCursor=m_maxPos;
    setStep();
}
void numEditor::incIdx()
{
    m_nIdxRate++;
    if(m_nIdxRate>m_maxIdxRate) m_nIdxRate = m_minIdxRate;
}
void numEditor::decIdx()
{
    m_nIdxRate--;
    if(m_nIdxRate<m_minIdxRate) m_nIdxRate = m_maxIdxRate;
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
void numEditor::strInc()
{
    qint64 i,j;
    i = m_i64/(10*m_step64) * (10*m_step64);
    j = m_i64%(10*m_step64);
    j = (j+m_step64)%(10*m_step64);
    m_i64 = i+j;
}
void numEditor::strDec()
{
    qint64 i,j;
    i = m_i64/(10*m_step64) * (10*m_step64);
    j = m_i64%(10*m_step64);
    j = (j-m_step64+10*m_step64)%(10*m_step64);
    m_i64 = i+j;

}

void numEditor::setStep()
{
    int i;
    m_step64 = 1;
    for(i=0;i<m_posCursor;i++){
        m_step64 *= 10;
    }
    qDebug(" step: %lld",m_step64);
}
void numEditor::strMoveCursor(int n)
{

    m_posCursor = modn(m_posCursor+n,m_nLenInt);
    setStep();
}
// freq MHz
void numEditor::moveCursor(int n)
{
    if(n>0){// +1
        m_posCursor++;
        if(m_posCursor>m_maxPos) m_posCursor = m_minPos;
    }
    else if(n<0){ // -1
        m_posCursor--;
        if(m_posCursor<m_minPos) m_posCursor = m_maxPos;
    }


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

    if(m_min64>0){
        ret = s.size()-1;
    }
    else{//
        ret = m_posCursor;
        if(ret>1)ret++;
    }

    //qDebug(" getCursor: %d",ret);
    return ret;

}
// xxxx.xxxx MHz
// m_i64 Hz
int numEditor::getCursorMHz()
{
    int ret=0;
    //const QLocale & locale = QLocale::c();
    //QString s=locale.toString(m_step64);
    QString s=QString("%1").arg(m_step64);

    if(m_i64>=0){
        ret = s.size()-3;
        if(ret<0)ret=0;
        if(ret>3)ret++;
    }
    return ret;

}
int numEditor::strGetCursor()
{
    if(m_posCursor>3)return 1+m_posCursor;
    else return m_posCursor;
}
void numEditor::setLen()
{
    m_nLenInt = QString("%1").arg(m_max64).size();
    if(m_i64<0) m_nLenInt--;
}

void numEditor::setNum64(qint64 i64, qint64 max64, qint64 min64, int posMax, int posMin, int pos)
{
    paraType = myqt::paraTypeInt64;

    //qDebug(" ===================== qint64.size : %s %d",str.toLatin1().data(),str.size());

    m_i64old = i64;
    m_i64 = i64;
    m_max64 = max64;
    m_min64 = min64;

    setLen();

    m_maxPos = modn(posMax,m_nLenInt);
    m_minPos = modn(posMin,m_nLenInt);
    m_posCursor = modn(pos,m_nLenInt);
    qDebug("cursor:%d max:%d min:%d",m_posCursor,m_maxPos,m_minPos);

    setStep();
}
void numEditor::setNumStr(qint64 i64,qint64 max64,qint64 min64, int posMax, int posMin, int pos)
{
    m_i64 = i64 % 100000000;
    m_i64old = m_i64;

    m_nLenInt=8;

    m_posCursor = modn(pos,m_nLenInt);

    m_max64 = max64;
    m_min64 = min64;
    m_maxPos = modn(posMax,m_nLenInt);
    m_minPos = modn(posMin,m_nLenInt);
    setStep();
}


QString numEditor::getStr643()
{
    QString s=QString("%1").arg(m_i64);
    for(;;){
        if(s.size()>m_posCursor)break;
        s.insert(0,'0');
    }

    int n=(s.size()-1)/3;
    int i;
    for(i=0;i<n;i++){
        s.insert(0,' ');
    }
    return s;
}
void numEditor::getStrFreq(char *p)
{
    double d=0.000001 * m_i64;
    sprintf(p,"%09.4f",d);
}
void numEditor::getStrTDM(char *p)
{
    double d=0.000001 * m_i64;
    sprintf(p,"%010.4f",d);
}

void numEditor::add(int n)
{
    m_i64 +=n;
    if(m_i64>m_max64) m_i64 = m_max64;
    if(m_i64<m_min64) m_i64 = m_min64;
}
