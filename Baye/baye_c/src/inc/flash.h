#ifndef		FLASH_H
#define		FLASH_H

/******************************************************************************************
*				
*	Copyright (c)2002 , 广东步步高教育电子分公司
*	All rights reserved.
**
*	文件名称：flash.h
*	文件标识：A600系统
*	摘    要：A600内存堆管理函数头文件
**
*	修改历史：
*	版本    	日期    	 作者    	 改动内容和原因
*   ------		-------		---------	------------------------------
*	1.0    		2002.08.20   	高大明      	完成基本内容
*******************************************************************************************/

typedef	struct	tagDictFileFindBlk
{	
	U16	fileName;				/*文件名	*/
	U32	position;				/*位置信息	*/
	U8	title[32];				/*标题		*/

	U8	fileType;				/*文件类型	*/
	U8	executable;				/*可否执行	*/
	
}TypeDictFileFindBlk,*PtrDictFileFindBlk;


/*



	1.	;------------------------------------------------------------------------
		;功能    :文件创建函数						      	
		;U8 	FileCreat( U8 filetype,U32 filelength, U8 * information,U16 * filename,U8 * filehandle);		      	;
		;-----------------------------------------------------------------------
		;入口    :U8 filetype,U32 filelength, U8 * information;			;
		;出口    :U16 *filename,U8 *filehandle;					;
		;									;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 创建一个文件,分配空间,并打开该文件, 文件指针指向文件的第一的字节.	;
		;U8	information[10],在同类型文件中是唯一的.				;
		;------------------------------------------------------------------------
*/
FAR	U8 	FileCreat( U8 filetype,U32 filelength, U8 * information,U16 * filename,U8 *filehandle);

/*	2.	FileUpdata_Begin:
		;-----------------------------------------------------------------------;
		;功能    :修改文件开始							;
		;U8 FileUpdata_Begin(U8 filehandle);					;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 修改文件开始.								;
		;-----------------------------------------------------------------------;
*/
FAR	U8 	FileUpdata_Begin(U8 filehandle);

/*	3.	FileUpdata_End:
		;-----------------------------------------------------------------------;
		;功能    :修改文件结束							;
		;U8 FileUpdata_End();					;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 修改文件结束.								;
		;-----------------------------------------------------------------------;
*/

FAR	U8 	FileUpdata_End();


/*	4.	FileOpen:
		;---------------------------------------------------------------------- ;
		;功能    :打开一个文件							;
		;U8  FileOpen(U16 filename, U8 filetype,U8 openmode,U8 * filehandle,U8 * filelength);	
		;入口    :U16 filename, U8 filetype,U8 openmode				;
		;出口    :U8 * filehandle						;
		;堆栈使用:无								;
		;全局变量:无								;
		;说明:									;
		; 打开一个文件.分配文件句柄filehandle.					;
		; U8 openmode三种打开方式:						;
		;	#define NoOpen		 0x00					;
		;	#define	ReadOnly	 0x01					;
		;	#define	ReadAndWrite	 0x02					;
		; 支持同时打开四个文件; 文件指针指向文件的第一的字节.			;
		; 									;
		;-----------------------------------------------------------------------;	
*/
FAR	U8 	FileOpen(U16 filename, U8 filetype,U8 openmode,U8 * filehandle, U32 *filelength);

/*	5.	FileDel:
		;-----------------------------------------------------------------------;
		;功能    :删除一个文件							;
		;U8 filedel(U8 filehandle);						;
		;入口    :无								;
		;出口    :U8 filehandle							;
		;堆栈使用:无								;
		;全局变量:无								;
		;说明:		  							;
		; 删除一个文件,并且关闭此文件.			
		;-----------------------------------------------------------------------;
*/	

FAR	U8 	FileDel(U8 filehandle);

		
/*	6.	FileWrite:
		;-----------------------------------------------------------------------;
		;功能    :向一个文件写数据						;
		;U8 	FileWrite(U8 filehandle,U8 datalength,U8 * bufadd);		;
		;入口    :U8 filehandle,U8 datalength,U8 * bufadd					;
		;出口    :无							;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 写完后,文件指针指向说写数据的下一个字节				;
		;-----------------------------------------------------------------------;
*/	

FAR	U8 	FileWrite(U8 filehandle,U8 datalength,U8 * bufadd);

/*	7.	FileClose:
		;-----------------------------------------------------------------------;
		;功能    :关闭一个文件							;
		;U8 fileclose(U8 filehandle);						;
		;入口    :U8 filehandle							;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		;说明:	关闭一个文件.支持同时打开四个文件.				;
		;-----------------------------------------------------------------------;	
*/
	
FAR	U8	FileClose(U8 filehandle);

/*	8.	FileRead:
		;-----------------------------------------------------------------------;
		;功能    :读一个文件的数据						;
		;U8 FileRead(U8 filehandle,U8 datalength,U8 * bufadd);			;
		;入口    :U8 filehandle,U8 datalength,U8 * bufadd					;
		;出口    :无							;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 写完后,文件指针指向说读数据的下一个字节				;
		;-----------------------------------------------------------------------;	
*/
FAR	U8	FileRead(U8 filehandle,U8 datalength,U8 * bufadd);

/*	9.	FileSeek:
		;-----------------------------------------------------------------------;
		;功能    :文件定位函数							;
		;U8 FileSeek(U8 filehandle,U32 fileoffset,U8 origin);			;
		;入口    :U8 filehandle,U32 fileoffset,U8 origin			;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		; 说明:									;
		;      	origin:								;
		;		#define FromTop		0x01				;
		;		#define	FromCurrent	0x02				;
		;		#define	FromEnd		0x03	  			;
		;   	fileoffset:							;
		; 		可以为负数.						;
		;-----------------------------------------------------------------------;	
*/	
FAR	U8 	FileSeek(U8 filehandle,U32 fileoffset,U8 origin);

/*	10.	ResetFlash:
		;-----------------------------------------------------------------------;
		;功能    :擦除FLASH的所有数据						;
		;void	ResetFlash();							;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 按复位键后调用.							;	
		;-----------------------------------------------------------------------;
*/
	
FAR	void	ResetFlash(void);

/*	11.	FileClearDataChip:	
		;-----------------------------------------------------------------------;
		;功能    :整理数据扇区							;
		;void	clearchip_data();						;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 关机状改下整理数据扇区.						;
		;-----------------------------------------------------------------------;
*/
FAR	void	FileClearDataChip(void);

/*	12.	FlashInit:
		;-----------------------------------------------------------------------;
		;功能    :初始化FLASH的一些变量						;
		;void	FlashInit();							;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 开机时调用.(进入个模块时也可以调用,可避免别的模块的干扰).							;	
		;-----------------------------------------------------------------------;
*/	
FAR	void	FlashInit(void);

/*	13.	FileTell:
		;-----------------------------------------------------------------------;
		;功能    :读一个文件的状态信息						;
		;U8	 FileTell(U8 filehandle,U16 * filename,U8 * information,U32 * filelength);				
		;入口    :U8 filehandle							;
		;出口    :U16 *filename,U8 * information,U32 * filelength		;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 返回一个文件的各种信息.						;	
		;-----------------------------------------------------------------------;	
*/
FAR	U8	FileTell(U8 filehandle,U16 * filename,U8 * information,U32 * filelength);

/*	14.	FileFreeSector:
		;-----------------------------------------------------------------------;
		;功能    :可下载的扇区数.						;
		;U8 	FileFreeSector(U16 * count_FileFreeSector)				;				
		;入口    :无								;
		;出口    :U16 * count_FileFreeSector					;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 可下载的扇区数.							;	
		;-----------------------------------------------------------------------;	
*/	
FAR	U8 	FileFreeSector(U16 * count_FileFreeSector);

/*	15.	FileChangeInf:
		;-----------------------------------------------------------------------;
		;功能    :修改文件的信息字段							;
		;U8 FileChangeInf(U8 filehandle,U8 * information);					;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 修改文件的信息字段.								;
		;-----------------------------------------------------------------------;	
*/

FAR	U8 	FileChangeInf(U8 filehandle,U8 * information);

/*	16.	Filenum:
		;-----------------------------------------------------------------------;
		;功能    :返回某类型文件的数量.						;
		;U8 	Filenum( U8 filetype,U16 * filenum);			;				
		;入口    :U8 filetype;							;
		;出口    :U16 * filenum							;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 返回某类型文件的数量.							;	
		;-----------------------------------------------------------------------;
*/
FAR	U8 	FileNum( U8 filetype,U16 * filenum);
		
/*	17.	FileSearch:
		;-----------------------------------------------------------------------;
		;功能    :浏览 某类型的第fileorder个文件的information.			;
		;U8 	FileSearch(U8 filetype,U16 fileorder,U16 * filename,U8 * information);				;				
		;入口    :U8 fls_filetype,U16 fileorder;				;
		;出口    :U16 * filename,U8 * information;				;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 通过比较information[10],知道是否是要打开的文件.			;	
		;-----------------------------------------------------------------------;
*/

FAR	U8 	FileSearch(U8 filetype,U16 fileorder,U16 * filename,U8 * information);


/*	18.	FileClearIndexChip:
		;-----------------------------------------------------------------------;
		;功能    :整理索引扇区							;
		;void	clearchip_data();						;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 关机状改下整理索引扇区.						;	
		;-----------------------------------------------------------------------;
*/
FAR	void 	FileClearIndexChip(void);



/*	20.	FileSearchInf:
		;-----------------------------------------------------------------------;
		;功能    :查找文件。（名片专用）							;
		;U8 	FileSearchInf(U16 CardKey,U16 * filename, U8 * information);
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 查找文件.						;	
		;-----------------------------------------------------------------------;
*/

FAR	U8 	FileSearchInf(U8 FileType ,U16 CardKey,U16 * filename, U8 * information);

/*	21.	FileChangeCardInf:
		;-----------------------------------------------------------------------;
		;功能    :查找文件。（名片专用）					;		;
		;U8	FileChangeCardInf(U8 filetype, U16  filename,U8 order, U8 Byte);	;
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 查找文件.								;	
		;-----------------------------------------------------------------------;
*/

FAR	U8	FileChangeCardInf(U8 filetype, U16  filename,U8 order, U8 Byte);

/*	22.	FileSectorUnite:
		;-----------------------------------------------------------------------;
		;功能    :合并小数据扇区						;
		;void	FileSectorUnite();						;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 关机下状合并小数据扇区.						;	
		;-----------------------------------------------------------------------;
*/
FAR	void	FileSectorUnite(void);

/*	23.	FileGetLogAdr:
		;-----------------------------------------------------------------------;
		;功能    :获得下载文件的逻辑地址。					;		;
		;U8	FileGetLogAdr(U8 filehandle, U32 * LogAdr);			;			;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 获得下载文件的逻辑地址.						;	
		;-----------------------------------------------------------------------;
*/

FAR	U8	FileGetLogAdr(U8 filehandle, U32 * LogAdr);

/*	24.	FileGetPhyAdr:
		;-----------------------------------------------------------------------;
		;功能    :获得下载文件的物理地址。					;		;
		;U8	FileGetPhyAdr(U8 filehandle, U32 * PhysicalAdr);		;			;				
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 获得下载文件的物理地址。						;	
		;-----------------------------------------------------------------------;
*/
FAR	U8	FileGetPhyAdr(U8 filehandle, U32 * PhysicalAdr);

/*	25.	FileFlashStartAdr:
		;-----------------------------------------------------------------------;
		;功能    :获得flash的物理起始地址。					;
		;U8	FileFlashStartAdr(U8 filehandle, U32 * FlashAdr);		;
		;入口    :无								;
		;出口    :无								;
		;堆栈使用:无								;
		;全局变量:无								;
		;		  							;
		; 说明:									;
		; 获得下载文件的flash内的偏移地址。.						;	
		;-----------------------------------------------------------------------;
*/
FAR	U8	FileFlashStartAdr(U32 * FlashAdr);

FAR	void	FileClearChip();



/*	27.	FileBackupFlash:
		;-----------------------------------------------------------------------;
		;备份：
		;函数声明：	FAR U8	FileBackupFlash(U8 dataLength, U8 *dataBuf);
		;返回值：	
		;
		;-----------------------------------------------------------------------;
*/

FAR U8	FileBackupFlash(U8 dataLength, U8 *dataBuf,U32 * FlashAdr);


FAR U8	FileRestoreFlash(U8 dataLength, U8 *dataBuf,U32 * FlashAdr);


FAR U8	FlashCapacity(U32	*capacity);



FAR void FileRepairFlash();


FAR	U8	FileFindFirst(U8	fileType,PtrDictFileFindBlk pFindBlk );

FAR	U8	FileFindNext(PtrDictFileFindBlk pFindBlk);


/*		Flash_CheckSum:
		;-----------------------------------------------------------------------;
		;flash校验：
		;函数声明：	FAR	U8	Flash_CheckSum(U32	*CheckSum);
		;返回值：	1:flash   读写正常  ； 0：flash 错误
		;参数：		CheckSum：校验和
		;-----------------------------------------------------------------------;
*/

FAR	U8	FlashCheckSum(U32	*CheckSum);



#define RestoreFlash   				FileRestoreFlash
#define BackupFlash   				FileBackupFlash
#define Flash_CheckSum    			FlashCheckSum 
#define clearchip_data    			FileClearDataChip 
#define SysFileFindNext   			FileFindNext 
#define SysFileFindFirst   			FileFindFirst 
#define FreeSector    				FileFreeSector 
#define ChangeInf    				FileChangeInf 
#define FilenumByType    			FileNum 
#define fun_fls_BrowsefileByOrder    		FileSearch 
#define clearchip_index    			FileClearIndexChip
#define InformationSearch    			FileSearchInf 
#define ChangeInfoByte    			FileChangeCardInf
#define MoveLittleData   			FileSectorUnite  
#define SysFileGetLogAdr    			FileGetLogAdr 
#define SysFileGetPhyAdr    			FileGetPhyAdr 
#define SysFileGetFlashAdr    			FileFlashStartAdr 
#define fun_fls_Recycle    			FileClearChip
#define GetFlashCapacity    			FlashCapacity
#define Sys_fixFlash    			FileRepairFlash 


#endif





