//
//  sys.c
//  baye-ios
//
//  Created by loong on 15/8/15.
//
//

#include <stdio.h>
#include "baye/compa.h"
#include "baye/comm.h"
#include "baye/fsys.h"
#include "timer.h"

static void(*_lcd_fluch_cb)(char*buffer);

#define SCR_W SCR_WID
#define SCR_H SCR_HGT
#define BYTES_PERLINE (SCR_LINE * 8 * AX_SCALE)

#define DOT 1
#define CLR 0
static char *static_buffer = 0;
static char *backup_buffer = 0;
static char isLcdDirty = 0;
static char *buffer;
static char *scr_buffer;
static size_t buffer_size;
U8 g_FlipDrawing = 0;
U8 g_paintColor = 0xff;
static U8 g_paintColorBak = 0xff;

U32 g_paintPalette[256] = {
    // 16-255: 灰度渐变色
    0x00FFFFFF,
    0x01FEFEFE,
    0x02FDFDFD,
    0x03FCFCFC,
    0x04FBFBFB,
    0x05FAFAFA,
    0x06F9F9F9,
    0x07F8F8F8,
    0x08F7F7F7,
    0x09F6F6F6,
    0x0AF5F5F5,
    0x0BF4F4F4,
    0x0CF3F3F3,
    0x0DF2F2F2,
    0x0EF1F1F1,
    0x0FF0F0F0,
    0x10EFEFEF,
    0x11EEEEEE,
    0x12EDEDED,
    0x13ECECEC,
    0x14EBEBEB,
    0x15EAEAEA,
    0x16E9E9E9,
    0x17E8E8E8,
    0x18E7E7E7,
    0x19E6E6E6,
    0x1AE5E5E5,
    0x1BE4E4E4,
    0x1CE3E3E3,
    0x1DE2E2E2,
    0x1EE1E1E1,
    0x1FE0E0E0,
    0x20DFDFDF,
    0x21DEDEDE,
    0x22DDDDDD,
    0x23DCDCDC,
    0x24DBDBDB,
    0x25DADADA,
    0x26D9D9D9,
    0x27D8D8D8,
    0x28D7D7D7,
    0x29D6D6D6,
    0x2AD5D5D5,
    0x2BD4D4D4,
    0x2CD3D3D3,
    0x2DD2D2D2,
    0x2ED1D1D1,
    0x2FD0D0D0,
    0x30CFCFCF,
    0x31CECECE,
    0x32CDCDCD,
    0x33CCCCCC,
    0x34CBCBCB,
    0x35CACACA,
    0x36C9C9C9,
    0x37C8C8C8,
    0x38C7C7C7,
    0x39C6C6C6,
    0x3AC5C5C5,
    0x3BC4C4C4,
    0x3CC3C3C3,
    0x3DC2C2C2,
    0x3EC1C1C1,
    0x3FC0C0C0,
    0x40BFBFBF,
    0x41BEBEBE,
    0x42BDBDBD,
    0x43BCBCBC,
    0x44BBBBBB,
    0x45BABABA,
    0x46B9B9B9,
    0x47B8B8B8,
    0x48B7B7B7,
    0x49B6B6B6,
    0x4AB5B5B5,
    0x4BB4B4B4,
    0x4CB3B3B3,
    0x4DB2B2B2,
    0x4EB1B1B1,
    0x4FB0B0B0,
    0x50AFAFAF,
    0x51AEAEAE,
    0x52ADADAD,
    0x53ACACAC,
    0x54ABABAB,
    0x55AAAAAA,
    0x56A9A9A9,
    0x57A8A8A8,
    0x58A7A7A7,
    0x59A6A6A6,
    0x5AA5A5A5,
    0x5BA4A4A4,
    0x5CA3A3A3,
    0x5DA2A2A2,
    0x5EA1A1A1,
    0x5FA0A0A0,
    0x609F9F9F,
    0x619E9E9E,
    0x629D9D9D,
    0x639C9C9C,
    0x649B9B9B,
    0x659A9A9A,
    0x66999999,
    0x67989898,
    0x68979797,
    0x69969696,
    0x6A959595,
    0x6B949494,
    0x6C939393,
    0x6D929292,
    0x6E919191,
    0x6F909090,
    0x708F8F8F,
    0x718E8E8E,
    0x728D8D8D,
    0x738C8C8C,
    0x748B8B8B,
    0x758A8A8A,
    0x76898989,
    0x77888888,
    0x78878787,
    0x79868686,
    0x7A858585,
    0x7B848484,
    0x7C838383,
    0x7D828282,
    0x7E818181,
    0x7F808080,
    0x807F7F7F,
    0x817E7E7E,
    0x827D7D7D,
    0x837C7C7C,
    0x847B7B7B,
    0x857A7A7A,
    0x86797979,
    0x87787878,
    0x88777777,
    0x89767676,
    0x8A757575,
    0x8B747474,
    0x8C737373,
    0x8D727272,
    0x8E717171,
    0x8F707070,
    0x906F6F6F,
    0x916E6E6E,
    0x926D6D6D,
    0x936C6C6C,
    0x946B6B6B,
    0x956A6A6A,
    0x96696969,
    0x97686868,
    0x98676767,
    0x99666666,
    0x9A656565,
    0x9B646464,
    0x9C636363,
    0x9D626262,
    0x9E616161,
    0x9F606060,
    0xA05F5F5F,
    0xA15E5E5E,
    0xA25D5D5D,
    0xA35C5C5C,
    0xA45B5B5B,
    0xA55A5A5A,
    0xA6595959,
    0xA7585858,
    0xA8575757,
    0xA9565656,
    0xAA555555,
    0xAB545454,
    0xAC535353,
    0xAD525252,
    0xAE515151,
    0xAF505050,
    0xB04F4F4F,
    0xB14E4E4E,
    0xB24D4D4D,
    0xB34C4C4C,
    0xB44B4B4B,
    0xB54A4A4A,
    0xB6494949,
    0xB7484848,
    0xB8474747,
    0xB9464646,
    0xBA454545,
    0xBB444444,
    0xBC434343,
    0xBD424242,
    0xBE414141,
    0xBF404040,
    0xC03F3F3F,
    0xC13E3E3E,
    0xC23D3D3D,
    0xC33C3C3C,
    0xC43B3B3B,
    0xC53A3A3A,
    0xC6393939,
    0xC7383838,
    0xC8373737,
    0xC9363636,
    0xCA353535,
    0xCB343434,
    0xCC333333,
    0xCD323232,
    0xCE313131,
    0xCF303030,
    0xD02F2F2F,
    0xD12E2E2E,
    0xD22D2D2D,
    0xD32C2C2C,
    0xD42B2B2B,
    0xD52A2A2A,
    0xD6292929,
    0xD7282828,
    0xD8272727,
    0xD9262626,
    0xDA252525,
    0xDB242424,
    0xDC232323,
    0xDD222222,
    0xDE212121,
    0xDF202020,
    0xE01F1F1F,
    0xE11E1E1E,
    0xE21D1D1D,
    0xE31C1C1C,
    0xE41B1B1B,
    0xE51A1A1A,
    0xE6191919,
    0xE7181818,
    0xE8171717,
    0xE9161616,
    0xEA151515,
    0xEB141414,
    0xEC131313,
    0xED121212,
    0xEE111111,
    0xEF101010,
    0xF00F0F0F,
    0xF10E0E0E,
    0xF20D0D0D,
    0xF30C0C0C,
    0xF40B0B0B,
    0xF50A0A0A,
    0xF6090909,
    0xF7080808,
    0xF8070707,
    0xF9060606,
    0xFA050505,
    0xFB040404,
    0xFC030303,
    0xFD020202,
    0xFE010101,
    0xFF000000,
};

void pushPaintColor(U8 color) {
    g_paintColorBak = g_paintColor;
    g_paintColor = color;
}

void popPaintColor() {
    g_paintColor = g_paintColorBak;
}

static void screen_buffer_realloc(int size);

void screen_buffer_init(void) {
    screen_buffer_realloc(MAX_SCR_BUF_LEN);
}

static void screen_buffer_realloc(int size) {
    gam_free(static_buffer);
    gam_free(backup_buffer);
    printf("realloc screen buffer to %d\n", size);
    static_buffer = gam_malloc(size);
    backup_buffer = gam_malloc(size);
    buffer = static_buffer;
    scr_buffer = static_buffer;
    buffer_size = size;
}

static U8 _insideScreen(PT x, PT y) {
    return x >= 0 && y >= 0 && x < g_screenWidth && y < g_screenHeight;
}

void GamSetLcdFlushCallback(void(*lcd_fluch_cb)(char*buffer))
{
    _lcd_fluch_cb = lcd_fluch_cb;
}

static void convert_image(U32* dst, char* src) {
    int i, j;
    for (j = 0; j < SCR_H * AX_SCALE; j++) {
        for (i = 0; i < SCR_W * AX_SCALE; i++) {
            int ind = j * BYTES_PERLINE + i;
            dst[ind] = g_paintPalette[(U8)src[ind]];
        }
    }
}

static void timed_flush_lcd()
{
    static char* outbuf;
    static int buflen = 0;

    int curlen = SCR_H * AX_SCALE * SCR_W * AX_SCALE * 4;

    if (outbuf == NULL || buflen < curlen) {
        if (outbuf) gam_free(outbuf);
        printf("realloc 32bit screen buffer size to %d (%dx%d@%d)\n", curlen, SCR_W, SCR_H, AX_SCALE);
        outbuf = (char*)gam_malloc(curlen);
        if (outbuf == NULL) {
            printf("malloc failed\n");
            return;
        }
        buflen = curlen;
    }

    if (isLcdDirty && _lcd_fluch_cb) {
        convert_image((U32*)outbuf, scr_buffer);
        _lcd_fluch_cb(outbuf);
        isLcdDirty = 0;
    }
}

FAR void logLcd()
{
    int x, y;
    int perLine = BYTES_PERLINE;
    for (y = 0; y < SCR_H; y++) {
        printf("%03d ", y);
        for (x = 0; x < SCR_W; x++) {
            bool pixel = buffer[perLine*y + x];
            printf("%s ", pixel ? "." : "#");
        }
        printf("\n");
    }
    printf("\n");
    printf("---------------%dx%d-------------------\n", SCR_W, SCR_H);
    printf("\n");
}

FAR void flushLcd()
{
    if (_lcd_fluch_cb && buffer == scr_buffer) {
        isLcdDirty = 1;
    }
}

FAR void logPicture(U8 wid, U8 hgt, U8* pic)
{
    int x, y;
    {
        int perLine = (wid + 7) / 8;
        for (y = 0; y < hgt; y++) {
            for (x = 0; x < wid; x++) {
                if (pic) {
                    bool pixel = pic[perLine*y + x/8] & (128 >> (x%8));
                    printf("%s", pixel ? "  " : "##");
                } else {
                    printf("@");
                }
            }
            printf("\n");
        }
        printf("\n");
        printf("----------------------------------\n");
        printf("\n");
    }
}


FAR void SysAscii(PT x,PT y,U8 asc)
{
}

FAR U8 SysGetKey(void)
{
    MsgType msg;
    GuiGetMsg(&msg);
    return msg.param;
}

FAR U8   SysGetKeySound(void)
{
    return 0;
}

FAR	U8 SysGetTimer1Number(void)
{
    return (U8)gam_timer_interval(1);
}

FAR	void SysIconAllClear(void)						/*清除所有icon显示,系统的除外 */
{
}

FAR void SysIconBattery(U8 data)
{
}

FAR	void SysLCDVoltage(U8 voltage)		/*voltage: 0 - 63 */
{
}

static void _dot(PT x, PT y, U8 color) {
    x *= AX_SCALE;
    y *= AX_SCALE;
    for (int i = 0; i < AX_SCALE; i++) {
        for (int j = 0; j < AX_SCALE; j++) {
            int ind = BYTES_PERLINE * (y+i) + (x+j);
            buffer[ind] = color ? g_paintColor : 0;
        }
    }
}

static void _rdot(PT x, PT y) {
    x *= AX_SCALE;
    y *= AX_SCALE;
    for (int i = 0; i < AX_SCALE; i++) {
        for (int j = 0; j < AX_SCALE; j++) {
            int ind = BYTES_PERLINE * (y+i) + (x+j);
            buffer[ind] = 0xff - buffer[ind];
        }
    }
}

FAR void SysLcdPartClear(PT x1,PT y1,PT x2,PT y2)
{
    PT x, y;
    for (y = y1; y <= y2; y++) {
        for (x = x1; x <= x2; x++) {
            if (!_insideScreen(x, y)) continue;
            _dot(x, y, CLR);
        }
    }
    flushLcd();
}

FAR void SysLcdReverse(PT x1,PT y1,PT x2,PT y2)
{
    PT x, y;
    for (y = y1; y <= y2; y++) {
        for (x = x1; x <= x2; x++) {
            if (!_insideScreen(x, y)) continue;

            _rdot(x, y);
        }
    }
    flushLcd();
}

FAR void SysLine(PT x1,PT y1,PT x2,PT y2)
{
}

static void _timer0cb()
{
    MsgType msg;
    msg.type = VM_TIMER;
    msg.param = 0;
    GuiPushMsg(&msg);
}

static void _timer1cb()
{
    MsgType msg;
    msg.type = VM_TIMER;
    msg.param = 1;
    GuiPushMsg(&msg);
}

FAR	void	SysMemInit(U16 start,U16 len)
{
    gam_timer_init();
    gam_timer_set_callback(0, _timer0cb);
    gam_timer_set_callback(1, _timer1cb);
    gam_timer2_open(2, timed_flush_lcd);
}

FAR void DecodePic(U8* dst, const U8*pic, PT w, PT h, U8 scale) {
    int ppl = (w + 7) / 8;
    PT x, y, W, H, X, Y;

    W = w * scale;
    H = h * scale;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            X = x * scale;
            Y = y * scale;
            
            U8 p = pic[ppl*y + x/8] & (128 >> (x%8));

            for (int j = 0; j < scale; j++) {
                for (int i = 0; i < scale; i++) {
                    dst[W*(Y+j) + X+i] = p;
                }
            }
        }
    }
}

FAR void SysPictureEx(PT sX, PT sY, PT eX, PT eY, U8*pic , U8 flag, U8 scale, U8 compat) {
    int wid = eX - sX + 1;
    int hgt = eY - sY + 1;
    int x, y, X, Y;
    int scrPerLine = BYTES_PERLINE;
    static U8 *_buf = NULL;
    int pixs = 0;

    if (_buf == NULL) {
        _buf = gam_malloc(MAX_SCR_BUF_LEN);
    } 

    if (compat && pic) {
        DecodePic(_buf, pic, wid, hgt, scale);
        pic = _buf;
    }

    wid *= scale;
    hgt *= scale;

    {
        int picPerLine = wid;

        for (y = 0; y < hgt; y++) {
            Y = sY*AX_SCALE + y;
            for (x = 0; x < wid; x++) {
                X = sX*AX_SCALE + x;
                unsigned char pixel0, pixel1;
                int ind = scrPerLine * Y + X;
                if (!_insideScreen(X/AX_SCALE, Y/AX_SCALE)) {
                    continue;
                }

                if (flag == 4) {
                    pixel1 = 0;
                }
                else {
                    
                    if (pic) {
                        PT rx, ry;
                        rx = (g_FlipDrawing & 0x01) ? wid-x-1 : x;
                        ry = (g_FlipDrawing & 0x02) ? hgt-y-1 : y;
                        pixel0 = pic[picPerLine*ry + rx];
                    }
                    else {
                        pixel0 = 0;
                    }
                    pixel0 = pixel0 ? g_paintColor : 0;
                    
                    {
                        pixel1 = buffer[ind];
                        
                        switch (flag) {
                            case 0: // Normal
                                pixel1 = pixel0;
                                break;
                            case 1: // &
                                pixel1 = pixel0 ? pixel1 : 0;
                                break;
                            case 2: // |
                                pixel1 = pixel0 ? pixel0 : pixel1;
                                break;
                            case 4: // clear
                                break;
                            default:
                                break;
                        }
                    }
                }
                buffer[ind] = pixel1;
                pixs ++;
            }
        }
    }
    flushLcd();
}

FAR void SysPicture(PT sX, PT sY, PT eX, PT eY, U8*pic , U8 flag, U8 scale) {
    return SysPictureEx(sX, sY, eX, eY, pic , flag, scale, 1);
}

static inline void _pixel(PT x,PT y,U8 data)
{
    if (!_insideScreen(x, y)) return;

    _dot(x, y, data);
}

FAR void SysPutPixel(PT x,PT y,U8 data)
{
    _pixel(x, y, data);
    flushLcd();
}

FAR void SysRect(PT x1,PT y1,PT x2,PT y2)
{
    int x, y;
    y = y1;
    for (x = x1; x <= x2; x++) {
        _pixel(x, y, DOT);
    }
    y = y2;
    for (x = x1; x <= x2; x++) {
        _pixel(x, y, DOT);
    }
    x = x1;
    for (y = y1; y <= y2; y++) {
        _pixel(x, y, DOT);
    }
    x = x2;
    for (y = y1; y <= y2; y++) {
        _pixel(x, y, DOT);
    }
    flushLcd();
}

FAR void SysRectClear(PT x1,PT y1,PT x2,PT y2)
{
    SysLcdPartClear(x1, y1, x2, y2);
    flushLcd();
}

FAR void SysSetKeySound(U8 keySoundFlag)
{
}

FAR void SysTimer1Close(void)
{
    gam_timer_close(1);
}

FAR void SysTimer1Open(U8 times)
{
    gam_timer_open(1, times);
}

FAR void SysScrollingTimerClose(void)
{
    gam_timer_close(0);
}

FAR int SysScrollingTimerOpen(int times)
{
    return gam_timer_open(0, times);
}

FAR U8 SysScrollingTimerOn(void) {
    return gam_check_timer_on(0);
}

FAR void SysSaveScreen()
{
    memcpy(backup_buffer, scr_buffer, buffer_size);
}

FAR void SysRestoreScreen()
{
    memcpy(scr_buffer, backup_buffer, buffer_size);
}

FAR void SysAdjustLCDBuffer(int wid, int height)
{
    size_t sz = wid * height * AX_SCALE * AX_SCALE;

    if (sz <= buffer_size) {
        memset(scr_buffer, 0, sz);
        return;
    }

    screen_buffer_realloc(sz);
    memset(scr_buffer, 0, sz);
}

FAR void SysSelectScreen(U8*scr)
{
    buffer = scr ? (char*)scr : scr_buffer;
}

FAR void SysCopyScreen(U8*scr)
{
    memcpy(scr_buffer, scr, buffer_size);
    isLcdDirty = 1;
}

FAR U8 *gam_freadall(gam_FILE *fhandle, U32 *datalen)
{
    U32 alloced = 1024*400;
    U8 buf[1024];
    U8 *rv = gam_malloc(alloced);
    U32 offset = 0;
    U32 cnt = 0;
    
    do {
        cnt = gam_fread(buf, 1, 1024, fhandle);
        if (offset + cnt > alloced) {
            alloced *= 2;
            rv = gam_realloc(rv, alloced);
        }
        memcpy(rv + offset, buf, cnt);
        offset += cnt;
    } while (cnt > 0);

    if (offset == alloced) {
        rv = gam_realloc(rv, offset+1);
    }
    rv[offset] = 0;

    if (datalen) {
        *datalen = offset;
    }

    printf("fread all:%d\n", offset);
    return rv;
}
