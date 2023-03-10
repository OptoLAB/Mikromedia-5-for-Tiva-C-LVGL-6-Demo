/*******************************************************************************
 * Size: 60 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef NI7SEG_60
#define NI7SEG_60 1
#endif

#if NI7SEG_60

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+002E "." */
    0x8, 0xe, 0xf, 0x8f, 0xef, 0xfb, 0xf8, 0xf8,
    0x38, 0x8, 0x0,

    /* U+0030 "0" */
    0x7, 0xff, 0xff, 0xfe, 0x0, 0x3f, 0xff, 0xff,
    0xf0, 0x1, 0xff, 0xff, 0xff, 0x80, 0xf, 0xff,
    0xff, 0xfc, 0x0, 0x7f, 0xff, 0xff, 0xe0, 0x7d,
    0xff, 0xff, 0xfe, 0x7f, 0xf7, 0xff, 0xff, 0xe7,
    0xff, 0xdf, 0xff, 0xfe, 0x7f, 0xff, 0x7f, 0xff,
    0xe7, 0xff, 0xfc, 0x0, 0x0, 0x7f, 0xff, 0xe0,
    0x0, 0x3, 0xff, 0xff, 0x0, 0x0, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x7,
    0xff, 0xfe, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0x0,
    0x1, 0xff, 0xff, 0x80, 0x0, 0xf, 0xff, 0xfc,
    0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x3, 0xff,
    0xff, 0x0, 0x0, 0x1f, 0xff, 0xf8, 0x0, 0x0,
    0xff, 0xff, 0xc0, 0x0, 0x7, 0xff, 0xfe, 0x0,
    0x0, 0x3f, 0xef, 0xe0, 0x0, 0x0, 0xfe, 0x3e,
    0x0, 0x0, 0x7, 0xe1, 0xe0, 0x0, 0x0, 0x1e,
    0x6, 0x0, 0x0, 0x0, 0x60, 0x78, 0x0, 0x0,
    0x7, 0x87, 0xe0, 0x0, 0x0, 0x7e, 0x3f, 0x80,
    0x0, 0x7, 0xfb, 0xfe, 0x0, 0x0, 0x3f, 0xff,
    0xf0, 0x0, 0x1, 0xff, 0xff, 0x80, 0x0, 0xf,
    0xff, 0xfc, 0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0,
    0x3, 0xff, 0xff, 0x0, 0x0, 0x1f, 0xff, 0xf8,
    0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x7, 0xff,
    0xfe, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0x0, 0x1,
    0xff, 0xff, 0x80, 0x0, 0xf, 0xff, 0xfc, 0x0,
    0x0, 0x7f, 0xff, 0x9f, 0xff, 0xfd, 0xff, 0xf9,
    0xff, 0xff, 0xf7, 0xff, 0x9f, 0xff, 0xff, 0xdf,
    0x81, 0xff, 0xff, 0xff, 0x0, 0x1f, 0xff, 0xff,
    0xf8, 0x0, 0xff, 0xff, 0xff, 0xc0, 0x7, 0xff,
    0xff, 0xfe, 0x0, 0x3f, 0xff, 0xff, 0xf0, 0x1,
    0xff, 0xff, 0xff, 0x80,

    /* U+0031 "1" */
    0xf, 0xfc, 0x7, 0xfe, 0x73, 0xff, 0x3d, 0xff,
    0x9f, 0xff, 0xcf, 0xdf, 0xe7, 0xf3, 0xf3, 0xfc,
    0x79, 0xff, 0xc, 0x7f, 0xc0, 0x1f, 0xf0, 0x7,
    0xfc, 0x1, 0xff, 0x0, 0x7f, 0xc0, 0x1f, 0xf0,
    0x7, 0xfc, 0x1, 0xff, 0x0, 0x7f, 0xc0, 0x1f,
    0xf0, 0x7, 0xfc, 0x1, 0xff, 0x0, 0x7f, 0xc0,
    0x1f, 0xf0, 0x3, 0xf8, 0x0, 0x7c, 0x0, 0xe,
    0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x10, 0x0,
    0xe, 0x0, 0x7, 0xc0, 0x3, 0xf8, 0x1, 0xff,
    0x0, 0x7f, 0xc0, 0x1f, 0xf0, 0x7, 0xfc, 0x1,
    0xff, 0x0, 0x7f, 0xc0, 0x1f, 0xf0, 0x7, 0xfc,
    0x1, 0xff, 0x0, 0x7f, 0xc0, 0x1f, 0xf0, 0x7,
    0xfc, 0x1, 0xff, 0x0, 0x7f, 0xc0, 0x1f, 0xf0,
    0x7, 0xfc, 0x0, 0xfe, 0x0, 0x1f, 0x0, 0x3,
    0x80, 0x0, 0x40,

    /* U+0032 "2" */
    0xf, 0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff,
    0xfc, 0xf, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff,
    0xff, 0xff, 0xf, 0xff, 0xff, 0xff, 0xf3, 0xbf,
    0xff, 0xff, 0xff, 0x3d, 0xff, 0xff, 0xff, 0xf3,
    0xe7, 0xff, 0xff, 0xff, 0x3f, 0x1f, 0xff, 0xff,
    0xf3, 0xf8, 0x7f, 0xff, 0xff, 0x3f, 0xc0, 0x0,
    0x0, 0x3, 0xfe, 0x0, 0x0, 0x0, 0x1f, 0xf0,
    0x0, 0x0, 0x0, 0xff, 0x80, 0x0, 0x0, 0x7,
    0xfc, 0x0, 0x0, 0x0, 0x3f, 0xe0, 0x0, 0x0,
    0x1, 0xff, 0x0, 0x0, 0x0, 0xf, 0xf8, 0x0,
    0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0, 0x3, 0xfe,
    0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x0,
    0xff, 0x80, 0x0, 0x0, 0x7, 0xfc, 0x3, 0xff,
    0xff, 0x9f, 0xe0, 0x3f, 0xff, 0xfe, 0x7f, 0x3,
    0xff, 0xff, 0xf9, 0xf8, 0x3f, 0xff, 0xff, 0xe7,
    0xc1, 0xff, 0xff, 0xff, 0x9e, 0x27, 0xff, 0xff,
    0xfe, 0x3, 0x9f, 0xff, 0xff, 0xf0, 0x3e, 0x7f,
    0xff, 0xff, 0x83, 0xf9, 0xff, 0xff, 0xfc, 0x1f,
    0xe0, 0x0, 0x0, 0x0, 0xff, 0x80, 0x0, 0x0,
    0x7, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xe0, 0x0,
    0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0xf, 0xf8,
    0x0, 0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0, 0x3,
    0xfe, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0,
    0x0, 0xff, 0x80, 0x0, 0x0, 0x7, 0xfc, 0x0,
    0x0, 0x0, 0x3f, 0xdf, 0xff, 0xff, 0xe1, 0xfd,
    0xff, 0xff, 0xff, 0x8f, 0xdf, 0xff, 0xff, 0xfe,
    0x7d, 0xff, 0xff, 0xff, 0xfb, 0xdf, 0xff, 0xff,
    0xff, 0xe1, 0xff, 0xff, 0xff, 0xfe, 0xf, 0xff,
    0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xfe, 0x3,
    0xff, 0xff, 0xff, 0xe0,

    /* U+0033 "3" */
    0xf, 0xff, 0xff, 0xff, 0x1, 0xff, 0xff, 0xff,
    0xf0, 0x3f, 0xff, 0xff, 0xff, 0x7, 0xff, 0xff,
    0xff, 0xf0, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff,
    0xff, 0xff, 0xcf, 0x7f, 0xff, 0xff, 0xf9, 0xf3,
    0xff, 0xff, 0xff, 0x3f, 0x1f, 0xff, 0xff, 0xe7,
    0xf0, 0x0, 0x0, 0x0, 0xff, 0x0, 0x0, 0x0,
    0x1f, 0xf0, 0x0, 0x0, 0x1, 0xff, 0x0, 0x0,
    0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1, 0xff, 0x0,
    0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1, 0xff,
    0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1,
    0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0,
    0x1, 0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x1, 0xff, 0x1f, 0xff, 0xff, 0x9f, 0xf3,
    0xff, 0xff, 0xfc, 0xfe, 0x7f, 0xff, 0xff, 0xe7,
    0xcf, 0xff, 0xff, 0xff, 0x38, 0xff, 0xff, 0xff,
    0xf9, 0x7, 0xff, 0xff, 0xff, 0x38, 0x3f, 0xff,
    0xff, 0xe7, 0xc1, 0xff, 0xff, 0xfc, 0xfe, 0xf,
    0xff, 0xff, 0x9f, 0xf0, 0x0, 0x0, 0x1, 0xff,
    0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1,
    0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0,
    0x1, 0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0,
    0x0, 0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0x1f,
    0xf0, 0x0, 0x0, 0x1, 0xff, 0x1f, 0xff, 0xff,
    0xef, 0xf3, 0xff, 0xff, 0xff, 0x3f, 0x7f, 0xff,
    0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff,
    0xff, 0xff, 0xfe, 0x77, 0xff, 0xff, 0xff, 0xf0,
    0x3f, 0xff, 0xff, 0xff, 0x1, 0xff, 0xff, 0xff,
    0xf0, 0xf, 0xff, 0xff, 0xff, 0x0,

    /* U+0034 "4" */
    0x8, 0x0, 0x0, 0x0, 0x80, 0xe0, 0x0, 0x0,
    0xe, 0xf, 0x80, 0x0, 0x0, 0xf8, 0xfe, 0x0,
    0x0, 0xf, 0xef, 0xf8, 0x0, 0x0, 0xff, 0xff,
    0xc0, 0x0, 0x7, 0xff, 0xfe, 0x0, 0x0, 0x3f,
    0xff, 0xf0, 0x0, 0x1, 0xff, 0xff, 0x80, 0x0,
    0xf, 0xff, 0xfc, 0x0, 0x0, 0x7f, 0xff, 0xe0,
    0x0, 0x3, 0xff, 0xff, 0x0, 0x0, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x7,
    0xff, 0xfe, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0x0,
    0x1, 0xff, 0xff, 0x80, 0x0, 0xf, 0xff, 0xfc,
    0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x3, 0xff,
    0xff, 0x0, 0x0, 0x1f, 0xff, 0xf8, 0x0, 0x0,
    0xff, 0xff, 0xc0, 0x0, 0x7, 0xfd, 0xfe, 0x3f,
    0xff, 0x3f, 0xe7, 0xf3, 0xff, 0xfc, 0xfe, 0x1f,
    0x3f, 0xff, 0xf3, 0xe0, 0x73, 0xff, 0xff, 0xdf,
    0x0, 0x3f, 0xff, 0xff, 0x70, 0x0, 0xff, 0xff,
    0xf3, 0x80, 0x3, 0xff, 0xff, 0x3e, 0x0, 0xf,
    0xff, 0xf3, 0xf8, 0x0, 0x3f, 0xff, 0x3f, 0xe0,
    0x0, 0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0xf,
    0xf8, 0x0, 0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0,
    0x3, 0xfe, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x0, 0xff, 0x80, 0x0, 0x0, 0x7, 0xfc,
    0x0, 0x0, 0x0, 0x3f, 0xe0, 0x0, 0x0, 0x1,
    0xff, 0x0, 0x0, 0x0, 0xf, 0xf8, 0x0, 0x0,
    0x0, 0x7f, 0xc0, 0x0, 0x0, 0x3, 0xfe, 0x0,
    0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x0, 0xff,
    0x80, 0x0, 0x0, 0x7, 0xfc, 0x0, 0x0, 0x0,
    0x3f, 0xe0, 0x0, 0x0, 0x0, 0xfe, 0x0, 0x0,
    0x0, 0x3, 0xe0, 0x0, 0x0, 0x0, 0xe, 0x0,
    0x0, 0x0, 0x0, 0x20,

    /* U+0035 "5" */
    0x7f, 0xff, 0xff, 0xff, 0x5, 0xff, 0xff, 0xff,
    0xfc, 0x37, 0xff, 0xff, 0xff, 0xf1, 0xdf, 0xff,
    0xff, 0xff, 0xcf, 0x7f, 0xff, 0xff, 0xff, 0x7d,
    0xff, 0xff, 0xff, 0xf3, 0xf7, 0xff, 0xff, 0xff,
    0x1f, 0xdf, 0xff, 0xff, 0xf0, 0xff, 0x7f, 0xff,
    0xff, 0x7, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xe0,
    0x0, 0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0xf,
    0xf8, 0x0, 0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0,
    0x3, 0xfe, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x0, 0xff, 0x80, 0x0, 0x0, 0x7, 0xfc,
    0x0, 0x0, 0x0, 0x3f, 0xe0, 0x0, 0x0, 0x0,
    0xff, 0x0, 0x0, 0x0, 0x3, 0xfb, 0xff, 0xfe,
    0x0, 0xf, 0xbf, 0xff, 0xf8, 0x0, 0x3b, 0xff,
    0xff, 0xe0, 0x0, 0xbf, 0xff, 0xff, 0x80, 0x3,
    0xff, 0xff, 0xfe, 0x0, 0xf, 0xff, 0xff, 0xe0,
    0x0, 0x3f, 0xff, 0xfe, 0x60, 0x0, 0xff, 0xff,
    0xe7, 0x80, 0x3, 0xff, 0xfe, 0x7e, 0x0, 0x0,
    0x0, 0x7, 0xf8, 0x0, 0x0, 0x0, 0x3f, 0xe0,
    0x0, 0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0xf,
    0xf8, 0x0, 0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0,
    0x3, 0xfe, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x0, 0xff, 0x80, 0x0, 0x0, 0x7, 0xfc,
    0x0, 0x0, 0x0, 0x3f, 0xe0, 0x0, 0x0, 0x1,
    0xff, 0x0, 0x0, 0x0, 0xf, 0xf8, 0x0, 0x0,
    0x0, 0x3f, 0xc3, 0xff, 0xff, 0xfe, 0xfe, 0x3f,
    0xff, 0xff, 0xfb, 0xf3, 0xff, 0xff, 0xff, 0xef,
    0x3f, 0xff, 0xff, 0xff, 0xb3, 0xff, 0xff, 0xff,
    0xfe, 0xf, 0xff, 0xff, 0xff, 0xf0, 0x3f, 0xff,
    0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0xf0, 0x3,
    0xff, 0xff, 0xff, 0x0,

    /* U+0036 "6" */
    0x7, 0xff, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff,
    0xfe, 0x1, 0xff, 0xff, 0xff, 0xf8, 0xf, 0xff,
    0xff, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xfd,
    0xff, 0xff, 0xff, 0xfb, 0xf7, 0xff, 0xff, 0xff,
    0x9f, 0xdf, 0xff, 0xff, 0xf8, 0xff, 0x7f, 0xff,
    0xff, 0x87, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xe0,
    0x0, 0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0xf,
    0xf8, 0x0, 0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0,
    0x3, 0xfe, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x0, 0xff, 0x80, 0x0, 0x0, 0x7, 0xfc,
    0x0, 0x0, 0x0, 0x3f, 0xe0, 0x0, 0x0, 0x1,
    0xff, 0x0, 0x0, 0x0, 0xf, 0xf8, 0x0, 0x0,
    0x0, 0x7f, 0xc0, 0x0, 0x0, 0x3, 0xfe, 0x7f,
    0xff, 0xc0, 0xf, 0xe7, 0xff, 0xfe, 0x0, 0x3e,
    0x7f, 0xff, 0xf8, 0x1, 0xe7, 0xff, 0xff, 0xe0,
    0x6, 0x7f, 0xff, 0xff, 0x80, 0x79, 0xff, 0xff,
    0xf8, 0x7, 0xe7, 0xff, 0xff, 0x98, 0x3f, 0x9f,
    0xff, 0xf9, 0xe3, 0xfe, 0x7f, 0xff, 0x9f, 0x9f,
    0xf0, 0x0, 0x1, 0xfe, 0xff, 0x80, 0x0, 0xf,
    0xff, 0xfc, 0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0,
    0x3, 0xff, 0xff, 0x0, 0x0, 0x1f, 0xff, 0xf8,
    0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x7, 0xff,
    0xfe, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0x0, 0x1,
    0xff, 0xff, 0x80, 0x0, 0xf, 0xff, 0xfc, 0x0,
    0x0, 0x7f, 0xff, 0x9f, 0xff, 0xfd, 0xff, 0xf9,
    0xff, 0xff, 0xf7, 0xff, 0x9f, 0xff, 0xff, 0xdf,
    0x81, 0xff, 0xff, 0xff, 0x0, 0x1f, 0xff, 0xff,
    0xf8, 0x0, 0xff, 0xff, 0xff, 0xc0, 0x7, 0xff,
    0xff, 0xfe, 0x0, 0x3f, 0xff, 0xff, 0xf0, 0x1,
    0xff, 0xff, 0xff, 0x80,

    /* U+0037 "7" */
    0xf, 0xff, 0xff, 0xff, 0x1, 0xff, 0xff, 0xff,
    0xf0, 0x3f, 0xff, 0xff, 0xff, 0x7, 0xff, 0xff,
    0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xf, 0xff,
    0xff, 0xff, 0xef, 0x7f, 0xff, 0xff, 0xfd, 0xf1,
    0xff, 0xff, 0xff, 0xbf, 0xf, 0xff, 0xff, 0xf7,
    0xf0, 0x0, 0x0, 0x0, 0xff, 0x0, 0x0, 0x0,
    0x1f, 0xf0, 0x0, 0x0, 0x1, 0xff, 0x0, 0x0,
    0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1, 0xff, 0x0,
    0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1, 0xff,
    0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1,
    0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0,
    0x1, 0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0,
    0x0, 0x0, 0x0, 0xfe, 0x0, 0x0, 0x0, 0x7,
    0xc0, 0x0, 0x0, 0x0, 0x38, 0x0, 0x0, 0x0,
    0x3, 0x80, 0x0, 0x0, 0x0, 0x7c, 0x0, 0x0,
    0x0, 0xf, 0xe0, 0x0, 0x0, 0x1, 0xff, 0x0,
    0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1, 0xff,
    0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1,
    0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0,
    0x1, 0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0x1f, 0xf0,
    0x0, 0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0x1f,
    0xf0, 0x0, 0x0, 0x1, 0xff, 0x0, 0x0, 0x0,
    0x1f, 0xf0, 0x0, 0x0, 0x1, 0xff, 0x0, 0x0,
    0x0, 0x1f, 0xf0, 0x0, 0x0, 0x1, 0xff, 0x0,
    0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x0, 0xfe,
    0x0, 0x0, 0x0, 0x7, 0xc0, 0x0, 0x0, 0x0,
    0x38, 0x0, 0x0, 0x0, 0x1, 0x0,

    /* U+0038 "8" */
    0x1, 0xff, 0xff, 0xf8, 0x0, 0x1f, 0xff, 0xff,
    0xe0, 0x1, 0xff, 0xff, 0xff, 0x80, 0xf, 0xff,
    0xff, 0xfc, 0x0, 0x7f, 0xff, 0xff, 0xe0, 0x7d,
    0xff, 0xff, 0xfe, 0x7f, 0xf7, 0xff, 0xff, 0xe7,
    0xff, 0xdf, 0xff, 0xfe, 0x7f, 0xff, 0x7f, 0xff,
    0xe7, 0xff, 0xfc, 0x0, 0x0, 0x7f, 0xff, 0xe0,
    0x0, 0x3, 0xff, 0xff, 0x0, 0x0, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x7,
    0xff, 0xfe, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0x0,
    0x1, 0xff, 0xff, 0x80, 0x0, 0xf, 0xff, 0xfc,
    0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x3, 0xff,
    0xff, 0x0, 0x0, 0x1f, 0xff, 0xf8, 0x0, 0x0,
    0xff, 0xff, 0xc0, 0x0, 0x7, 0xff, 0xfe, 0x7f,
    0xff, 0x3f, 0xef, 0xe7, 0xff, 0xfc, 0xfe, 0x3e,
    0x7f, 0xff, 0xf7, 0xe1, 0xe7, 0xff, 0xff, 0xde,
    0x6, 0x7f, 0xff, 0xff, 0x60, 0x79, 0xff, 0xff,
    0xf7, 0x87, 0xe7, 0xff, 0xff, 0x7e, 0x3f, 0x9f,
    0xff, 0xf7, 0xfb, 0xfe, 0x7f, 0xff, 0x3f, 0xff,
    0xf0, 0x0, 0x1, 0xff, 0xff, 0x80, 0x0, 0xf,
    0xff, 0xfc, 0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0,
    0x3, 0xff, 0xff, 0x0, 0x0, 0x1f, 0xff, 0xf8,
    0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x7, 0xff,
    0xfe, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0x0, 0x1,
    0xff, 0xff, 0x80, 0x0, 0xf, 0xff, 0xfc, 0x0,
    0x0, 0x7f, 0xff, 0x9f, 0xff, 0xfd, 0xff, 0xf9,
    0xff, 0xff, 0xf7, 0xff, 0x9f, 0xff, 0xff, 0xdf,
    0x81, 0xff, 0xff, 0xff, 0x0, 0x1f, 0xff, 0xff,
    0xf8, 0x0, 0xff, 0xff, 0xff, 0xc0, 0x7, 0xff,
    0xff, 0xfe, 0x0, 0x1f, 0xff, 0xff, 0xe0, 0x0,
    0x3f, 0xff, 0xfe, 0x0,

    /* U+0039 "9" */
    0x1, 0xff, 0xff, 0xf8, 0x0, 0x1f, 0xff, 0xff,
    0xe0, 0x1, 0xff, 0xff, 0xff, 0x80, 0xf, 0xff,
    0xff, 0xfc, 0x0, 0x7f, 0xff, 0xff, 0xe0, 0x7d,
    0xff, 0xff, 0xfe, 0x7f, 0xf7, 0xff, 0xff, 0xe7,
    0xff, 0xdf, 0xff, 0xfe, 0x7f, 0xff, 0x7f, 0xff,
    0xe7, 0xff, 0xfc, 0x0, 0x0, 0x7f, 0xff, 0xe0,
    0x0, 0x3, 0xff, 0xff, 0x0, 0x0, 0x1f, 0xff,
    0xf8, 0x0, 0x0, 0xff, 0xff, 0xc0, 0x0, 0x7,
    0xff, 0xfe, 0x0, 0x0, 0x3f, 0xff, 0xf0, 0x0,
    0x1, 0xff, 0xff, 0x80, 0x0, 0xf, 0xff, 0xfc,
    0x0, 0x0, 0x7f, 0xff, 0xe0, 0x0, 0x3, 0xff,
    0xff, 0x0, 0x0, 0x1f, 0xff, 0xf8, 0x0, 0x0,
    0xff, 0xff, 0xc0, 0x0, 0x7, 0xff, 0xfe, 0x7f,
    0xff, 0x3f, 0xef, 0xe7, 0xff, 0xfc, 0xfe, 0x3e,
    0x7f, 0xff, 0xf7, 0xe1, 0xe7, 0xff, 0xff, 0xde,
    0x6, 0x7f, 0xff, 0xff, 0x60, 0x1, 0xff, 0xff,
    0xf7, 0x80, 0x7, 0xff, 0xff, 0x7e, 0x0, 0x1f,
    0xff, 0xf7, 0xf8, 0x0, 0x7f, 0xff, 0x3f, 0xe0,
    0x0, 0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0xf,
    0xf8, 0x0, 0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0,
    0x3, 0xfe, 0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0,
    0x0, 0x0, 0xff, 0x80, 0x0, 0x0, 0x7, 0xfc,
    0x0, 0x0, 0x0, 0x3f, 0xe0, 0x0, 0x0, 0x1,
    0xff, 0x0, 0x0, 0x0, 0xf, 0xf8, 0x0, 0x0,
    0x0, 0x7f, 0xc3, 0xff, 0xff, 0xfd, 0xfe, 0x3f,
    0xff, 0xff, 0xf7, 0xf7, 0xff, 0xff, 0xff, 0xdf,
    0xff, 0xff, 0xff, 0xff, 0x3, 0xff, 0xff, 0xff,
    0xf8, 0xf, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff,
    0xff, 0xfe, 0x0, 0xff, 0xff, 0xff, 0xe0, 0x3,
    0xff, 0xff, 0xfe, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 264, .box_w = 9, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 11, .adv_w = 703, .box_w = 37, .box_h = 51, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 703, .box_w = 18, .box_h = 51, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 362, .adv_w = 703, .box_w = 37, .box_h = 51, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 598, .adv_w = 703, .box_w = 36, .box_h = 51, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 828, .adv_w = 703, .box_w = 37, .box_h = 51, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 1064, .adv_w = 703, .box_w = 37, .box_h = 51, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 1300, .adv_w = 703, .box_w = 37, .box_h = 51, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 1536, .adv_w = 703, .box_w = 36, .box_h = 51, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 1766, .adv_w = 703, .box_w = 37, .box_h = 51, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 2002, .adv_w = 703, .box_w = 37, .box_h = 51, .ofs_x = 4, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint8_t glyph_id_ofs_list_0[] = {
    0, 0, 1, 2, 3, 4, 5, 6,
    7, 8, 9, 10
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 46, .range_length = 12, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = glyph_id_ofs_list_0, .list_length = 12, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_FULL
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
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
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t ni7seg_60 = {
#else
lv_font_t ni7seg_60 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 51,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -8,
    .underline_thickness = 5,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if NI7SEG_60*/

