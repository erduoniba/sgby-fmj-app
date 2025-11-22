package fmj.combat.anim

import fmj.lib.ResImage
import graphics.Canvas

/** 帧动画 */
class FrameAnimation constructor(private val mImage: ResImage, private val mStartFrame: Int = 1, private val mEndFrame: Int = mImage.number) {

    private var DELTA = 1000 / 5

    private var mCurFrame: Int = 0

    private var mTimeCnt: Long = 0

    init {
        mCurFrame = mStartFrame
    }

    fun setFPS(fps: Int) {
        DELTA = 1000 / fps
    }

    fun update(delta: Long) {
        mTimeCnt += delta
        if (mTimeCnt >= DELTA) {
            mTimeCnt = 0

            if (++mCurFrame > mEndFrame) {
                mCurFrame = mStartFrame
            }
        }
    }

    fun draw(canvas: Canvas, x: Int, y: Int) {
        mImage.draw(canvas, mCurFrame, x, y)
    }
}
