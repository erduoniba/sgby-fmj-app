;==============================================================
;  A系列游戏图形显示相关函数
;  2005/5/17 02:33PM
;
;  Copyright (c)2001 , 广东步步高教育电子分公司
;  All rights reserved.
;==============================================================
.include picture.h

;=======================================================================================
;将函数声明成长调用函数
	bank_table:	.section
	.global		&GamePictureDummy	;(U8 x1,U8 y1,U8 x2,U8 y2,U8* pic,U8* Screen,U8 flag);
	
	.bank_table
	&GamePictureDummy:	.bankaddr	_GamePictureDummy
		
;=======================================================================================
;=======================================================================================
;定义本页生成的程序段名
GraLib_Program:	.section
		.GraLib_Program	
;=======================================================================================
_GamePictureDummy:
	;函数参数的弹栈和参数变量的初始化
	sta	gam_PicStartX
	ldy	#0
	lda	(__stack_ptr),y
	sta	gam_PicStartY
	iny
	lda	(__stack_ptr),y
	sta	gam_PicEndX
	iny
	lda	(__stack_ptr),y
	sta	gam_PicEndY
	iny	
	lda	(__stack_ptr),y
	sta	gam_PicPtr
	iny	
	lda	(__stack_ptr),y
	sta	gam_PicPtr+1
	iny	
	lda	(__stack_ptr),y
	sta	gam_VSPtr
	iny	
	lda	(__stack_ptr),y
	sta	gam_VSPtr+1
	iny	
	lda	(__stack_ptr),y
	sta	gam_ShowFlag

	lda	gam_PicStartX
	and	#%00000111
	sta	gam_BitOff
	
	inc	gam_PicEndX
	lda	gam_PicEndX
	sec
	sbc	gam_PicStartX
	clc
	adc	#7
	sta	gam_PicWidth
	lsr	gam_PicWidth
	lsr	gam_PicWidth
	lsr	gam_PicWidth
	
	lda	gam_PicEndX
	cmp	#SCR_WIDTH
	bcc	?lowwidth
	lda	#SCR_WIDTH
	sta	gam_PicEndX
?lowwidth:
	and	#%00000111
	sta	gam_EndLineOff
	
	lda	gam_PicEndX
	sec
	sbc	gam_PicStartX
	clc
	adc	#7
	sta	gam_VSWidth
	lsr	gam_VSWidth
	lsr	gam_VSWidth
	lsr	gam_VSWidth
	
	lda	gam_PicEndX
	sta	gam_Counter2
	dec	gam_Counter2
	lsr	gam_Counter2
	lsr	gam_Counter2
	lsr	gam_Counter2
	lda	gam_PicStartX
	sta	gam_Counter1
	lsr	gam_Counter1
	lsr	gam_Counter1
	lsr	gam_Counter1
	lda	gam_Counter2
	sec
	sbc	gam_Counter1
	sta	gam_LineEndByte

	inc	gam_PicEndY
	lda	gam_PicEndY
	sec
	sbc	gam_PicStartY
	sta	gam_PicHeight
	
	lda	gam_PicEndY
	cmp	#SCR_HEIGHT
	bcc	?lowheight
	lda	#SCR_HEIGHT
	sta	gam_PicEndY
?lowheight:
	sec
	sbc	gam_PicStartY
	sta	gam_VSHeight

	;计算屏幕缓冲的偏移
	lda	#SCR_WBYTE
	sta	gam_Counter2
	lda	#0
	sta	gam_Counter2+1
	sta	gam_Counter1
	sta	gam_Counter1+1
	lda	gam_PicStartY
	sta	gam_MulTmp
	ldy	#1
	lsr	gam_MulTmp
	bcc	?opstart
	lda	#SCR_WBYTE
	sta	gam_Counter1
?opstart:
	cpy	#8
	beq	?opend
	iny
	asl	gam_Counter2
	rol	gam_Counter2+1
	lsr	gam_MulTmp
	bcc	?opstart
	lda	gam_Counter1
	clc
	adc	gam_Counter2
	sta	gam_Counter1
	lda	gam_Counter1+1
	adc	gam_Counter2+1
	sta	gam_Counter1+1
	jmp	?opstart
?opend:
	lda	gam_VSPtr
	clc
	adc	gam_Counter1
	sta	gam_VSPtr
	lda	gam_VSPtr+1
	adc	gam_Counter1+1
	sta	gam_VSPtr+1
	lsr	gam_PicStartX
	lsr	gam_PicStartX
	lsr	gam_PicStartX
	lda	gam_VSPtr
	clc
	adc	gam_PicStartX
	sta	gam_VSPtr
	lda	gam_VSPtr+1
	adc	#0
	sta	gam_VSPtr+1
	
	lda	gam_ShowFlag
	cmp	#0			;直接贴图
	bne	?cmpandpirture
	jmp	?directpicture
?cmpandpirture:
	cmp	#1			;与运算贴图
	bne	?cmporpirture
	jmp	?andpicture
?cmporpirture:
	cmp	#2			;或运算贴图
	bne	?cmporxpirture
	jmp	?orpicture
?cmporxpirture:
	cmp	#3			;异或运算贴图
	bne	?cmpclspirture
	jmp	?orxpicture
?cmpclspirture:
	cmp	#4			;清除贴图
	bne	?errorflag
	jmp	?clspirture
?errorflag:
	rts
	
?directpicture:				;直接贴图
	lda	gam_LineEndByte
	beq	?directonebyte
?directpictureloop:
	jsr	?getlinebyte
	jsr	?shiftline
	jsr	?clearneekdeal
	lda	gam_BitOff
	bne	?linebitoff
	ldy	#0
	jmp	?directlineloop
?linebitoff:
	ldx	gam_BitOff
	lda	graphyrmask,x
	sta	gam_PicTmp
	ldy	#0
	lda	(gam_VSPtr),y
	and	gam_PicTmp
	sta	gam_PicTmp
	lda	gam_PicLine,y
	ora	gam_PicTmp
	sta	(gam_VSPtr),y
	ldy	#1
	
?directlineloop:
	cpy	gam_LineEndByte
	beq	?directlineend
	lda	gam_PicLine,y
	sta	(gam_VSPtr),y
	iny
	jmp	?directlineloop
?directlineend:
	lda	gam_EndLineOff
	beq	?noendlineoff
	ldx	gam_EndLineOff
	lda	graphymask,x
	sta	gam_PicTmp
	lda	(gam_VSPtr),y
	and	gam_PicTmp
	sta	gam_PicTmp
	lda	gam_PicLine,y
	ora	gam_PicTmp
	sta	(gam_VSPtr),y
	jmp	?picturenextline
?noendlineoff:
	lda	gam_PicLine,y
	sta	(gam_VSPtr),y
?picturenextline:
	
	lda	gam_PicPtr
	clc
	adc	gam_PicWidth
	sta	gam_PicPtr
	lda	#0
	adc	gam_PicPtr+1
	sta	gam_PicPtr+1
	
	lda	gam_VSPtr
	clc
	adc	#SCR_WBYTE
	sta	gam_VSPtr
	lda	#0
	adc	gam_VSPtr+1
	sta	gam_VSPtr+1
	
	dec	gam_VSHeight
	bne	?directpictureloop
	rts
?directonebyte:
	ldx	gam_BitOff
	lda	orgraphyrmask,x
	sta	gam_MaskTmp
	ldx	gam_EndLineOff
	lda	orgraphymask,x
	ora	gam_MaskTmp
	sta	gam_MaskTmp
?directonebyteloop:
	jsr	?getlinebyte
	jsr	?shiftline
	jsr	?clearneekdeal
	ldy	#0
	lda	(gam_VSPtr),y
	and	gam_MaskTmp
	sta	gam_PicTmp
	lda	gam_PicLine
	ora	gam_PicTmp
	sta	(gam_VSPtr),y
	
	lda	gam_PicPtr
	clc
	adc	gam_PicWidth
	sta	gam_PicPtr
	lda	#0
	adc	gam_PicPtr+1
	sta	gam_PicPtr+1
	
	lda	gam_VSPtr
	clc
	adc	#SCR_WBYTE
	sta	gam_VSPtr
	lda	#0
	adc	gam_VSPtr+1
	sta	gam_VSPtr+1
	
	dec	gam_VSHeight
	bne	?directonebyteloop
	rts
	
?andpicture:				;与运算贴图
?andpictureloop:
	jsr	?getlinebyte
	jsr	?shiftline
	jsr	?setneekdeal
	ldy	#0
?andlineloop:
	lda	gam_PicLine,y
	sta	gam_PicTmp
	lda	(gam_VSPtr),y
	and	gam_PicTmp
	sta	(gam_VSPtr),y
	iny
	cpy	gam_LineEndByte
	bcc	?andlineloop
	beq	?andlineloop
	
	lda	gam_PicPtr
	clc
	adc	gam_PicWidth
	sta	gam_PicPtr
	lda	#0
	adc	gam_PicPtr+1
	sta	gam_PicPtr+1
	
	lda	gam_VSPtr
	clc
	adc	#SCR_WBYTE
	sta	gam_VSPtr
	lda	#0
	adc	gam_VSPtr+1
	sta	gam_VSPtr+1
	
	dec	gam_VSHeight
	bne	?andpictureloop
	rts
	
?orpicture:				;或运算贴图
?orpictureloop:
	jsr	?getlinebyte
	jsr	?shiftline
	jsr	?clearneekdeal
	ldy	#0
?orlineloop:
	lda	gam_PicLine,y
	sta	gam_PicTmp
	lda	(gam_VSPtr),y
	ora	gam_PicTmp
	sta	(gam_VSPtr),y
	iny
	cpy	gam_LineEndByte
	bcc	?orlineloop
	beq	?orlineloop
	
	lda	gam_PicPtr
	clc
	adc	gam_PicWidth
	sta	gam_PicPtr
	lda	#0
	adc	gam_PicPtr+1
	sta	gam_PicPtr+1
	
	lda	gam_VSPtr
	clc
	adc	#SCR_WBYTE
	sta	gam_VSPtr
	lda	#0
	adc	gam_VSPtr+1
	sta	gam_VSPtr+1
	
	dec	gam_VSHeight
	bne	?orpictureloop
	rts
	
?orxpicture:				;异或运算贴图
?eorpictureloop:
	jsr	?getlinebyte
	jsr	?shiftline
	jsr	?clearneekdeal
	ldy	#0
?eorlineloop:
	lda	gam_PicLine,y
	sta	gam_PicTmp
	lda	(gam_VSPtr),y
	eor	gam_PicTmp
	sta	(gam_VSPtr),y
	iny
	cpy	gam_LineEndByte
	bcc	?eorlineloop
	beq	?eorlineloop
	
	lda	gam_PicPtr
	clc
	adc	gam_PicWidth
	sta	gam_PicPtr
	lda	#0
	adc	gam_PicPtr+1
	sta	gam_PicPtr+1
	
	lda	gam_VSPtr
	clc
	adc	#SCR_WBYTE
	sta	gam_VSPtr
	lda	#0
	adc	gam_VSPtr+1
	sta	gam_VSPtr+1
	
	dec	gam_VSHeight
	bne	?eorpictureloop
	rts

?clspirture:				;清除贴图
	lda	#0
	ldy	#0
?clsbyteloop:
	sta	gam_PicLine,y
	iny
	cpy	gam_VSWidth
	bcc	?clsbyteloop
	
	jsr	?setneekdeal
?clspictureloop:
	ldy	#0
?clslineloop:
	lda	gam_PicLine,y
	sta	gam_PicTmp
	lda	(gam_VSPtr),y
	and	gam_PicTmp
	sta	(gam_VSPtr),y
	iny
	cpy	gam_LineEndByte
	bcc	?clslineloop
	beq	?clslineloop
	
	lda	gam_PicPtr
	clc
	adc	gam_PicWidth
	sta	gam_PicPtr
	lda	#0
	adc	gam_PicPtr+1
	sta	gam_PicPtr+1
	
	lda	gam_VSPtr
	clc
	adc	#SCR_WBYTE
	sta	gam_VSPtr
	lda	#0
	adc	gam_VSPtr+1
	sta	gam_VSPtr+1
	
	dec	gam_VSHeight
	bne	?clspictureloop
	rts
	
	

?getlinebyte:				;取一行数据
	ldy	#0
?getbyteloop:
	lda	(gam_PicPtr),y
	sta	gam_PicLine,y
	iny
	cpy	gam_VSWidth
	bcc	?getbyteloop
	rts

?shiftline:				;行偏移调整
	lda	gam_BitOff
	bne	?shiftstart
	rts
?shiftstart:
	sta	gam_MulTmp
?shiftloop:
	ldy	gam_LineEndByte
	ldx	#1
	lsr	gam_PicLine
?shiftbyteloop:
	dey
	bmi	?shiftnextloop
	ror	gam_PicLine,x
	inx
	jmp	?shiftbyteloop
?shiftnextloop:
	dec	gam_MulTmp
	bne	?shiftloop
	rts

?setneekdeal:				;行首末偏移置1
	ldx	gam_BitOff
	lda	orgraphyrmask,x
	ora	gam_PicLine
	sta	gam_PicLine
	ldx	gam_EndLineOff
	lda	orgraphymask,x
	sta	gam_PicTmp
	ldx	gam_LineEndByte
	lda	gam_PicLine,x
	ora	gam_PicTmp
	sta	gam_PicLine,x
	rts
	
?clearneekdeal:				;行首末偏移清0
	ldx	gam_BitOff
	lda	graphymask,x
	and	gam_PicLine
	sta	gam_PicLine
	ldx	gam_EndLineOff
	lda	graphyrmask,x
	sta	gam_PicTmp
	ldx	gam_LineEndByte
	lda	gam_PicLine,x
	and	gam_PicTmp
	sta	gam_PicLine,x
	rts
	
	

;=======================================================================================
;定义本页生成的数据段名
GraLib_DataSec:	.section
		.GraLib_DataSec
;=======================================================================================
graphymask:
	db $ff,$7f,$3f,$1f,$0f,$07,$03,$01,$00
	
graphyrmask:
	db $ff,$80,$c0,$e0,$f0,$f8,$fc,$fe,$ff
orgraphymask:
	db $00,$7f,$3f,$1f,$0f,$07,$03,$01,$00
	
orgraphyrmask:
	db $00,$80,$c0,$e0,$f0,$f8,$fc,$fe,$ff

