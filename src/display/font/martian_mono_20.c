/*******************************************************************************
 * Size: 20 px
 * Bpp: 1
 * Opts: --bpp 1 --size 20 --no-compress --font MartianMono-Regular.ttf --symbols 0123456789.+- --format lvgl -o martian_mono_20.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef MARTIAN_MONO_20
#define MARTIAN_MONO_20 1
#endif

#if MARTIAN_MONO_20

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+002B "+" */
    0x6, 0x0, 0xc0, 0x18, 0x3, 0x0, 0x61, 0xff,
    0xff, 0xf8, 0x30, 0x6, 0x0, 0xc0, 0x18, 0x0,

    /* U+002D "-" */
    0xff, 0xff, 0xfc,

    /* U+002E "." */
    0xff, 0xf0,

    /* U+0030 "0" */
    0x1f, 0x83, 0xfc, 0x70, 0xe6, 0x6, 0xc0, 0x7c,
    0xf, 0xc3, 0xfc, 0x7b, 0xde, 0x3f, 0xc3, 0xf0,
    0x3e, 0x3, 0x60, 0x67, 0xe, 0x3f, 0xc1, 0xf8,

    /* U+0031 "1" */
    0x3c, 0xf, 0x83, 0xb0, 0x66, 0x0, 0xc0, 0x18,
    0x3, 0x0, 0x60, 0xc, 0x1, 0x80, 0x30, 0x6,
    0x0, 0xc0, 0x18, 0x3f, 0xff, 0xff,

    /* U+0032 "2" */
    0x1f, 0x7, 0xf9, 0xc3, 0x30, 0x3e, 0x6, 0x0,
    0xc0, 0x18, 0x6, 0x1, 0xc0, 0xf0, 0x3c, 0xe,
    0x7, 0x80, 0xc0, 0x1f, 0xfb, 0xff,

    /* U+0033 "3" */
    0x1f, 0x1f, 0xe6, 0x1f, 0x83, 0xc0, 0xc0, 0x30,
    0x18, 0x7e, 0x1f, 0x80, 0x60, 0xf, 0x3, 0xc0,
    0xf8, 0x77, 0xf8, 0xfc,

    /* U+0034 "4" */
    0x7, 0x80, 0xf8, 0xd, 0x81, 0x98, 0x39, 0x83,
    0x18, 0x71, 0x8e, 0x18, 0xc1, 0x8f, 0xff, 0xff,
    0xf0, 0x18, 0x1, 0x80, 0x18, 0x1, 0x80, 0x18,

    /* U+0035 "5" */
    0x7f, 0xef, 0xfd, 0x80, 0x30, 0x6, 0x0, 0xde,
    0x1f, 0xf3, 0x86, 0x60, 0x60, 0xc, 0x1, 0xe0,
    0x36, 0xe, 0xe1, 0x8f, 0xe0, 0xf8,

    /* U+0036 "6" */
    0x3, 0x0, 0xe0, 0x38, 0xe, 0x1, 0xc0, 0x70,
    0x1f, 0xc3, 0xfe, 0xe0, 0xd8, 0xf, 0x1, 0xe0,
    0x3c, 0x6, 0xc1, 0x8f, 0xf0, 0xf8,

    /* U+0037 "7" */
    0xff, 0xff, 0xf0, 0xc, 0x7, 0x1, 0x80, 0xe0,
    0x38, 0xc, 0x7, 0x1, 0x80, 0xe0, 0x30, 0x1c,
    0x6, 0x3, 0x80, 0xc0,

    /* U+0038 "8" */
    0x1f, 0xf, 0xfb, 0x83, 0xe0, 0x3c, 0x7, 0x80,
    0xd8, 0x31, 0xfc, 0x7f, 0xcc, 0x1b, 0x1, 0xe0,
    0x3c, 0x7, 0xc1, 0xdf, 0xf0, 0xf8,

    /* U+0039 "9" */
    0x1f, 0x7, 0xf1, 0xc7, 0x70, 0x7c, 0x7, 0x80,
    0xf0, 0x1f, 0x7, 0x71, 0xcf, 0xf8, 0x7e, 0x1,
    0x80, 0x70, 0x1c, 0x3, 0x0, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 224, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 16, .adv_w = 224, .box_w = 11, .box_h = 2, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 19, .adv_w = 224, .box_w = 4, .box_h = 3, .ofs_x = 5, .ofs_y = 0},
    {.bitmap_index = 21, .adv_w = 224, .box_w = 12, .box_h = 16, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 45, .adv_w = 224, .box_w = 11, .box_h = 16, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 67, .adv_w = 224, .box_w = 11, .box_h = 16, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 89, .adv_w = 224, .box_w = 10, .box_h = 16, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 109, .adv_w = 224, .box_w = 12, .box_h = 16, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 133, .adv_w = 224, .box_w = 11, .box_h = 16, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 155, .adv_w = 224, .box_w = 11, .box_h = 16, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 177, .adv_w = 224, .box_w = 10, .box_h = 16, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 197, .adv_w = 224, .box_w = 11, .box_h = 16, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 219, .adv_w = 224, .box_w = 11, .box_h = 16, .ofs_x = 2, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 0, 1, 2, 0, 3, 4, 5,
    6, 7, 8, 9, 10, 11, 12
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 43, .range_length = 15, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 15, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
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
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};

extern const lv_font_t lv_font_montserrat_24;


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t martian_mono_20 = {
#else
lv_font_t martian_mono_20 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = &lv_font_montserrat_24,
#endif
    .user_data = NULL,
};



#endif /*#if MARTIAN_MONO_20*/

