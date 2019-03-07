#ifndef THREADSPI_H
#define THREADSPI_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QSemaphore>
#include <QByteArray>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

class threadSPI : public QThread
{
    Q_OBJECT
public:
    explicit threadSPI(QObject *parent = 0);

    QQueue<char> m_q;// spi send
    QSemaphore m_sem;
    QByteArray m_baSend;// <256

    int m_fdSPI;
    QByteArray m_baDev;

    char m_SPImode;
    char m_SPIbits;// = 9;
    int m_SPIspeed;// = 16000000;
    short m_SPIdelay;

    QByteArray m_baTx;
    QByteArray m_baRx;

    struct spi_ioc_transfer m_tr;


    int openSPI();
    void transfer(QByteArray baSend);

protected:
    virtual void run();

signals:
    void sigReady();

public slots:
    void slotSend();// semaphore release
    void slotSend2(char ch0,char ch1);// enqueue

};

#endif // THREADSPI_H
