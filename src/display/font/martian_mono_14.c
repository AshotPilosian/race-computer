/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts: --bpp 1 --size 14 --no-compress --font MartianMono-Regular.ttf --range 32-47,48-57,58-63,65-90,91-95,96,97-122,123-126 --format lvgl -o martian_mono_14.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef MARTIAN_MONO_14
#define MARTIAN_MONO_14 1
#endif

#if MARTIAN_MONO_14

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0x3,

    /* U+0022 "\"" */
    0xde, 0xf7, 0xbd, 0x80,

    /* U+0023 "#" */
    0x1b, 0x9, 0x4, 0x8f, 0xf1, 0x21, 0xb0, 0xd8,
    0x48, 0xff, 0x12, 0x9, 0xd, 0x80,

    /* U+0024 "$" */
    0x10, 0x3c, 0x7e, 0xd3, 0xd3, 0xd0, 0x78, 0x3e,
    0x13, 0x13, 0xd3, 0x7e, 0x3c, 0x10,

    /* U+0025 "%" */
    0x70, 0x6c, 0x36, 0x3b, 0x37, 0x70, 0x70, 0xe0,
    0xee, 0xcd, 0xc6, 0xc3, 0x60, 0xe0,

    /* U+0026 "&" */
    0x3c, 0x33, 0x19, 0x8d, 0xc3, 0xc1, 0x81, 0xe5,
    0x9a, 0xcf, 0x63, 0xb1, 0xcf, 0xe0,

    /* U+0027 "'" */
    0xff, 0xc0,

    /* U+0028 "(" */
    0x19, 0x98, 0xcc, 0x63, 0x18, 0xc6, 0x30, 0xc6,
    0x18, 0x60,

    /* U+0029 ")" */
    0xc3, 0xc, 0x61, 0x8c, 0x63, 0x18, 0xc6, 0x63,
    0x33, 0x0,

    /* U+002A "*" */
    0x18, 0x18, 0xdb, 0xff, 0x18, 0x3c, 0x66, 0x4,

    /* U+002B "+" */
    0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0x18,

    /* U+002C "," */
    0xfd, 0x70,

    /* U+002D "-" */
    0xff,

    /* U+002E "." */
    0xfc,

    /* U+002F "/" */
    0xc, 0x30, 0x86, 0x18, 0x43, 0xc, 0x21, 0x86,
    0x10, 0xc3, 0x8, 0x0,

    /* U+0030 "0" */
    0x3c, 0x66, 0xc2, 0xc7, 0xcf, 0xdb, 0xd3, 0xf3,
    0xe3, 0x43, 0x66, 0x3c,

    /* U+0031 "1" */
    0x39, 0xf3, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18,
    0x30, 0x67, 0xf0,

    /* U+0032 "2" */
    0x3c, 0x76, 0x63, 0xe3, 0x3, 0x7, 0x6, 0xe,
    0x1c, 0x70, 0x60, 0x7f,

    /* U+0033 "3" */
    0x3c, 0x67, 0x43, 0x3, 0x6, 0x1c, 0x6, 0x3,
    0x43, 0x43, 0x66, 0x3c,

    /* U+0034 "4" */
    0xe, 0xf, 0xd, 0x86, 0xc6, 0x63, 0x33, 0x19,
    0xff, 0x6, 0x3, 0x1, 0x80, 0xc0,

    /* U+0035 "5" */
    0xff, 0x83, 0x6, 0xd, 0xdd, 0xb1, 0x83, 0x7,
    0x8f, 0x33, 0xc0,

    /* U+0036 "6" */
    0xc, 0x18, 0x10, 0x30, 0x7c, 0x66, 0xc3, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+0037 "7" */
    0xfe, 0x4, 0x18, 0x20, 0x41, 0x82, 0xc, 0x18,
    0x20, 0xc1, 0x0,

    /* U+0038 "8" */
    0x3c, 0xe7, 0xc3, 0xc3, 0x66, 0x3c, 0x66, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+0039 "9" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3e,
    0xc, 0x8, 0x18, 0x30,

    /* U+003A ":" */
    0xfc, 0xf, 0xc0,

    /* U+003B ";" */
    0xfc, 0xf, 0xd7,

    /* U+003C "<" */
    0x2, 0x1c, 0xf7, 0x8c, 0x18, 0x1c, 0xe, 0x6,

    /* U+003D "=" */
    0xfe, 0x0, 0x0, 0xf, 0xe0,

    /* U+003E ">" */
    0x81, 0xc1, 0xe0, 0xf0, 0x60, 0xc7, 0x38, 0xc0,

    /* U+003F "?" */
    0x38, 0x9b, 0x18, 0x30, 0x61, 0xc7, 0x1c, 0x30,
    0x60, 0x1, 0x80,

    /* U+0041 "A" */
    0x38, 0x3c, 0x3c, 0x2c, 0x24, 0x66, 0x66, 0x7e,
    0x42, 0xc2, 0xc3, 0x83,

    /* U+0042 "B" */
    0xf9, 0x9b, 0x1e, 0x3c, 0x79, 0xbe, 0x63, 0xc7,
    0x8f, 0x1f, 0xe0,

    /* U+0043 "C" */
    0x3c, 0x66, 0x43, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc3, 0xc2, 0x66, 0x3c,

    /* U+0044 "D" */
    0xfc, 0xc6, 0xc6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc6, 0xc6, 0xfc,

    /* U+0045 "E" */
    0xff, 0x83, 0x6, 0xc, 0x18, 0x3f, 0x60, 0xc1,
    0x83, 0x7, 0xf0,

    /* U+0046 "F" */
    0xff, 0x83, 0x6, 0xc, 0x18, 0x3f, 0x60, 0xc1,
    0x83, 0x6, 0x0,

    /* U+0047 "G" */
    0x3c, 0x62, 0xc3, 0xc1, 0xc0, 0xc0, 0xcf, 0xc3,
    0xc3, 0x43, 0x67, 0x3f,

    /* U+0048 "H" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xff, 0xe3, 0xc7,
    0x8f, 0x1e, 0x30,

    /* U+0049 "I" */
    0xfe, 0x30, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18,
    0x30, 0x67, 0xf0,

    /* U+004A "J" */
    0x7e, 0xc, 0x18, 0x30, 0x60, 0xc1, 0xe3, 0xc7,
    0x8f, 0xb3, 0xc0,

    /* U+004B "K" */
    0xc6, 0xc6, 0xcc, 0xd8, 0xd8, 0xf0, 0xf0, 0xf0,
    0xd8, 0xcc, 0xcc, 0xc6,

    /* U+004C "L" */
    0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
    0x83, 0x7, 0xf0,

    /* U+004D "M" */
    0xe7, 0xe7, 0xe7, 0xe7, 0xdb, 0xdb, 0xdb, 0xdb,
    0xc3, 0xc3, 0xc3, 0xc3,

    /* U+004E "N" */
    0xe3, 0xf3, 0xf3, 0xd3, 0xd3, 0xdb, 0xdb, 0xcb,
    0xcb, 0xcf, 0xcf, 0xc7,

    /* U+004F "O" */
    0x3c, 0x66, 0xc2, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0x43, 0x66, 0x3c,

    /* U+0050 "P" */
    0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc6, 0xfc, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0051 "Q" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0x66, 0x7e, 0x8, 0xe, 0x6,

    /* U+0052 "R" */
    0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc6, 0xfc, 0xc8,
    0xcc, 0xc6, 0xc6, 0xc3,

    /* U+0053 "S" */
    0x3c, 0x67, 0xc3, 0xc0, 0xe0, 0xfe, 0x7f, 0x7,
    0x3, 0xc3, 0xe6, 0x7c,

    /* U+0054 "T" */
    0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18,

    /* U+0055 "U" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0x66, 0x3c,

    /* U+0056 "V" */
    0xc3, 0xc3, 0xc2, 0x42, 0x46, 0x66, 0x66, 0x24,
    0x24, 0x3c, 0x3c, 0x38,

    /* U+0057 "W" */
    0x81, 0xc0, 0xe0, 0x53, 0x2d, 0x96, 0xeb, 0x75,
    0xaa, 0x57, 0x33, 0x99, 0xcc, 0xc0,

    /* U+0058 "X" */
    0xc3, 0xc2, 0x66, 0x66, 0x2c, 0x3c, 0x18, 0x3c,
    0x2c, 0x66, 0x46, 0xc3,

    /* U+0059 "Y" */
    0x40, 0x98, 0x62, 0x10, 0xcc, 0x12, 0x7, 0x80,
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30,

    /* U+005A "Z" */
    0xfe, 0x4, 0x18, 0x60, 0xc3, 0xc, 0x30, 0x61,
    0x82, 0x7, 0xf0,

    /* U+005B "[" */
    0xfe, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x31, 0x8c,
    0x63, 0xe0,

    /* U+005C "\\" */
    0x83, 0x4, 0x10, 0x61, 0x82, 0xc, 0x30, 0x41,
    0x86, 0x8, 0x30, 0xc0,

    /* U+005D "]" */
    0xf8, 0xc6, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x31,
    0x8f, 0xe0,

    /* U+005E "^" */
    0x31, 0xc5, 0xb2, 0x8c,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0xc6, 0x23,

    /* U+0061 "a" */
    0x7d, 0x9e, 0x18, 0x77, 0xfc, 0xf1, 0xe7, 0x76,

    /* U+0062 "b" */
    0xc0, 0xc0, 0xc0, 0xdc, 0xe6, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0xe6, 0xdc,

    /* U+0063 "c" */
    0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xc0, 0xc2, 0x66,
    0x3c,

    /* U+0064 "d" */
    0x3, 0x3, 0x3, 0x3b, 0x67, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc3, 0x67, 0x3b,

    /* U+0065 "e" */
    0x38, 0xcb, 0x1e, 0x3f, 0xf8, 0x30, 0xa3, 0x3c,

    /* U+0066 "f" */
    0x1e, 0x32, 0x30, 0xfe, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x30, 0x30,

    /* U+0067 "g" */
    0x76, 0xdf, 0x1e, 0x3c, 0x78, 0xdb, 0xbb, 0x7,
    0x8f, 0x3b, 0xe0,

    /* U+0068 "h" */
    0xc1, 0x83, 0x7, 0xec, 0xf8, 0xf1, 0xe3, 0xc7,
    0x8f, 0x1e, 0x30,

    /* U+0069 "i" */
    0x30, 0x0, 0x7, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc7, 0xf0,

    /* U+006A "j" */
    0x18, 0x1, 0xf1, 0x8c, 0x63, 0x18, 0xc6, 0x31,
    0xcf, 0xc0,

    /* U+006B "k" */
    0xc1, 0x83, 0x6, 0x3c, 0xdb, 0x3c, 0x78, 0xf1,
    0xb3, 0x36, 0x30,

    /* U+006C "l" */
    0xf0, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xc7, 0xf0,

    /* U+006D "m" */
    0xfe, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb, 0xdb,
    0xdb,

    /* U+006E "n" */
    0xfd, 0x9f, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc6,

    /* U+006F "o" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66,
    0x3c,

    /* U+0070 "p" */
    0xdc, 0xe6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe6,
    0xdc, 0xc0, 0xc0, 0xc0,

    /* U+0071 "q" */
    0x3b, 0x67, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x67,
    0x3b, 0x3, 0x3, 0x3,

    /* U+0072 "r" */
    0xee, 0x73, 0x63, 0x63, 0x60, 0x60, 0x60, 0x60,
    0xfc,

    /* U+0073 "s" */
    0x7d, 0x8f, 0x1f, 0xc7, 0xc7, 0xf1, 0xe3, 0x7c,

    /* U+0074 "t" */
    0x30, 0x60, 0xc7, 0xf3, 0x6, 0xc, 0x18, 0x30,
    0x64, 0xc8, 0xe0,

    /* U+0075 "u" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe7, 0x7e,

    /* U+0076 "v" */
    0xc3, 0xc2, 0x42, 0x66, 0x64, 0x24, 0x2c, 0x3c,
    0x18,

    /* U+0077 "w" */
    0x4c, 0xa6, 0x53, 0x29, 0xb7, 0x5b, 0xb8, 0xcc,
    0x66, 0x33, 0x0,

    /* U+0078 "x" */
    0xc3, 0x66, 0x64, 0x3c, 0x18, 0x3c, 0x24, 0x66,
    0xc2,

    /* U+0079 "y" */
    0xc3, 0xc2, 0x42, 0x66, 0x64, 0x24, 0x3c, 0x3c,
    0x8, 0x18, 0xd8, 0x70,

    /* U+007A "z" */
    0xfc, 0x10, 0xc6, 0x31, 0x8c, 0x20, 0xfc,

    /* U+007B "{" */
    0x1c, 0xe3, 0xc, 0x30, 0xc6, 0x30, 0x60, 0xc3,
    0xc, 0x30, 0xe1, 0xc0,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xc0,

    /* U+007D "}" */
    0xe1, 0xc3, 0xc, 0x30, 0xc1, 0x83, 0x18, 0xc3,
    0xc, 0x31, 0xce, 0x0,

    /* U+007E "~" */
    0x70, 0xdb, 0xe
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 157, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 157, .box_w = 2, .box_h = 12, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 157, .box_w = 5, .box_h = 5, .ofs_x = 3, .ofs_y = 7},
    {.bitmap_index = 8, .adv_w = 157, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 157, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 36, .adv_w = 157, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 50, .adv_w = 157, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 157, .box_w = 2, .box_h = 5, .ofs_x = 4, .ofs_y = 7},
    {.bitmap_index = 66, .adv_w = 157, .box_w = 5, .box_h = 15, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 76, .adv_w = 157, .box_w = 5, .box_h = 15, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 86, .adv_w = 157, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 94, .adv_w = 157, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 102, .adv_w = 157, .box_w = 2, .box_h = 6, .ofs_x = 4, .ofs_y = -3},
    {.bitmap_index = 104, .adv_w = 157, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 105, .adv_w = 157, .box_w = 2, .box_h = 3, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 157, .box_w = 6, .box_h = 15, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 118, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 130, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 153, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 165, .adv_w = 157, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 179, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 190, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 202, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 225, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 237, .adv_w = 157, .box_w = 2, .box_h = 9, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 240, .adv_w = 157, .box_w = 2, .box_h = 12, .ofs_x = 4, .ofs_y = -3},
    {.bitmap_index = 243, .adv_w = 157, .box_w = 7, .box_h = 9, .ofs_x = 2, .ofs_y = 1},
    {.bitmap_index = 251, .adv_w = 157, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 256, .adv_w = 157, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 264, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 275, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 287, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 298, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 310, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 333, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 356, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 367, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 378, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 389, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 401, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 412, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 424, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 436, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 448, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 460, .adv_w = 157, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 474, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 486, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 522, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 534, .adv_w = 157, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 548, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 560, .adv_w = 157, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 586, .adv_w = 157, .box_w = 5, .box_h = 15, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 596, .adv_w = 157, .box_w = 6, .box_h = 15, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 608, .adv_w = 157, .box_w = 5, .box_h = 15, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 618, .adv_w = 157, .box_w = 6, .box_h = 5, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 622, .adv_w = 157, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 623, .adv_w = 157, .box_w = 4, .box_h = 4, .ofs_x = 3, .ofs_y = 8},
    {.bitmap_index = 625, .adv_w = 157, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 633, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 645, .adv_w = 157, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 654, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 666, .adv_w = 157, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 674, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 686, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 697, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 708, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 719, .adv_w = 157, .box_w = 5, .box_h = 15, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 729, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 740, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 751, .adv_w = 157, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 760, .adv_w = 157, .box_w = 7, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 768, .adv_w = 157, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 777, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 789, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 801, .adv_w = 157, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 810, .adv_w = 157, .box_w = 7, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 818, .adv_w = 157, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 829, .adv_w = 157, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 837, .adv_w = 157, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 846, .adv_w = 157, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 857, .adv_w = 157, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 866, .adv_w = 157, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 878, .adv_w = 157, .box_w = 6, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 885, .adv_w = 157, .box_w = 6, .box_h = 15, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 897, .adv_w = 157, .box_w = 2, .box_h = 13, .ofs_x = 4, .ofs_y = -1},
    {.bitmap_index = 901, .adv_w = 157, .box_w = 6, .box_h = 15, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 913, .adv_w = 157, .box_w = 8, .box_h = 3, .ofs_x = 1, .ofs_y = 4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 32, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 65, .range_length = 62, .glyph_id_start = 33,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t lv_font_montserrat_14;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t martian_mono_14 = {
#else
lv_font_t martian_mono_14 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_montserrat_14,
#endif
    .user_data = NULL,
};



#endif /*#if MARTIAN_MONO_14*/

