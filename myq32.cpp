#include "myq32.h"

#define MYQ_LEN 0x10000
#define MYQ_MASK (MYQ_LEN-1)

myq32::myq32(QObject *parent) :
    QObject(parent)
{
    m_p32 = new int[MYQ_LEN];
    m_nDeq = 0;
    m_nEnq = 0;
}

int myq32::enq(int v32)
{
    int ret=0;

    m_p32[m_nEnq]=v32;
    m_nEnq = MYQ_MASK & (m_nEnq+1);
    return ret;
}


int myq32::deq(int *p32)
{
    int ret=0;

    if(m_nDeq==m_nEnq) return -1;
    *p32 = m_p32[m_nDeq];
    m_nDeq = MYQ_MASK & (m_nDeq +1);

    return ret;
}


int myq32::len()
{
    return (m_nEnq-m_nDeq+MYQ_LEN) & MYQ_MASK;
}

bool myq32::isEmpty()
{
    if(m_nEnq == m_nDeq) return true;
    else return false;
}
