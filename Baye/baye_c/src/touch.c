//
//  touch.c
//  baye-ios
//
//  Created by loong on 16/10/3.
//
//

#include "touch.h"
#include "baye/compa.h"

void touchSendTouchEvent(U16 event, I16 x, I16 y)
{
    MsgType msg;
    msg.type = VM_TOUCH;
    msg.param = event;
    msg.param2.i16.p0 = x;
    msg.param2.i16.p1 = y;
    GuiPushMsg(&msg);
}

///判断一个点是否在矩形框内
U8 touchIsPointInRect(I16 x, I16 y, Rect r)
{
    return (r.left <= x && x <= r.right) && (r.top <= y && y <= r.bottom);
}

///获取触摸点在list内的item序号。
I16 touchListViewItemIndexAtPoint(I16 x, I16 y, Rect listRect, I16 topPadding, I16 bottomPadding, U16 itemStart, U16 itemCount, U16 itemHeight)
{
    if (!touchIsPointInRect(x, y, listRect)) {
        return -2;
    }
    U16 yOffset = y - listRect.top - topPadding;
    U16 index = itemStart + yOffset / itemHeight;
    return index < itemCount ? index : -1;
}

static I16 fade(I16 v) {
    if (v == 0) {
        return 0;
    } else {
        I16 dv = v / 10;
        if (dv == 0) {
            dv = v > 0 ? 1 : -1;
        }
        return v - dv;
    }
}

U8 pointState(I16 v, I16 min, I16 max) {
    U8 ret = 0;
    if (v <= min) {
        ret |= 1;
    }
    if (v >= max) {
        ret |= 2;
    }
    return ret;
}

void touchUpdateViewState(Touch *touch, U8 sx, U8 sy)
{
    if (!touch->gliding) {
        return;
    }
    if (touch->speedX < 0 && (sx & 2) || touch->speedX > 0 && (sx & 1) ) {
        touch->speedX = 0;
    }
    if (touch->speedY < 0 && (sy & 2) || touch->speedY > 0 && (sy & 1) ) {
        touch->speedY = 0;
    }
    if (touch->speedX == 0 && touch->speedY == 0) {
        touch->gliding = 0;
    }
}

I8 touchUpdate(Touch *touch, MsgType msg)
{
    switch(msg.type) {
        case VM_TIMER:
        {
            if (msg.param != 0) {
                return 0;
            }
            if (touch->gliding) {
                touch->currentX += touch->speedX;
                touch->currentY += touch->speedY;
                touch->speedX = fade(touch->speedX);
                touch->speedY = fade(touch->speedY);
                if (touch->speedX == 0 && touch->speedY == 0) {
                    touch->gliding = 0;
                }
                return 1;
            }
            if (touch->touched) {
                touch->speedX = touch->currentX - touch->prevX;
                touch->speedY = touch->currentY - touch->prevY;
                touch->prevX = touch->currentX;
                touch->prevY = touch->currentY;
            }
            return 0;
        }
        case VM_TOUCH:
        {
            U16 currentX = msg.param2.i16.p0;
            U16 currentY = msg.param2.i16.p1;

            switch (msg.param) {
                case VT_TOUCH_DOWN:
                    touch->prevX = touch->startX = touch->currentX = currentX;
                    touch->prevY = touch->startY = touch->currentY = currentY;
                    touch->touched = 1;
                    touch->moved = touch->gliding;
                    touch->gliding = 0;
                    touch->speedX = 0;
                    touch->speedY = 0;
                    break;
                case VT_TOUCH_UP:
                    if (touch->touched && touch->moved) {
                        touch->gliding = SysScrollingTimerOn();
                        touch->currentX = currentX;
                        touch->currentY = currentY;
                        U16 speedX = touch->currentX - touch->prevX;
                        U16 speedY = touch->currentY - touch->prevY;
                        #define S_ABS(x) (x >= 0?x:-x)
                        #define UP_IF_GT(speed) if (S_ABS(speed) > S_ABS(touch->speed)) { \
                            touch->speed = speed; \
                        }
                        UP_IF_GT(speedX);
                        UP_IF_GT(speedY);
                    }
                    touch->completed = touch->touched;
                    touch->touched = 0;
                    break;
                case VT_TOUCH_MOVE:
                    if (!touch->touched) break;
                    touch->currentX = currentX;
                    touch->currentY = currentY;
                    if (abs(touch->currentX - touch->startX) > 2 || abs(touch->currentY - touch->startY) > 2) {
                        touch->moved = 1;
                    }
                    break;
                case VT_TOUCH_CANCEL:
                    touch->touched = 0;
                    touch->completed = 0;
                    touch->gliding = 0;
                    break;
                default:
                    break;
            }
            return 0;
        }
        case VM_KEY:
        case VM_CHAR_ASC:
        case VM_CHAR_FUN:
        case VM_CHAR_HZ:
        case VM_CHAR_MATH:
        {
            touch->gliding = 0;
            return 0;
        }
        default:
            return 0;
    }
}

I32 limitValueInRange(I32 value, I32 min, I32 max)
{
    if (value > max) value = max;
    if (value < min) value = min;
    return value;
}

Point touchListViewCalcTopLeftForMove(Touch *touch_,
                                      U16 leftWhenTouchDown,
                                      U16 xMax,
                                      U16 itemWitdh,
                                      U16 topWhenTouchDown,
                                      U16 yMax,
                                      U16 itemHeight
                                      )
{
#define touch (*touch_)
    Point result = {leftWhenTouchDown, topWhenTouchDown};

    if (itemWitdh) {
        I16 dx = touch.currentX - touch.startX;
        I16 cols = dx / itemWitdh;
        I16 x = leftWhenTouchDown - cols;
        result.x = limitValueInRange(x, 0, xMax);
    }
    if (itemHeight) {
        I16 dy = touch.currentY - touch.startY;
        I16 rows = dy / itemHeight;
        I16 y = topWhenTouchDown - rows;
        result.y = limitValueInRange(y, 0, yMax);
    }
    return result;
}

Rect MakeRect(I16 x, I16 y, I16 w, I16 h)
{
    return (Rect){
        x, y, x + w - 1, y + h - 1
    };
}

void touchDrawButton(Rect rect, const char*title)
{
#define gam_rect2(r) gam_rect(r.left, r.top, r.right, r.bottom)
#define gam_clr_rect2(r) gam_clrlcd(r.left, r.top, r.right, r.bottom)
    gam_clr_rect2(rect);
    gam_rect2(rect);
    GamStrShowS(rect.left+2, rect.top+1, (const U8*)title);
}