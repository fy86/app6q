#include "editorratec.h"

editorRateC::editorRateC(QObject *parent) :
    QObject(parent)
{
    m_listRate.append(64);
    m_listRate.append(128);
    m_listRate.append(256);
    m_listRate.append(512);
    m_listRate.append(768);
    m_listRate.append(1024);
    m_listRate.append(2048);
    m_listRate.append(3072);
    m_listRate.append(4096);
    m_listRate.append(5120);
    m_listRate.append(6144);
    m_listRate.append(7168);
    m_listRate.append(8192);
    m_listRate.append(9216);
    m_listRate.append(10240);

    m_nLen = m_listRate.size();

    m_vRate = 36;
    loadv();
}
void editorRateC::loadv()
{
    QFile fcmdline("/opt/satcs/conf/rixi_config.json");
    if(fcmdline.open(QIODevice::ReadOnly)){
        //qDebug("    open.ok  XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXxxx load >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ");
        QTextStream in(&fcmdline);
        while(!in.atEnd()){
            QString line=in.readLine();
            //qDebug(" line : %s",line.toLatin1().data());
            if(line.contains("hardware_version")){
                if(line.contains("3")){
                    m_vRate = 4;
                }
                else m_vRate = 36;
                break;
            }
        }
        fcmdline.close();
    }

}

int editorRateC::setRate(int rate)
{
    m_nIdx = m_nLen-1;

    for(int i=0;i<m_nLen;i++){
        if(m_listRate.at(i)>=rate){
            m_nIdx = i;
            break;
        }
    }

    return m_listRate.at(m_nIdx);
}

int editorRateC::getRate()
{
    return m_listRate.at(m_nIdx);
}
bool editorRateC::isValidRate()
{
    bool ret=true;

    if(m_vRate==4){
        if(getRate()>4096) ret = false;
    }

    return ret;

}

void editorRateC::inc()
{
    m_nIdx++;
    if(m_nIdx>=m_nLen) m_nIdx = m_nLen-1;

    if(!isValidRate()){
        m_nIdx--;
        if(m_nIdx<0) m_nIdx=0;
    }


}

void editorRateC::dec()
{
    m_nIdx--;
    if(m_nIdx<0) m_nIdx=0;

}
