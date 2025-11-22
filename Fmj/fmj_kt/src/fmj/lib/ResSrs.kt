package fmj.lib

import graphics.Canvas

class ResSrs : ResBase() {
    /**
     * 帧数
     */
    private var mFrameNum: Int = 0

    /**
     * 图片数
     */
    private var mImageNum: Int = 0

    private var mStartFrame: Int = 0
    private var mEndFrame: Int = 0

    /**
     * `mFrameHeader = new int[mFrameNum][5];`
     *
     *
     * x,y,Show,nShow,imgIndex
     */
    private var mFrameHeader: Array<IntArray>? = null

    private var mImage: Array<ResImage>? = null

    private var ITERATOR = 1 // update 迭代次数
    private val mShowList = mutableListOf<Key>()

    override fun setData(buf: ByteArray, offset: Int) {
        type = buf[offset].toInt()
        index = buf[offset + 1].toInt() and 0xFF
        mFrameNum = buf[offset + 2].toInt() and 0xFF
        mImageNum = buf[offset + 3].toInt() and 0xFF
        mStartFrame = buf[offset + 4].toInt() and 0xFF
        mEndFrame = buf[offset + 5].toInt() and 0xFF

        var ptr = offset + 6
        mFrameHeader = Array(mFrameNum) { IntArray(5) }
        for (i in 0 until mFrameNum) {
            mFrameHeader!![i][0] = buf[ptr++].toInt() and 0xFF // x
            mFrameHeader!![i][1] = buf[ptr++].toInt() and 0xFF // y
            mFrameHeader!![i][2] = buf[ptr++].toInt() and 0xFF // Show
            mFrameHeader!![i][3] = buf[ptr++].toInt() and 0xFF // nShow
            mFrameHeader!![i][4] = buf[ptr++].toInt() and 0xFF // 图号
        }

        // 读入mImageNum个ResImage
        mImage = Array(mImageNum) {
            val img = ResImage()
            img.setData(buf, ptr)
            ptr += img.bytesCount
            img
        }
    }

    private inner class Key(internal var index: Int) {
        internal var show: Int = 0
        internal var nshow: Int = 0

        init {
            val frameHeaders = mFrameHeader
            if (frameHeaders != null && index >= 0 && index < frameHeaders.size) {
                this.show = frameHeaders[index][2]
                this.nshow = frameHeaders[index][3]
            }
        }
    }

    /**
     * 开始特效动画
     */
    fun start() {
        if (mFrameNum == 0 || mFrameHeader == null || mImage == null) return
        mShowList.clear()
        mShowList.add(Key(0))
    }

    /**
     *
     * @return 返回false动画播放完毕
     */
    fun update(delta: Long): Boolean {
        if (mFrameNum == 0 || mFrameHeader == null || mImage == null) return false
        for (j in 0 until ITERATOR) {
            var iter: MutableListIterator<Key> = mShowList.listIterator()
            while (iter.hasNext()) {
                val i = iter.next()
                --i.show
                --i.nshow
                if (i.nshow == 0 && i.index + 1 < mFrameNum) {
                    iter.add(Key(i.index + 1)) // 下一帧开始显示
                }
            }
            iter = mShowList.listIterator()
            while (iter.hasNext()) {
                val i = iter.next()
                if (i.show <= 0) { // 该帧的图片显示完成
                    iter.remove()
                }
            }
            if (mShowList.isEmpty()) return false
        }
        return true
    }

    fun draw(canvas: Canvas, dx: Int, dy: Int) {
        val images = mImage ?: return
        val frameHeaders = mFrameHeader ?: return
        
        for (i in mShowList) {
            val frameIndex = i.index
            if (frameIndex >= 0 && frameIndex < frameHeaders.size) {
                val imageIndex = frameHeaders[frameIndex][4]
                if (imageIndex >= 0 && imageIndex < images.size) {
                    images[imageIndex].draw(canvas, 1, 
                        frameHeaders[frameIndex][0] + dx, 
                        frameHeaders[frameIndex][1] + dy)
                }
            }
        }
    }

    fun drawAbsolutely(canvas: Canvas, x: Int, y: Int) {
        val images = mImage ?: return
        val frameHeaders = mFrameHeader ?: return
        
        if (frameHeaders.isEmpty()) return
        
        for (i in mShowList) {
            val frameIndex = i.index
            if (frameIndex >= 0 && frameIndex < frameHeaders.size) {
                val imageIndex = frameHeaders[frameIndex][4]
                if (imageIndex >= 0 && imageIndex < images.size) {
                    images[imageIndex].draw(canvas, 1,
                        frameHeaders[frameIndex][0] - frameHeaders[0][0] + x,
                        frameHeaders[frameIndex][1] - frameHeaders[0][1] + y)
                }
            }
        }
    }

    fun setIteratorNum(n: Int) {
        ITERATOR = n
        if (ITERATOR < 1) {
            ITERATOR = 1
        }
    }

}
