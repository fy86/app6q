#ifndef OBJOLED_H
#define OBJOLED_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QHostAddress>

#include <objbase.h>
#include <unistd.h>


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define	OLED_Shift		0x1C
#define OLED_Max_Column	0x3F			// 256/4-1
#define OLED_Max_Row		0x3F			// 64-1
#define	OLED_Brightness	0x0F


class objOled : public objBase
{
    Q_OBJECT
public:
    explicit objOled(QObject *parent = 0);

    QHostAddress m_hostVOLED;
    QUdpSocket m_udp;
    QByteArray m_ba;

    //void OLED_Init();
    void setRES(int res);
    void Set_Command_Lock(unsigned char d);
    void Set_Display_On_Off(unsigned char d);
    void Write_Command(unsigned char Data);
    void Write_Data(unsigned char Data);
    void Set_Display_Clock(unsigned char d);
    void Set_Multiplex_Ratio(unsigned char d);
    void Set_Display_Offset(unsigned char d);
    void Set_Start_Line(unsigned char d);
    void Set_Remap_Format(unsigned char d);
    void Set_GPIO(unsigned char d);
    void Set_Function_Selection(unsigned char d);
    void Set_Display_Enhancement_A(unsigned char a, unsigned char b);
    void Set_Master_Current(unsigned char d);
    void Set_Gray_Scale_Table();
    void Set_Phase_Length(unsigned char d);
    void Set_Display_Enhancement_B(unsigned char d);
    void Set_Precharge_Voltage(unsigned char d);
    void Set_Precharge_Period(unsigned char d);
    void Set_VCOMH(unsigned char d);
    void Set_Display_Mode(unsigned char d);
    void Set_Partial_Display(unsigned char a, unsigned char b, unsigned char c);
    void Fill_RAM(unsigned char Data);
    void Set_Column_Address(unsigned char a, unsigned char b);
    void Set_Row_Address(unsigned char a, unsigned char b);
    void Set_Write_RAM();
    void Set_Contrast_Current(unsigned char d);

    void Fill_BlockP(unsigned char *p, unsigned char a, unsigned char b, unsigned char c, unsigned char d);


    void testGray();
    void logo();

signals:
    void sigQ2(char,char);
    void sigFlush();

    void sigReadyOled();

public slots:
    void slotFlush();// public sigFlush

    virtual void OLED_Init();


};

#endif // OBJOLED_H
