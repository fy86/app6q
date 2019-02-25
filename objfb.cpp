#include "objfb.h"

char objFB::font8x12[256][12]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x00
{0x00,0x7E,0xC3,0x81,0xA5,0x81,0xBD,0x99,0xC3,0x7E,0x00,0x00},	// 0x01
{0x00,0x7E,0xFF,0xFF,0xDB,0xFF,0xC3,0xE7,0xFF,0x7E,0x00,0x00},	// 0x02
{0x00,0x00,0x22,0x77,0x7F,0x7F,0x7F,0x3E,0x1C,0x08,0x00,0x00},	// 0x03
{0x00,0x08,0x1C,0x3E,0x7F,0x7F,0x3E,0x1C,0x08,0x00,0x00,0x00},	// 0x04
{0x00,0x18,0x3C,0x3C,0xFF,0xE7,0xE7,0x18,0x18,0x7E,0x00,0x00},	// 0x05
{0x00,0x18,0x3C,0x7E,0xFF,0xFF,0x7E,0x18,0x18,0x7E,0x00,0x00},	// 0x06
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x07
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x08
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x09
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x0A
{0x00,0x7C,0x70,0x5C,0x4E,0x1F,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x0B
{0x00,0x3C,0x66,0x66,0x66,0x3C,0x18,0x7E,0x18,0x18,0x00,0x00},	// 0x0C
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x0D
{0x00,0xFE,0xC6,0xFE,0xC6,0xC6,0xC6,0xE6,0xE7,0x67,0x03,0x00},	// 0x0E
{0x00,0x00,0x18,0xDB,0x7E,0xE7,0xE7,0x7E,0xDB,0x18,0x00,0x00},	// 0x0F
{0x00,0x01,0x03,0x07,0x1F,0x7F,0x1F,0x07,0x03,0x01,0x00,0x00},	// 0x10
{0x00,0x40,0x60,0x70,0x7C,0x7F,0x7C,0x70,0x60,0x40,0x00,0x00},	// 0x11
{0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00,0x00},	// 0x12
{0x00,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x66,0x66,0x00,0x00},	// 0x13
{0x00,0xFE,0xDB,0xDB,0xDB,0xDE,0xD8,0xD8,0xD8,0xD8,0x00,0x00},	// 0x14
{0x00,0x7E,0xC6,0x0C,0x3C,0x66,0x66,0x3C,0x30,0x63,0x7E,0x00},	// 0x15
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x7F,0x7F,0x00,0x00},	// 0x16
{0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x7E,0x3C,0x18,0x7E,0x00},	// 0x17
{0x00,0x18,0x3C,0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00},	// 0x18
{0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x3C,0x18,0x00,0x00},	// 0x19
{0x00,0x00,0x00,0x18,0x30,0x7F,0x30,0x18,0x00,0x00,0x00,0x00},	// 0x1A
{0x00,0x00,0x00,0x0C,0x06,0x7F,0x06,0x0C,0x00,0x00,0x00,0x00},	// 0x1B
{0x00,0x00,0x00,0x00,0x03,0x03,0x03,0x7F,0x00,0x00,0x00,0x00},	// 0x1C
{0x00,0x00,0x00,0x24,0x66,0xFF,0x66,0x24,0x00,0x00,0x00,0x00},	// 0x1D
{0x00,0x00,0x08,0x08,0x1C,0x1C,0x3E,0x3E,0x7F,0x7F,0x00,0x00},	// 0x1E
{0x00,0x00,0x7F,0x7F,0x3E,0x3E,0x1C,0x1C,0x08,0x08,0x00,0x00},	// 0x1F
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x20
{0x00,0x0C,0x1E,0x1E,0x1E,0x0C,0x0C,0x00,0x0C,0x0C,0x00,0x00},	// 0x21
{0x00,0x66,0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x22
{0x00,0x36,0x36,0x7F,0x36,0x36,0x36,0x7F,0x36,0x36,0x00,0x00},	// 0x23
{0x0C,0x0C,0x3E,0x03,0x03,0x1E,0x30,0x30,0x1F,0x0C,0x0C,0x00},	// 0x24
{0x00,0x00,0x00,0x23,0x33,0x18,0x0C,0x06,0x33,0x31,0x00,0x00},	// 0x25
{0x00,0x0E,0x1B,0x1B,0x0E,0x5F,0x7B,0x33,0x3B,0x6E,0x00,0x00},	// 0x26
{0x00,0x0C,0x0C,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x27
{0x00,0x30,0x18,0x0C,0x06,0x06,0x06,0x0C,0x18,0x30,0x00,0x00},	// 0x28
{0x00,0x06,0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x06,0x00,0x00},	// 0x29
{0x00,0x00,0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,0x00,0x00},	// 0x2A
{0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,0x00,0x00},	// 0x2B
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x06,0x00},	// 0x2C
{0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x2D
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x00,0x00},	// 0x2E
{0x00,0x00,0x40,0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00,0x00},	// 0x2F
{0x00,0x3E,0x63,0x73,0x7B,0x6B,0x6F,0x67,0x63,0x3E,0x00,0x00},	// 0x30
{0x00,0x08,0x0C,0x0F,0x0C,0x0C,0x0C,0x0C,0x0C,0x3F,0x00,0x00},	// 0x31
{0x00,0x1E,0x33,0x33,0x30,0x18,0x0C,0x06,0x33,0x3F,0x00,0x00},	// 0x32
{0x00,0x1E,0x33,0x30,0x30,0x1C,0x30,0x30,0x33,0x1E,0x00,0x00},	// 0x33
{0x00,0x30,0x38,0x3C,0x36,0x33,0x7F,0x30,0x30,0x78,0x00,0x00},	// 0x34
{0x00,0x3F,0x03,0x03,0x03,0x1F,0x30,0x30,0x33,0x1E,0x00,0x00},	// 0x35
{0x00,0x1C,0x06,0x03,0x03,0x1F,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x36
{0x00,0x7F,0x63,0x63,0x60,0x30,0x18,0x0C,0x0C,0x0C,0x00,0x00},	// 0x37
{0x00,0x1E,0x33,0x33,0x33,0x1E,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x38
{0x00,0x1E,0x33,0x33,0x33,0x3E,0x18,0x18,0x0C,0x0E,0x00,0x00},	// 0x39
{0x00,0x00,0x00,0x1C,0x1C,0x00,0x00,0x1C,0x1C,0x00,0x00,0x00},	// 0x3A
{0x00,0x00,0x00,0x1C,0x1C,0x00,0x00,0x1C,0x1C,0x18,0x0C,0x00},	// 0x3B
{0x00,0x30,0x18,0x0C,0x06,0x03,0x06,0x0C,0x18,0x30,0x00,0x00},	// 0x3C
{0x00,0x00,0x00,0x00,0x7E,0x00,0x7E,0x00,0x00,0x00,0x00,0x00},	// 0x3D
{0x00,0x06,0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x06,0x00,0x00},	// 0x3E
{0x00,0x1E,0x33,0x30,0x18,0x0C,0x0C,0x00,0x0C,0x0C,0x00,0x00},	// 0x3F
{0x00,0x3E,0x63,0x63,0x7B,0x7B,0x7B,0x03,0x03,0x3E,0x00,0x00},	// 0x40
{0x00,0x0C,0x1E,0x33,0x33,0x33,0x3F,0x33,0x33,0x33,0x00,0x00},	// 0x41
{0x00,0x3F,0x66,0x66,0x66,0x3E,0x66,0x66,0x66,0x3F,0x00,0x00},	// 0x42
{0x00,0x3C,0x66,0x63,0x03,0x03,0x03,0x63,0x66,0x3C,0x00,0x00},	// 0x43
{0x00,0x1F,0x36,0x66,0x66,0x66,0x66,0x66,0x36,0x1F,0x00,0x00},	// 0x44
{0x00,0x7F,0x46,0x06,0x26,0x3E,0x26,0x06,0x46,0x7F,0x00,0x00},	// 0x45
{0x00,0x7F,0x66,0x46,0x26,0x3E,0x26,0x06,0x06,0x0F,0x00,0x00},	// 0x46
{0x00,0x3C,0x66,0x63,0x03,0x03,0x73,0x63,0x66,0x7C,0x00,0x00},	// 0x47
{0x00,0x33,0x33,0x33,0x33,0x3F,0x33,0x33,0x33,0x33,0x00,0x00},	// 0x48
{0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00},	// 0x49
{0x00,0x78,0x30,0x30,0x30,0x30,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x4A
{0x00,0x67,0x66,0x36,0x36,0x1E,0x36,0x36,0x66,0x67,0x00,0x00},	// 0x4B
{0x00,0x0F,0x06,0x06,0x06,0x06,0x46,0x66,0x66,0x7F,0x00,0x00},	// 0x4C
{0x00,0x63,0x77,0x7F,0x7F,0x6B,0x63,0x63,0x63,0x63,0x00,0x00},	// 0x4D
{0x00,0x63,0x63,0x67,0x6F,0x7F,0x7B,0x73,0x63,0x63,0x00,0x00},	// 0x4E
{0x00,0x1C,0x36,0x63,0x63,0x63,0x63,0x63,0x36,0x1C,0x00,0x00},	// 0x4F
{0x00,0x3F,0x66,0x66,0x66,0x3E,0x06,0x06,0x06,0x0F,0x00,0x00},	// 0x50
{0x00,0x1C,0x36,0x63,0x63,0x63,0x73,0x7B,0x3E,0x30,0x78,0x00},	// 0x51
{0x00,0x3F,0x66,0x66,0x66,0x3E,0x36,0x66,0x66,0x67,0x00,0x00},	// 0x52
{0x00,0x1E,0x33,0x33,0x03,0x0E,0x18,0x33,0x33,0x1E,0x00,0x00},	// 0x53
{0x00,0x3F,0x2D,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00},	// 0x54
{0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x55
{0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x0C,0x00,0x00},	// 0x56
{0x00,0x63,0x63,0x63,0x63,0x6B,0x6B,0x36,0x36,0x36,0x00,0x00},	// 0x57
{0x00,0x33,0x33,0x33,0x1E,0x0C,0x1E,0x33,0x33,0x33,0x00,0x00},	// 0x58
{0x00,0x33,0x33,0x33,0x33,0x1E,0x0C,0x0C,0x0C,0x1E,0x00,0x00},	// 0x59
{0x00,0x7F,0x73,0x19,0x18,0x0C,0x06,0x46,0x63,0x7F,0x00,0x00},	// 0x5A
{0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,0x00},	// 0x5B
{0x00,0x00,0x01,0x03,0x06,0x0C,0x18,0x30,0x60,0x40,0x00,0x00},	// 0x5C
{0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,0x00},	// 0x5D
{0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x5E
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00},	// 0x5F
{0x0C,0x0C,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x60
{0x00,0x00,0x00,0x00,0x1E,0x30,0x3E,0x33,0x33,0x6E,0x00,0x00},	// 0x61
{0x00,0x07,0x06,0x06,0x3E,0x66,0x66,0x66,0x66,0x3B,0x00,0x00},	// 0x62
{0x00,0x00,0x00,0x00,0x1E,0x33,0x03,0x03,0x33,0x1E,0x00,0x00},	// 0x63
{0x00,0x38,0x30,0x30,0x3E,0x33,0x33,0x33,0x33,0x6E,0x00,0x00},	// 0x64
{0x00,0x00,0x00,0x00,0x1E,0x33,0x3F,0x03,0x33,0x1E,0x00,0x00},	// 0x65
{0x00,0x1C,0x36,0x06,0x06,0x1F,0x06,0x06,0x06,0x0F,0x00,0x00},	// 0x66
{0x00,0x00,0x00,0x00,0x6E,0x33,0x33,0x33,0x3E,0x30,0x33,0x1E},	// 0x67
{0x00,0x07,0x06,0x06,0x36,0x6E,0x66,0x66,0x66,0x67,0x00,0x00},	// 0x68
{0x00,0x18,0x18,0x00,0x1E,0x18,0x18,0x18,0x18,0x7E,0x00,0x00},	// 0x69
{0x00,0x30,0x30,0x00,0x3C,0x30,0x30,0x30,0x30,0x33,0x33,0x1E},	// 0x6A
{0x00,0x07,0x06,0x06,0x66,0x36,0x1E,0x36,0x66,0x67,0x00,0x00},	// 0x6B
{0x00,0x1E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00},	// 0x6C
{0x00,0x00,0x00,0x00,0x3F,0x6B,0x6B,0x6B,0x6B,0x63,0x00,0x00},	// 0x6D
{0x00,0x00,0x00,0x00,0x1F,0x33,0x33,0x33,0x33,0x33,0x00,0x00},	// 0x6E
{0x00,0x00,0x00,0x00,0x1E,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x6F
{0x00,0x00,0x00,0x00,0x3B,0x66,0x66,0x66,0x66,0x3E,0x06,0x0F},	// 0x70
{0x00,0x00,0x00,0x00,0x6E,0x33,0x33,0x33,0x33,0x3E,0x30,0x78},	// 0x71
{0x00,0x00,0x00,0x00,0x37,0x76,0x6E,0x06,0x06,0x0F,0x00,0x00},	// 0x72
{0x00,0x00,0x00,0x00,0x1E,0x33,0x06,0x18,0x33,0x1E,0x00,0x00},	// 0x73
{0x00,0x00,0x04,0x06,0x3F,0x06,0x06,0x06,0x36,0x1C,0x00,0x00},	// 0x74
{0x00,0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x33,0x6E,0x00,0x00},	// 0x75
{0x00,0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x1E,0x0C,0x00,0x00},	// 0x76
{0x00,0x00,0x00,0x00,0x63,0x63,0x6B,0x6B,0x36,0x36,0x00,0x00},	// 0x77
{0x00,0x00,0x00,0x00,0x63,0x36,0x1C,0x1C,0x36,0x63,0x00,0x00},	// 0x78
{0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x3C,0x30,0x18,0x0F},	// 0x79
{0x00,0x00,0x00,0x00,0x3F,0x31,0x18,0x06,0x23,0x3F,0x00,0x00},	// 0x7A
{0x00,0x38,0x0C,0x0C,0x06,0x03,0x06,0x0C,0x0C,0x38,0x00,0x00},	// 0x7B
{0x00,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x00,0x00},	// 0x7C
{0x00,0x07,0x0C,0x0C,0x18,0x30,0x18,0x0C,0x0C,0x07,0x00,0x00},	// 0x7D
{0x00,0xCE,0x5B,0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x7E
{0x00,0x00,0x00,0x08,0x1C,0x36,0x63,0x63,0x7F,0x00,0x00,0x00},	// 0x7F
{0x00,0x1E,0x33,0x33,0x03,0x03,0x03,0x33,0x33,0x1E,0x0C,0x06},	// 0x80
{0x00,0x33,0x33,0x00,0x33,0x33,0x33,0x33,0x33,0x6E,0x00,0x00},	// 0x81
{0x30,0x18,0x0C,0x00,0x1E,0x33,0x3F,0x03,0x33,0x1E,0x00,0x00},	// 0x82
{0x0C,0x1E,0x33,0x00,0x1E,0x30,0x3E,0x33,0x33,0x6E,0x00,0x00},	// 0x83
{0x00,0x33,0x33,0x00,0x1E,0x30,0x3E,0x33,0x33,0x6E,0x00,0x00},	// 0x84
{0x03,0x06,0x0C,0x00,0x1E,0x30,0x3E,0x33,0x33,0x6E,0x00,0x00},	// 0x85
{0x1C,0x36,0x36,0x1C,0x1F,0x30,0x3E,0x33,0x33,0x6E,0x00,0x00},	// 0x86
{0x00,0x00,0x00,0x00,0x1E,0x33,0x03,0x03,0x33,0x1E,0x0C,0x06},	// 0x87
{0x0C,0x1E,0x33,0x00,0x1E,0x33,0x3F,0x03,0x03,0x3E,0x00,0x00},	// 0x88
{0x00,0x33,0x33,0x00,0x1E,0x33,0x3F,0x03,0x03,0x3E,0x00,0x00},	// 0x89
{0x03,0x06,0x0C,0x00,0x1E,0x33,0x3F,0x03,0x03,0x3E,0x00,0x00},	// 0x8A
{0x00,0x36,0x36,0x00,0x1E,0x18,0x18,0x18,0x18,0x7E,0x00,0x00},	// 0x8B
{0x08,0x1C,0x36,0x00,0x1E,0x18,0x18,0x18,0x18,0x7E,0x00,0x00},	// 0x8C
{0x06,0x0C,0x18,0x00,0x1E,0x18,0x18,0x18,0x18,0x7E,0x00,0x00},	// 0x8D
{0x00,0x33,0x00,0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00,0x00},	// 0x8E
{0x1E,0x33,0x33,0x1E,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00,0x00},	// 0x8F
{0x30,0x18,0x00,0x3F,0x23,0x03,0x1F,0x03,0x23,0x3F,0x00,0x00},	// 0x90
{0x00,0x00,0x00,0x00,0x7F,0xD8,0xFE,0x1B,0x1B,0xF7,0x00,0x00},	// 0x91
{0x00,0x7C,0x1E,0x1B,0x1B,0x7F,0x1B,0x1B,0x1B,0x7B,0x00,0x00},	// 0x92
{0x0C,0x1E,0x33,0x00,0x1E,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x93
{0x00,0x33,0x33,0x00,0x1E,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x94
{0x03,0x06,0x0C,0x00,0x1E,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x95
{0x0C,0x1E,0x33,0x00,0x33,0x33,0x33,0x33,0x33,0x6E,0x00,0x00},	// 0x96
{0x03,0x06,0x0C,0x00,0x33,0x33,0x33,0x33,0x33,0x6E,0x00,0x00},	// 0x97
{0x00,0x66,0x66,0x00,0x66,0x66,0x66,0x66,0x3C,0x30,0x18,0x0F},	// 0x98
{0x00,0x33,0x00,0x1E,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x99
{0x33,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0x9A
{0x00,0x00,0x00,0x00,0x1E,0x33,0x3B,0x37,0x33,0x1E,0x00,0x00},	// 0x9B
{0x3C,0x66,0x06,0x06,0x06,0x3F,0x06,0x06,0x03,0x7F,0x00,0x00},	// 0x9C
{0x00,0x5C,0x36,0x73,0x6B,0x6B,0x6B,0x67,0x36,0x1D,0x00,0x00},	// 0x9D
{0x00,0x00,0x00,0x00,0x00,0x63,0x36,0x1C,0x36,0x63,0x00,0x00},	// 0x9E
{0x70,0xD8,0x18,0x18,0x7E,0x18,0x18,0x18,0x1B,0x0E,0x00,0x00},	// 0x9F
{0x30,0x18,0x0C,0x00,0x1E,0x30,0x3E,0x33,0x33,0x6E,0x00,0x00},	// 0xA0
{0x30,0x18,0x0C,0x00,0x1E,0x18,0x18,0x18,0x18,0x7E,0x00,0x00},	// 0xA1
{0x30,0x18,0x0C,0x00,0x1E,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0xA2
{0x30,0x18,0x0C,0x00,0x33,0x33,0x33,0x33,0x33,0x6E,0x00,0x00},	// 0xA3
{0x00,0x6E,0x3B,0x00,0x1F,0x33,0x33,0x33,0x33,0x33,0x00,0x00},	// 0xA4
{0x6E,0x3B,0x00,0x63,0x67,0x6F,0x7B,0x73,0x63,0x63,0x00,0x00},	// 0xA5
{0x00,0x1E,0x33,0x33,0x7E,0x00,0x7F,0x00,0x00,0x00,0x00,0x00},	// 0xA6
{0x00,0x1E,0x33,0x33,0x1E,0x00,0x7F,0x00,0x00,0x00,0x00,0x00},	// 0xA7
{0x00,0x0C,0x0C,0x00,0x0C,0x06,0x03,0x03,0x33,0x1E,0x00,0x00},	// 0xA8
{0x00,0x1C,0x22,0x5D,0x55,0x5D,0x4D,0x55,0x22,0x1C,0x00,0x00},	// 0xA9
{0x00,0x00,0x00,0x00,0x00,0x3F,0x30,0x30,0x30,0x00,0x00,0x00},	// 0xAA
{0x00,0x46,0x67,0x36,0x1E,0x0C,0x76,0xC3,0x61,0x30,0xF8,0x00},	// 0xAB
{0x00,0xC6,0x67,0x36,0x1E,0xEC,0xF6,0xDB,0xCD,0xFC,0xC0,0x00},	// 0xAC
{0x00,0x0C,0x0C,0x00,0x0C,0x0C,0x1E,0x1E,0x1E,0x0C,0x00,0x00},	// 0xAD
{0x00,0x00,0x00,0x00,0xCC,0x66,0x33,0x33,0x66,0xCC,0x00,0x00},	// 0xAE
{0x00,0x00,0x00,0x00,0x33,0x66,0xCC,0xCC,0x66,0x33,0x00,0x00},	// 0xAF
{0x24,0x49,0x92,0x24,0x49,0x92,0x24,0x49,0x92,0x24,0x49,0x92},	// 0xB0
{0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55,0xAA,0x55},	// 0xB1
{0xB6,0xDB,0x6D,0xB6,0xDB,0x6D,0xB6,0xDB,0x6D,0xB6,0xDB,0x6D},	// 0xB2
{0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18},	// 0xB3
{0x18,0x18,0x18,0x18,0x18,0x1F,0x18,0x18,0x18,0x18,0x18,0x18},	// 0xB4
{0x30,0x18,0x00,0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00,0x00},	// 0xB5
{0x1E,0x33,0x00,0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00,0x00},	// 0xB6
{0x06,0x0C,0x00,0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00,0x00},	// 0xB7
{0x00,0x1C,0x22,0x5D,0x45,0x45,0x45,0x5D,0x22,0x1C,0x00,0x00},	// 0xB8
{0x66,0x66,0x66,0x66,0x67,0x60,0x60,0x67,0x66,0x66,0x66,0x66},	// 0xB9
{0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66},	// 0xBA
{0x00,0x00,0x00,0x00,0x7F,0x60,0x60,0x67,0x66,0x66,0x66,0x66},	// 0xBB
{0x66,0x66,0x66,0x66,0x67,0x60,0x60,0x7F,0x00,0x00,0x00,0x00},	// 0xBC
{0x00,0x0C,0x0C,0x1E,0x33,0x03,0x03,0x33,0x1E,0x0C,0x0C,0x00},	// 0xBD
{0x33,0x33,0x33,0x33,0x1E,0x3F,0x0C,0x3F,0x0C,0x0C,0x00,0x00},	// 0xBE
{0x00,0x00,0x00,0x00,0x00,0x1F,0x18,0x18,0x18,0x18,0x18,0x18},	// 0xBF
{0x18,0x18,0x18,0x18,0x18,0xF8,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xC0
{0x18,0x18,0x18,0x18,0x18,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xC1
{0x00,0x00,0x00,0x00,0x00,0xFF,0x18,0x18,0x18,0x18,0x18,0x18},	// 0xC2
{0x18,0x18,0x18,0x18,0x18,0xF8,0x18,0x18,0x18,0x18,0x18,0x18},	// 0xC3
{0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xC4
{0x18,0x18,0x18,0x18,0x18,0xFF,0x18,0x18,0x18,0x18,0x18,0x18},	// 0xC5
{0x00,0x6E,0x3B,0x00,0x1E,0x30,0x3E,0x33,0x33,0x6E,0x00,0x00},	// 0xC6
{0x6E,0x3B,0x00,0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00,0x00},	// 0xC7
{0x66,0x66,0x66,0x66,0xE6,0x06,0x06,0xFE,0x00,0x00,0x00,0x00},	// 0xC8
{0x00,0x00,0x00,0x00,0xFE,0x06,0x06,0xE6,0x66,0x66,0x66,0x66},	// 0xC9
{0x66,0x66,0x66,0x66,0xE7,0x00,0x00,0xFF,0x00,0x00,0x00,0x00},	// 0xCA
{0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0xE7,0x66,0x66,0x66,0x66},	// 0xCB
{0x66,0x66,0x66,0x66,0xE6,0x06,0x06,0xE6,0x66,0x66,0x66,0x66},	// 0xCC
{0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0x00,0x00},	// 0xCD
{0x66,0x66,0x66,0x66,0xE7,0x00,0x00,0xE7,0x66,0x66,0x66,0x66},	// 0xCE
{0x00,0x00,0x00,0x00,0x00,0x63,0x3E,0x36,0x3E,0x63,0x00,0x00},	// 0xCF
{0x33,0x0C,0x1B,0x30,0x60,0x7E,0x63,0x63,0x63,0x3E,0x00,0x00},	// 0xD0
{0x00,0x1F,0x36,0x66,0x66,0x6F,0x66,0x66,0x36,0x1F,0x00,0x00},	// 0xD1
{0x1E,0x33,0x00,0x3F,0x23,0x03,0x1F,0x03,0x23,0x3F,0x00,0x00},	// 0xD2
{0x00,0x33,0x00,0x3F,0x23,0x03,0x1F,0x03,0x23,0x3F,0x00,0x00},	// 0xD3
{0x06,0x0C,0x00,0x3F,0x23,0x03,0x1F,0x03,0x23,0x3F,0x00,0x00},	// 0xD4
{0x00,0x0F,0x0C,0x0C,0x0C,0x3F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xD5
{0x18,0x0C,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00},	// 0xD6
{0x1E,0x33,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00},	// 0xD7
{0x00,0x33,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00},	// 0xD8
{0x18,0x18,0x18,0x18,0x18,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xD9
{0x00,0x00,0x00,0x00,0x00,0xF8,0x18,0x18,0x18,0x18,0x18,0x18},	// 0xDA
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},	// 0xDB
{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},	// 0xDC
{0x00,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x00,0x00},	// 0xDD
{0x06,0x0C,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00},	// 0xDE
{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xDF
{0x18,0x0C,0x00,0x1E,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0xE0
{0x00,0x1E,0x33,0x33,0x1B,0x33,0x33,0x33,0x1F,0x03,0x06,0x00},	// 0xE1
{0x1E,0x33,0x00,0x1E,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0xE2
{0x06,0x0C,0x00,0x1E,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0xE3
{0x00,0x6E,0x3B,0x00,0x1E,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0xE4
{0x6E,0x3B,0x00,0x1E,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0xE5
{0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0xDE,0x06,0x03},	// 0xE6
{0x00,0x00,0x07,0x06,0x3E,0x66,0x66,0x3E,0x06,0x0F,0x00,0x00},	// 0xE7
{0x00,0x0F,0x06,0x3E,0x66,0x66,0x66,0x3E,0x06,0x0F,0x00,0x00},	// 0xE8
{0x18,0x0C,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0xE9
{0x1E,0x33,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0xEA
{0x06,0x0C,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00},	// 0xEB
{0x60,0x30,0x18,0x00,0x66,0x66,0x66,0x66,0x3C,0x30,0x18,0x0F},	// 0xEC
{0x18,0x0C,0x00,0x33,0x33,0x33,0x1E,0x0C,0x0C,0x1E,0x00,0x00},	// 0xED
{0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xEE
{0x30,0x18,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xEF
{0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xF0
{0x00,0x00,0x0C,0x0C,0x3F,0x0C,0x0C,0x00,0x3F,0x00,0x00,0x00},	// 0xF1
{0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x1F,0x00,0x00,0x00},	// 0xF2
{0x07,0xCC,0x66,0x3C,0x1F,0xEC,0xF6,0xDB,0xCD,0xFC,0xC0,0x00},	// 0xF3
{0x00,0xFE,0xDB,0xDB,0xDB,0xDE,0xD8,0xD8,0xD8,0xD8,0x00,0x00},	// 0xF4
{0x00,0x7E,0xC6,0x0C,0x3C,0x66,0x66,0x3C,0x30,0x63,0x7E,0x00},	// 0xF5
{0x00,0x00,0x0C,0x0C,0x00,0x3F,0x00,0x0C,0x0C,0x00,0x00,0x00},	// 0xF6
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x0E},	// 0xF7
{0x00,0x3C,0x66,0x66,0x66,0x3C,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xF8
{0x00,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xF9
{0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xFA
{0x00,0x0C,0x0E,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xFB
{0x00,0x1E,0x30,0x1C,0x30,0x1E,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xFC
{0x00,0x1E,0x30,0x18,0x0C,0x3E,0x00,0x00,0x00,0x00,0x00,0x00},	// 0xFD
{0x00,0x00,0x00,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00},	// 0xFE
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} 	// 0xFF
};


objFB::objFB(QObject *parent) :
    objOled(parent)
{
    m_nPage = 0;
    m_baFB.resize(10000);
    loaddot();
}

void objFB::loaddot()
{
    int fd;

    m_ba16.resize(300000);

#if 0
    fd=open("song12.dot",O_RDONLY);
    if(fd!=-1){
    read(fd,gp12,300000);
    close(fd);
    }
#endif
    fd=open("/root/qt/song16.dot",O_RDONLY);
    if(fd!=-1){
        read(fd,m_ba16.data(),290000);
        close(fd);
    }
#if 0
    fd=open("song24.dot",O_RDONLY);
    if(fd!=-1){
    read(fd,gp24,650000);
    close(fd);
    }
#endif
}
void objFB::zeroFB(char f)
{
    int i;
    char *p;
    p=m_baFB.data();
    for(i=0;i<(256*64/2);i++){
        *p++=f;
    }

}

void objFB::demo16()
{
    //qDebug("func demo16");

    OLED_Init();
    emit sigFlush();

    zeroFB(0x0);
    //lineXY();

    //col0();
    //Fill_BlockP(gfb,0,64-1,0,63);

    showHZdemo();

}
void objFB::show8x16(char ch, int x, int y)
{
    int i,j;

    for(i=0;i<2;i++){
        for(j=0;j<8;j++){
            draw1(x+j,y+i,0);
        }
    }
    for(i=14;i<16;i++){
        for(j=0;j<8;j++){
            draw1(x+j,y+i,0);
        }
    }
    show8x12(ch,x,y+2);

}
void objFB::show8x12(char ch, int x, int y)
{
    int offset;
    offset =(0x0ff & ch) * 12;
    int i,j;
    char *p,ch1;
    int m=1;
    p=(char*)font8x12;
    p+=offset;

    for(i=0;i<12;i++){
        ch1 = *p++;
        for(j=0;j<8;j++){
            m=1<<j;
            if(ch1 & m) draw1(x+j,y+i,0x0f);
            else draw1(x+j,y+i,0);
        }
    }
}

void objFB::showHZ16(char *p, int x, int y)
{
    char ch;
    int c1;
    int i,j,k,m;

    //qDebug("func showHZ16");

    for(i=0;i<16;i++){
        for(j=0;j<2;j++){
            ch = *p++;
            for(k=0;k<8;k++){
                m = 7-k;
                c1 = (1<<m)&ch;
                if(c1)
                   draw1(x+(j<<3)+k,y+i,0x0f);
                else
                   draw1(x+(j<<3)+k,y+i,0x0);
            }
        }
    }

}

void objFB::showHZdemo()
{
    char *pdot;
    int i,j;
    pdot = m_ba16.data()+(15*94*32);
    //pdot = gp16;
    for(i=0;i<4;i++){
        for(j=0;j<16;j++){
            showHZ16(pdot,j<<4,i<<4);
            pdot+=32;
        }
    }
    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();
}

void objFB::draw1(int x, int y, int c)
{
    int offset=0;
    int s;
    char *p;
    char ch;

    offset=y*128;
    offset+=(x/4)*2;
    switch(x%4){
#if 0
        case 0:
          offset+=1;
          s=0;
          break;
        case 1:
          offset+=1;
          s=4;
          break;
        case 2:
          s=0;
          break;
        default:
          s=4;
          break;
#endif
        case 0:
          s=4;
          break;
        case 1:
          s=0;
          break;
        case 2:
          offset+=1;
          s=4;
          break;
        default:
          offset+=1;
          s=0;
          break;
    }
    p=m_baFB.data() + offset;
    ch = *p;
    switch(s){
        case 0:
          c=0x0f & c;
          ch = (ch & 0x0ff)|c;
          break;
        case 4:
          c =(c<<4)&0x0f0;
          ch = (ch & 0x0f0)| c;
          break;
        default:
          break;
    }
    *p=ch;

}
// 0:left 1:center 2:left
void objFB::centerXY(QString str, int left, int top, int w, int h, int hc, int vc)
{
    QByteArray ba;
    int len;
    int x,y;

    x=left;
    y=top;

    str2GBK(str,ba);
    len=ba.size()<<3;
    switch(hc){
    case 0:
        x=left;
        break;
    case 1:
        x=left+((w-len)>1);
        break;
    case 2:
        x=left+w-len;
        break;
    default:
        break;
    }
    switch(vc){
    case 0:
        y=top;
        break;
    case 1:
        y=top+((h-16)>1);
        break;
    case 2:
        y=top-16;
        break;
    default:
        break;
    }
    strXY(str,x,y);
}

void objFB::strXY(QString str, int x, int y)
{
    if(x<0)return;
    if(y<0)return;
    if(x>OLED_WIDTH)return;
    if(y>OLED_HEIGHT)return;
    QByteArray ba;
    int i,len;
    int qu,wei;
    int n=0;
    char ch;
    int offset,x16=x;
    str2GBK(str,ba);
    len = ba.size();
    //qDebug("gbk size: %d",ba.size());
    if(len<1)return;

    //zeroFB(0);
    for(i=0;i<len;i++){
        if(0!=(0x080&ba.at(i))){
            if(n==0){
                n++;
                qu=(0x0ff&ba.at(i))-0x0a0;
                continue;
            }
            else{
                wei = (0x0ff & ba.at(i)) - 0x0a0;
                offset = ((qu-1)*94+wei-1)<<5;
                showHZ16(m_ba16.data()+offset,x16,y);
                x16+=16;
                n=0;
            }
        }
        else{
            show8x16(ba.at(i),x16,y);
            n=0;
            x16+=8;
        }
    }

#if 0
    QByteArray ba;
    int i,len;
    int offset;
    QString str1;
    int x16=x;

    zeroFB(0);
    len = str.length();
    for(i=0;i<len;i++){
        str1=str.mid(i,1);
        qDebug("%s",str1.toLatin1().data());
        str2GBK(str1,ba);
        qDebug("gbk size : %d",ba.size());
        switch(ba.size()){
        case 2:
            qDebug("gbk show");
            if(0==(0x080 & ba.at(0)))break;
            if(0==(0x080 & ba.at(1)))break;
            offset = (0x0ff & ba.at(1)) + 94*(0x0ff&ba.at(0));
            offset = offset<<4;
            offset=0;
            showHZ16(m_ba16.data()+offset,20,20);
            x16+=16;
            break;
        case 1:
            x16+=8;
            break;
        default:
            break;
        }
    }
#endif
    //Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    //emit sigFlush();

}

void objFB::show0000()
{
    zeroFB(0);

    //strXY(QString("网桥"),0,0);
    strXY(QString("空    闲"),0,0);
    strXY(QString("点  对  点"),128+48,0);
    strXY(QString("S/N: 12.34"),128+48,48);
    strXY(QString("发 : 1024k"),128+48,16);
    strXY(QString("收 : 1024k"),128+48,32);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
#if 0
void objFB::show0000()
{
    zeroFB(0);

    //strXY(QString("网桥"),0,0);
    strXY(QString("空    闲"),6*16,0);
    strXY(QString("点对点"),0,24);
    strXY(QString("S/N: 12.34"),8*16,24);
    strXY(QString("发 : 1024k"),0,48);
    strXY(QString("收 : 1024k"),8*16,48);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}
#endif

void objFB::show0001()
{
    //OLED_Init();

    zeroFB(0);

    strXY(QString("  发送功率: "),0,0);
    strXY(QString("  发送频点: "),0,24);
    strXY(QString("  接收频点: "),0,48);
    strXY(QString(" -37.25 dbm"),96,0);
    strXY(QString(" 14 000 000 000 Hz"),96,24);
    strXY(QString(" 12 250 000 000 Hz"),96,48);

    Fill_BlockP((unsigned char*)m_baFB.data(),0,63,0,63);

    emit sigFlush();

}

void objFB::slotKey(int)
{
    if(0==m_nPage){
        show0001();
        m_nPage = 1;
    }
    else{
        show0000();
        m_nPage=0;
    }
}



