/* http://srecord.sourceforge.net/ */
const unsigned char MtpHandler[] =
{
0x02, 0x02, 0xD7, 0x75, 0xFF, 0x02, 0x43, 0xE4, 0x04, 0x53, 0xE4, 0xF9,
0x75, 0xFF, 0x02, 0x53, 0xE6, 0xBF, 0x12, 0x02, 0xCA, 0x12, 0x02, 0xAC,
0x7F, 0x00, 0x12, 0x01, 0x32, 0x8F, 0x08, 0xE5, 0x08, 0x12, 0x03, 0x63,
0x00, 0x3D, 0x07, 0x00, 0xF9, 0x2E, 0x01, 0x13, 0x39, 0x00, 0x8A, 0x4D,
0x00, 0xAD, 0x56, 0x00, 0xDF, 0x9A, 0x00, 0x5E, 0xB5, 0x00, 0x00, 0x00,
0x18, 0x7F, 0x70, 0x12, 0x01, 0x32, 0x8F, 0x09, 0x7F, 0x00, 0x12, 0x01,
0x32, 0x8F, 0x0A, 0xAF, 0x0A, 0x12, 0x01, 0x8A, 0x8F, 0x0B, 0xAF, 0x0B,
0x12, 0x01, 0x32, 0xE5, 0x0A, 0x04, 0xF5, 0x14, 0x80, 0xBA, 0xAF, 0x14,
0x05, 0x14, 0x12, 0x01, 0x8A, 0x8F, 0x09, 0xAF, 0x14, 0x05, 0x14, 0x12,
0x01, 0x8A, 0x8F, 0x0A, 0xAF, 0x14, 0x05, 0x14, 0x12, 0x01, 0x8A, 0x8F,
0x0B, 0xAF, 0x09, 0x12, 0x01, 0x32, 0xAF, 0x0A, 0x12, 0x01, 0x32, 0xAF,
0x0B, 0x12, 0x01, 0x32, 0x80, 0x8E, 0x7F, 0xD8, 0x12, 0x01, 0x32, 0x8F,
0x09, 0x7F, 0x00, 0x12, 0x01, 0x32, 0x8F, 0x0A, 0xAF, 0x0A, 0x12, 0x01,
0x32, 0x8F, 0x0B, 0xAD, 0x0B, 0xAF, 0x0A, 0x12, 0x01, 0xCF, 0xE5, 0x0A,
0x04, 0xF5, 0x14, 0x01, 0x18, 0x7F, 0x67, 0x12, 0x01, 0x32, 0x8F, 0x09,
0xAF, 0x09, 0x12, 0x01, 0x32, 0x8F, 0x0A, 0xAF, 0x0A, 0x12, 0x01, 0x32,
0x8F, 0x0B, 0xAF, 0x14, 0x05, 0x14, 0xAD, 0x09, 0x12, 0x01, 0xCF, 0xAF,
0x14, 0x05, 0x14, 0xAD, 0x0A, 0x12, 0x01, 0xCF, 0xAF, 0x14, 0x05, 0x14,
0xAD, 0x0B, 0x12, 0x01, 0xCF, 0x01, 0x18, 0x7F, 0xA8, 0x12, 0x01, 0x32,
0x8F, 0x09, 0x7F, 0x00, 0x12, 0x01, 0x32, 0x8F, 0x0A, 0x7F, 0x00, 0x12,
0x01, 0x32, 0x8F, 0x0B, 0x12, 0x02, 0x0E, 0x01, 0x18, 0x7F, 0xE6, 0x12,
0x01, 0x32, 0x8F, 0x09, 0x7F, 0x00, 0x12, 0x01, 0x32, 0x8F, 0x0A, 0x7F,
0x00, 0x12, 0x01, 0x32, 0x8F, 0x0B, 0x12, 0x02, 0x4A, 0x01, 0x18, 0x7F,
0x92, 0x12, 0x01, 0x32, 0x8F, 0x09, 0x7F, 0x00, 0x12, 0x01, 0x32, 0x8F,
0x0A, 0x7F, 0x00, 0x12, 0x01, 0x32, 0x8F, 0x0B, 0xAF, 0x0B, 0x12, 0x02,
0x80, 0x01, 0x18, 0x01, 0x18, 0x22, 0x8F, 0x0C, 0x75, 0x0D, 0x00, 0x75,
0x0E, 0x08, 0xE5, 0xA0, 0x30, 0xE4, 0x04, 0x7F, 0x01, 0x80, 0x02, 0x7F,
0x00, 0xEF, 0x70, 0xF2, 0xE5, 0x0C, 0x30, 0xE7, 0x05, 0x43, 0xA0, 0x08,
0x80, 0x03, 0x53, 0xA0, 0xF7, 0xE5, 0x0C, 0x25, 0xE0, 0xF5, 0x0C, 0xE5,
0xA0, 0x20, 0xE4, 0x04, 0x7F, 0x01, 0x80, 0x02, 0x7F, 0x00, 0xEF, 0x70,
0xF2, 0xE5, 0x0D, 0x25, 0xE0, 0xF5, 0x0D, 0xE5, 0xA0, 0x30, 0xE2, 0x04,
0x7F, 0x01, 0x80, 0x02, 0x7F, 0x00, 0xEF, 0x60, 0x03, 0x43, 0x0D, 0x01,
0x15, 0x0E, 0xAF, 0x0E, 0xEF, 0x70, 0xB3, 0xAF, 0x0D, 0x22, 0x8F, 0x13,
0x75, 0xFF, 0x01, 0xE5, 0xC4, 0x20, 0xE7, 0x04, 0x7F, 0x01, 0x80, 0x02,
0x7F, 0x00, 0xEF, 0x60, 0x2F, 0x75, 0xFF, 0x01, 0x85, 0x13, 0xC5, 0x75,
0xFF, 0x01, 0x53, 0xC4, 0xFC, 0x75, 0xFF, 0x01, 0x53, 0xC4, 0xFB, 0x75,
0xFF, 0x01, 0x43, 0xC4, 0x80, 0x75, 0xFF, 0x01, 0xE5, 0xC4, 0x30, 0xE7,
0x04, 0x7F, 0x01, 0x80, 0x02, 0x7F, 0x00, 0xEF, 0x70, 0xEF, 0x75, 0xFF,
0x01, 0xAF, 0xC6, 0x22, 0x7F, 0x56, 0x22, 0x8F, 0x0F, 0x8D, 0x10, 0x75,
0xFF, 0x01, 0xE5, 0xC4, 0x20, 0xE7, 0x04, 0x7F, 0x01, 0x80, 0x02, 0x7F,
0x00, 0xEF, 0x60, 0x29, 0x75, 0xFF, 0x01, 0x85, 0x0F, 0xC5, 0x75, 0xFF,
0x01, 0x85, 0x10, 0xC6, 0x75, 0xFF, 0x01, 0x53, 0xC4, 0xFC, 0x75, 0xFF,
0x01, 0x43, 0xC4, 0x84, 0x75, 0xFF, 0x01, 0xE5, 0xC4, 0x30, 0xE7, 0x04,
0x7F, 0x01, 0x80, 0x02, 0x7F, 0x00, 0xEF, 0x70, 0xEF, 0x22, 0x75, 0xFF,
0x01, 0xE5, 0xC4, 0x20, 0xE7, 0x04, 0x7F, 0x01, 0x80, 0x02, 0x7F, 0x00,
0xEF, 0x60, 0x2A, 0x75, 0xFF, 0x01, 0xE5, 0xC4, 0x54, 0xFC, 0xFF, 0xEF,
0x44, 0x02, 0xF5, 0xC4, 0x75, 0xFF, 0x01, 0x53, 0xC4, 0xFB, 0x75, 0xFF,
0x01, 0x43, 0xC4, 0x80, 0x75, 0xFF, 0x01, 0xE5, 0xC4, 0x30, 0xE7, 0x04,
0x7F, 0x01, 0x80, 0x02, 0x7F, 0x00, 0xEF, 0x70, 0xEF, 0x22, 0x75, 0xFF,
0x01, 0xE5, 0xC4, 0x20, 0xE7, 0x04, 0x7F, 0x01, 0x80, 0x02, 0x7F, 0x00,
0xEF, 0x60, 0x24, 0x75, 0xFF, 0x01, 0xE5, 0xC4, 0x54, 0xFC, 0xFF, 0xEF,
0x44, 0x02, 0xF5, 0xC4, 0x75, 0xFF, 0x01, 0x43, 0xC4, 0x84, 0x75, 0xFF,
0x01, 0xE5, 0xC4, 0x30, 0xE7, 0x04, 0x7F, 0x01, 0x80, 0x02, 0x7F, 0x00,
0xEF, 0x70, 0xEF, 0x22, 0x8F, 0x11, 0x75, 0xFF, 0x01, 0xE5, 0xC4, 0x20,
0xE7, 0x04, 0x7F, 0x01, 0x80, 0x02, 0x7F, 0x00, 0xEF, 0x60, 0x18, 0x75,
0x12, 0x00, 0xE5, 0x12, 0xC3, 0x94, 0x40, 0x50, 0x0B, 0xAD, 0x11, 0xAF,
0x12, 0x12, 0x01, 0xCF, 0x05, 0x12, 0x80, 0xEE, 0x12, 0x02, 0x4A, 0x22,
0x75, 0xFF, 0x01, 0x53, 0xC4, 0xDF, 0x75, 0xFF, 0x01, 0x43, 0xC4, 0x08,
0x75, 0xFF, 0x01, 0xE5, 0xC4, 0x30, 0xE7, 0x04, 0x7F, 0x01, 0x80, 0x02,
0x7F, 0x00, 0xEF, 0x70, 0xEF, 0x22, 0x75, 0xFF, 0x01, 0x53, 0xCA, 0xF7,
0x75, 0xFF, 0x01, 0x43, 0xCA, 0x14, 0x22, 0x78, 0x7F, 0xE4, 0xF6, 0xD8,
0xFD, 0x75, 0x81, 0x14, 0x02, 0x03, 0x1E, 0x02, 0x00, 0x03, 0xE4, 0x93,
0xA3, 0xF8, 0xE4, 0x93, 0xA3, 0x40, 0x03, 0xF6, 0x80, 0x01, 0xF2, 0x08,
0xDF, 0xF4, 0x80, 0x29, 0xE4, 0x93, 0xA3, 0xF8, 0x54, 0x07, 0x24, 0x0C,
0xC8, 0xC3, 0x33, 0xC4, 0x54, 0x0F, 0x44, 0x20, 0xC8, 0x83, 0x40, 0x04,
0xF4, 0x56, 0x80, 0x01, 0x46, 0xF6, 0xDF, 0xE4, 0x80, 0x0B, 0x01, 0x02,
0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x90, 0x03, 0x89, 0xE4, 0x7E, 0x01,
0x93, 0x60, 0xBC, 0xA3, 0xFF, 0x54, 0x3F, 0x30, 0xE5, 0x09, 0x54, 0x1F,
0xFE, 0xE4, 0x93, 0xA3, 0x60, 0x01, 0x0E, 0xCF, 0x54, 0xC0, 0x25, 0xE0,
0x60, 0xA8, 0x40, 0xB8, 0xE4, 0x93, 0xA3, 0xFA, 0xE4, 0x93, 0xA3, 0xF8,
0xE4, 0x93, 0xA3, 0xC8, 0xC5, 0x82, 0xC8, 0xCA, 0xC5, 0x83, 0xCA, 0xF0,
0xA3, 0xC8, 0xC5, 0x82, 0xC8, 0xCA, 0xC5, 0x83, 0xCA, 0xDF, 0xE9, 0xDE,
0xE7, 0x80, 0xBE, 0xD0, 0x83, 0xD0, 0x82, 0xF8, 0xE4, 0x93, 0x70, 0x12,
0x74, 0x01, 0x93, 0x70, 0x0D, 0xA3, 0xA3, 0x93, 0xF8, 0x74, 0x01, 0x93,
0xF5, 0x82, 0x88, 0x83, 0xE4, 0x73, 0x74, 0x02, 0x93, 0x68, 0x60, 0xEF,
0xA3, 0xA3, 0xA3, 0x80, 0xDF, 0x01, 0x14, 0x00, 0x00,
};
const unsigned long MtpHandler_termination = 0x00000000;
const unsigned long MtpHandler_start       = 0x00000000;
const unsigned long MtpHandler_finish      = 0x0000038D;
const unsigned long MtpHandler_length      = 0x0000038D;

#define MTPHANDLER_TERMINATION 0x00000000
#define MTPHANDLER_START       0x00000000
#define MTPHANDLER_FINISH      0x0000038D
#define MTPHANDLER_LENGTH      0x0000038D