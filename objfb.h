#ifndef OBJFB_H
#define OBJFB_H

#include <QObject>
#include <QByteArray>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


#include <objoled.h>

class objFB : public objOled
{
    Q_OBJECT
public:
    explicit objFB(QObject *parent = 0);

    QByteArray m_ba16;
    QByteArray m_baFB;

    void loaddot();
    void draw1(int x,int y,int c);

    void demo16();
    void zeroFB(char f);
    void showHZdemo();
    void showHZ16(char *p,int x,int y);




signals:

public slots:

};

#endif // OBJFB_H
