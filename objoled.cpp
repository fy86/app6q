#include "objoled.h"

objOled::objOled(QObject *parent) :
    objBase(parent)
{
}
void objOled::slotFlush()
{
    emit sigFlush();
}

void objOled::Fill_BlockP(unsigned char *p, unsigned char a, unsigned char b, unsigned char c, unsigned char d)
{
    unsigned char i,j;

        Set_Column_Address(OLED_Shift+a,OLED_Shift+b);
        Set_Row_Address(c,d);
        Set_Write_RAM();

        for(i=0;i<(d-c+1);i++)
        {
            for(j=0;j<(b-a+1);j++)
            {
                Write_Data(*p++);
                Write_Data(*p++);
            }
        }

}

void objOled::Write_Command(unsigned char Data)
{
    char ch1,ch2;
    ch1 = Data;
    //m_q.enqueue(ch);
    ch2 = 0;
    emit sigQ2(ch1,ch2);
    //m_q.enqueue(ch);
    //transfer01(Data,0);

}
void objOled::Write_Data(unsigned char Data)
{
    //printf("./spidev_test_oled -b 9 -0 %02x -1 ff\n",Data);
    char ch1,ch2;
    ch1 = Data;
    //m_q.enqueue(ch);
    ch2 = 0x0ff;
    emit sigQ2(ch1,ch2);
    //m_q.enqueue(ch);
    //transfer01(Data,0x0ff);

}

void objOled::Set_Display_On_Off(unsigned char d)
{
    Write_Command(d);			// Set Display On/Off
                        //   Default => 0xAE
                        //     0xAE (0x00) => Display Off (Sleep Mode On)
                        //     0xAF (0x01) => Display On (Sleep Mode Off)

}

void objOled::Set_Command_Lock(unsigned char d)
{
    Write_Command(0xFD);			// Set Command Lock
    Write_Data(d);				//   Default => 0x12
                        //     0x12 => Driver IC interface is unlocked from entering command.
                        //     0x16 => All Commands are locked except 0xFD.

}
void objOled::Set_Display_Clock(unsigned char d)
{
    Write_Command(0xB3);			// Set Display Clock Divider / Oscillator Frequency
    Write_Data(d);				//   Default => 0xD0
                        //     A[3:0] => Display Clock Divider
                        //     A[7:4] => Oscillator Frequency

}

void objOled::Set_Multiplex_Ratio(unsigned char d)
{
    Write_Command(0xCA);			// Set Multiplex Ratio
    Write_Data(d);				//   Default => 0x7F (1/128 Duty)

}

void objOled::Set_Display_Offset(unsigned char d)
{
    Write_Command(0xA2);			// Set Vertical Scroll by Row
    Write_Data(d);				//   Default => 0x00

}

void objOled::Set_Start_Line(unsigned char d)
{
    Write_Command(0xA1);			// Set Vertical Scroll by RAM
    Write_Data(d);				//   Default => 0x00

}
void objOled::Set_Remap_Format(unsigned char d)
{
    Write_Command(0xA0);			// Set Re-Map / Dual COM Line Mode
    Write_Data(d);				//   Default => 0x40
                        //     Horizontal Address Increment
                        //     Column Address 0 Mapped to SEG0
                        //     Disable Nibble Remap
                        //     Scan from COM0 to COM[N-1]
                        //     Disable COM Split Odd Even
    Write_Data(0x11);			//   Default => 0x01 (Disable Dual COM Mode)

}

void objOled::setRES(int res)
{

}
void objOled::Set_GPIO(unsigned char d)
{
    Write_Command(0xB5);			// General Purpose IO
    Write_Data(d);				//   Default => 0x0A (GPIO Pins output Low Level.)

}
void objOled::Set_Function_Selection(unsigned char d)
{
    Write_Command(0xAB);			// Function Selection
    Write_Data(d);				//   Default => 0x01
                        //     Enable Internal VDD Regulator

}
void objOled::Set_Display_Enhancement_A(unsigned char a, unsigned char b)
{
    Write_Command(0xB4);			// Display Enhancement
    Write_Data(a);				//   Default => 0xA2
                        //     0xA0 => Enable External VSL
                        //     0xA2 => Enable Internal VSL (Kept VSL Pin N.C.)
    Write_Data(b);				//   Default => 0xB5
                        //     0xB5 => Normal
                        //     0xFD => Enhance Low Gray Scale Display Quality

}
void objOled::Set_Master_Current(unsigned char d)
{
    Write_Command(0xC7);			// Master Contrast Current Control
    Write_Data(d);				//   Default => 0x0f (Maximum)

}
void objOled::Set_Gray_Scale_Table()
{
    Write_Command(0xB8);			// Set Gray Scale Table
    Write_Data(0x0C);			//   Gray Scale Level 1
    Write_Data(0x18);			//   Gray Scale Level 2
    Write_Data(0x24);			//   Gray Scale Level 3
    Write_Data(0x30);			//   Gray Scale Level 4
    Write_Data(0x3C);			//   Gray Scale Level 5
    Write_Data(0x48);			//   Gray Scale Level 6
    Write_Data(0x54);			//   Gray Scale Level 7
    Write_Data(0x60);			//   Gray Scale Level 8
    Write_Data(0x6C);			//   Gray Scale Level 9
    Write_Data(0x78);			//   Gray Scale Level 10
    Write_Data(0x84);			//   Gray Scale Level 11
    Write_Data(0x90);			//   Gray Scale Level 12
    Write_Data(0x9C);			//   Gray Scale Level 13
    Write_Data(0xA8);			//   Gray Scale Level 14
    Write_Data(0xB4);			//   Gray Scale Level 15

    Write_Command(0x00);			// Enable Gray Scale Table

}
void objOled::Set_Phase_Length(unsigned char d)
{
    Write_Command(0xB1);			// Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment
    Write_Data(d);				//   Default => 0x74 (7 Display Clocks [Phase 2] / 9 Display Clocks [Phase 1])
                        //     D[3:0] => Phase 1 Period in 5~31 Display Clocks
                        //     D[7:4] => Phase 2 Period in 3~15 Display Clocks

}
void objOled::Set_Display_Enhancement_B(unsigned char d)
{
    Write_Command(0xD1);			// Display Enhancement
    Write_Data(d);				//   Default => 0xA2
                        //     0x82 => Reserved
                        //     0xA2 => Normal
    Write_Data(0x20);

}
void objOled::Set_Precharge_Voltage(unsigned char d)
{
    Write_Command(0xBB);			// Set Pre-Charge Voltage Level
    Write_Data(d);				//   Default => 0x17 (0.50*VCC)

}
void objOled::Set_Precharge_Period(unsigned char d)
{
    Write_Command(0xB6);			// Set Second Pre-Charge Period
    Write_Data(d);				//   Default => 0x08 (8 Display Clocks)

}
void objOled::Set_VCOMH(unsigned char d)
{
    Write_Command(0xBE);			// Set COM Deselect Voltage Level
    Write_Data(d);				//   Default => 0x04 (0.80*VCC)

}
void objOled::Set_Display_Mode(unsigned char d)
{
    Write_Command(d);			// Set Display Mode
                        //   Default => 0xA4
                        //     0xA4 => Entire Display Off, All Pixels Turn Off
                        //     0xA5 => Entire Display On, All Pixels Turn On at GS Level 15
                        //     0xA6 => Normal Display
                        //     0xA7 => Inverse Display

}
void objOled::Set_Partial_Display(unsigned char a, unsigned char b, unsigned char c)
{
    Write_Command(0xA8|a);

    if(a == 0x00)
    {
        Write_Data(b);
        Write_Data(c);
    }

}
void objOled::Set_Write_RAM()
{
    Write_Command(0x5C);			// Enable MCU to Write into RAM

}

void objOled::Fill_RAM(unsigned char Data)
{
    unsigned char i,j;

        Set_Column_Address(0x00,0x77);
        Set_Row_Address(0x00,0x7F);
        Set_Write_RAM();

        for(i=0;i<128;i++)
        {
            for(j=0;j<120;j++)
            {
                Write_Data(Data);
                Write_Data(Data);
            }
        }

}
void objOled::Set_Column_Address(unsigned char a, unsigned char b)
{
    Write_Command(0x15);			// Set Column Address
    Write_Data(a);				//   Default => 0x00
    Write_Data(b);				//   Default => 0x77

}
void objOled::Set_Row_Address(unsigned char a, unsigned char b)
{
    Write_Command(0x75);			// Set Row Address
    Write_Data(a);				//   Default => 0x00
    Write_Data(b);				//   Default => 0x7F

}
void objOled::Set_Contrast_Current(unsigned char d)
{
    Write_Command(0xC1);			// Set Contrast Current
    Write_Data(d);				//   Default => 0x7F

}

void objOled::OLED_Init()
{
    unsigned char i;

    //qDebug("func OLED_Init");

        setRES(0);//RES=0;
        for(i=0;i<200;i++)
        {
            //uDelay(200);
            usleep(200);
        }
        setRES(1);//RES=1;

        Set_Command_Lock(0x12);			// Unlock Basic Commands (0x12/0x16)
        Set_Display_On_Off(0xAE);		// Display Off (0xAE/0xAF)
        Set_Display_Clock(0x91);		// Set Clock as 80 Frames/Sec

        //qDebug("func OLED_Init 30");

        Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
        Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
        Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x7F)
        Set_Remap_Format(0x14);			// Set Horizontal Address Increment
                            //     Column Address 0 Mapped to SEG0
                            //     Disable Nibble Remap
                            //     Scan from COM[N-1] to COM0
                            //     Disable COM Split Odd Even
                            //     Enable Dual COM Line Mode
        Set_GPIO(0x00);				// Disable GPIO Pins Input
        Set_Function_Selection(0x01);		// Enable Internal VDD Regulator

        //qDebug("func OLED_Init 50");

        Set_Display_Enhancement_A(0xA0,0xFD);	// Enable External VSL
                            // Set Low Gray Scale Enhancement
        Set_Contrast_Current(0x9F);		// Set Segment Output Current
        Set_Master_Current(OLED_Brightness);		// Set Scale Factor of Segment Output Current Control
        Set_Gray_Scale_Table();			// Set Pulse Width for Gray Scale Table
        Set_Phase_Length(0xE8);			// Set Phase 1 as 17 Clocks & Phase 2 as 14 Clocks
        Set_Display_Enhancement_B(0xA2);	// Enhance Driving Scheme Capability (0x82/0xA2)
        //qDebug("func OLED_Init 70");
        Set_Precharge_Voltage(0x1F);		// Set Pre-Charge Voltage Level as 0.60*VCC
        Set_Precharge_Period(0x08);		// Set Second Pre-Charge Period as 8 Clocks
        Set_VCOMH(0x07);			// Set Common Pins Deselect Voltage Level as 0.86*VCC
        Set_Display_Mode(0xA6);			// Normal Display Mode (0xA4/0xA5/0xA6/0xA7)
        Set_Partial_Display(0x01,0x00,0x00);	// Disable Partial Display

        //qDebug("func OLED_Init 80");

        Fill_RAM(0x00);				// Clear Screen
        //qDebug("func OLED_Init 90");

        Set_Display_On_Off(0xAF);		// Display On (0xAE/0xAF)

        emit sigFlush();

        qDebug("  objoled OLED_init.done ,  emit sigReadyOled");

        QTimer::singleShot(500,this,SIGNAL(sigReadyOled()));
}
