#include "threadspi.h"

threadSPI::threadSPI(QObject *parent) :
    QThread(parent)
{
    m_baDev.append("/dev/spidev2.0");
    m_fdSPI = -1;
    m_SPIbits = 9;
    m_SPIspeed = 4000000;
    m_SPImode = 0;
    m_SPIdelay = 0;

    m_baTx.resize(300);
    m_baRx.resize(300);

}

void threadSPI::run()
{
    int i32;
    char *p=(char*)(&i32);
    char ch;
    int ret;
    int len32;

    ret=openSPI();
    if(ret<0) return;
    qDebug(" thread.spi emit sigReady");
    emit sigReady();

    for(;;){
        if(!m_myq32.isEmpty()){
        //if(!m_q32.isEmpty()){
        //if(m_q.size()>3){
            //i32 = m_q32.dequeue();
            m_myq32.deq(&i32);
            m_baSend.append(p,4);
#if 0
            ch = m_q.dequeue();
            m_baSend.append(ch);
            ch = m_q.dequeue();
            m_baSend.append(ch);
            ch = m_q.dequeue();
            m_baSend.append(ch);
            ch = m_q.dequeue();
            m_baSend.append(ch);
#endif
            len32 = m_baSend.size();
            if( (len32>=240) && (len32==((len32>>2)<<2)) ){
                if(m_fdSPI>0){
                    transfer(m_baSend);
                }
                m_baSend.clear();
            }
            continue;
        }
        else{
            if(m_baSend.size()>0){
                if(m_fdSPI>0){
                    transfer(m_baSend);
                }
                m_baSend.clear();
            }
        }

        m_sem.acquire();
    }

}
void threadSPI::slotSend()
{
    m_sem.release();
}
void threadSPI::slotSend2(char ch0,char cd)
{
    int i;
    i = 0x0ff & ch0;
    i |= 0x0ff00 & (cd<<8);
    m_myq32.enq(i);
    //m_q32.enqueue(i);
#if 0
    char z=0;
    m_q.enqueue(ch0);
    m_q.enqueue(cd);
    m_q.enqueue(z);
    m_q.enqueue(z);
    //m_sem.release();
#endif
}

int threadSPI::openSPI()
{
    int ret = 0;

    //parse_opts(argc, argv);

    m_fdSPI = open(m_baDev.data(), O_RDWR);
    if (m_fdSPI < 0){
        ret=-1;
        return ret;
    }

    /*
     * spi mode
     */
    ret = ioctl(m_fdSPI, SPI_IOC_WR_MODE, &m_SPImode);
    if (ret == -1) return -2;

    ret = ioctl(m_fdSPI, SPI_IOC_RD_MODE, &m_SPImode);
    if (ret == -1) return -3;

    /*
     * bits per word
     */
    ret = ioctl(m_fdSPI, SPI_IOC_WR_BITS_PER_WORD, &m_SPIbits);
    if (ret == -1) return -4;

    ret = ioctl(m_fdSPI, SPI_IOC_RD_BITS_PER_WORD, &m_SPIbits);
    if (ret == -1) return -5;

    /*
     * max speed hz
     */
    ret = ioctl(m_fdSPI, SPI_IOC_WR_MAX_SPEED_HZ, &m_SPIspeed);
    if (ret == -1) return -6;

    ret = ioctl(m_fdSPI, SPI_IOC_RD_MAX_SPEED_HZ, &m_SPIspeed);
    if (ret == -1) return -7;

    //printf("spi mode: %d\n", mode);
    //printf("bits per word: %d\n", bits);
    //printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

    //transfer();
    //demo16();

    //close(gfd);

    return ret;

}

void threadSPI::transfer(QByteArray baSend)
{
    int ret;
    qint64 buf64[64];
#if 0
    uint8_t tx[] = {
         0xFF, 0x00, 0xFF, 0x00, 0xFF,
        0x00, 0xFF, 0x00,
    };
#endif
#if 0

        0x40, 0x00, 0x00, 0x00, 0x00, 0x95,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xAD,
        0xF0, 0x0D,
    };
#endif
#if 0
    uint8_t rx[ARRAY_SIZE(tx)] = {0, };
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = ARRAY_SIZE(tx),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };
#endif
    m_baTx.clear();
    m_baTx.append(baSend);
    memcpy((void*)buf64,baSend.data(),baSend.size());

    m_tr.tx_buf=(unsigned long)buf64;
    m_tr.rx_buf=(unsigned long)m_baRx.data();
    m_tr.len = baSend.size();
    m_tr.delay_usecs = m_SPIdelay;
    m_tr.speed_hz = m_SPIspeed;
    m_tr.bits_per_word = m_SPIbits;

    ret = ioctl(m_fdSPI, SPI_IOC_MESSAGE(1), &m_tr);
    if (ret < 1) return;
#if 0
    for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
        if (!(ret % 6))
            puts("");
        printf("%.2X ", rx[ret]);
    }
    puts("");
#endif

}

