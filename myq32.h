#ifndef MYQ32_H
#define MYQ32_H

#include <QObject>

class myq32 : public QObject
{
    Q_OBJECT
public:
    explicit myq32(QObject *parent = 0);

    int *m_p32;
    int m_nDeq;
    int m_nEnq;

    int enq(int v32);
    int deq(int *p32);

    int len();
    bool isEmpty();

signals:

public slots:

};

#endif // MYQ32_H
