#include "objpage.h"

objPage::objPage(QObject *parent) :
    objFB(parent)
{
    m_devMode = dev_mode_bridge;
    m_workMode = work_mode_central;
    m_bStatConnect = false;
    m_bStatLogin = false;
    m_nSelectMenu10 =0;// central
    setSelectMenu10();
}

void objPage::setSelectMenu10()
{
    if(m_bStatLogin){
        m_bEnableMenu10[2]=false;
        m_bEnableMenu10[3]=true;
        if(m_bStatConnect){
            m_bEnableMenu10[0]=false;
            m_bEnableMenu10[1]=true;
        }
        else{
            m_bEnableMenu10[0]=true;
            m_bEnableMenu10[1]=false;
        }
    }
    else{
        m_bEnableMenu10[0]=false;
        m_bEnableMenu10[1]=false;
        m_bEnableMenu10[2]=true;
        m_bEnableMenu10[3]=false;
    }
    if(m_nSelectMenu10<0 || m_nSelectMenu10>3)m_nSelectMenu10=0;
    while(!m_bEnableMenu10[m_nSelectMenu10]){
        m_nSelectMenu10 = (m_nSelectMenu10+1) & 0x3;
    }
}
