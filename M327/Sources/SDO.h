/*
 * SDO.h
 *
 *  Created on: Jan 2, 2016
 *      Author: gianluca
 */

#ifndef SDO_H_
#define SDO_H_

/* MODULE SDO */

#define SDO_BUFFER_SIZE	1024

#define SDO_BLOCK_SIZE	127

#define SDO_SEGMENT_SIZE	8

#define SDOErrorOK					0x00000000

#define SDOErrorToggleBit			0x05030000

#define SDOErrorProtocolTimeOut		0x05040000

#define SDOErrorCommandUnknow		0x05040001

#define SDOErrorInvalidBlockSize	0x05040002

#define SDOErrorInvalidSeqNumber	0x05040003

#define SDOErrorCRCError			0x05040004

#define SDOErrorOutMemory			0x05040005

#define SDOErrorUnsupportedAccess	0x06010000

#define SDOErrorWO					0x06010001

#define SDOErrorRO					0x06010002

#define SDOErrorNotExist			0x06020000

#define SDOErrorPDOMapping			0x06040041

#define SDOErrorPDOLenght			0x06040042

#define SDOErrorParamInc			0x06040043

#define SDOErrorIntDevInc			0x06040047

#define SDOErrorHwError				0x06060000

#define SDOErrorDataType1			0x06070010

#define SDOErrorDataType2			0x06070012

#define SDOErrorDataType3			0x06070013

#define SDOErrorSubIndex			0x06090011



enum SDOProtocols
{
	SDONull,		// Nessun protocollo attivo
	SDOsDownloadClient,	// SDO Segment Download Client
	SDOsDownloadServer,	// SDO Segment Download Server
	SDOsUploadClient,	// SDO Segment Upload Client
	SDOsUploadServer,	// SDO Segment Upload Server
	SDObDownloadClient,	// SDO Block Download Client
	SDObDownloadServer,	// SDO Block Download Server
	SDObUploadClient,	// SDO Block Upload	Server
	SDObUploadServer	// SDO Block Upload	Client
};



typedef struct td_SDOcInitDownload
{
	byte ccs:	3;
	byte X: 	1;
	byte n:		2;
	byte e:		1;
	byte s:		1;
	byte b0;
	byte m[3];
	byte data[4];		
} SDOInitDownloadData;


typedef struct td_SDOcDownloadSegment
{
	byte ccs:	3;
	byte t:		1;
	byte n:		3;
	byte c:		1;
	byte segData[7];
}SDOcDownlodSegmentData;

typedef struct td_SDOcInitDownloadConf
{
	byte scs:	3;
	byte X:		5;
	byte m[3];
	byte reserved[4];
	
}SDOInitDownloadDataConf;


typedef struct td_SDOcDownloadSegmentConf
{
	byte ccs:	3;
	byte t:		1;
	byte X:		4;	
	byte reserved[7];
}SDOcDownlodSegmentDataConf;


typedef union td_SDOcDataReq
{
	SDOInitDownloadData InitiateDownload;
	SDOcDownlodSegmentData DownloadSegment;
}SDOcDataReq;

typedef union td_SDOcDataResp
{
	SDOInitDownloadDataConf InitiateDownload;
	SDOcDownlodSegmentDataConf	DownloadSegment;
}SDOcDataConf;


/* 
 * Struttura Abort SDO Transfer Protocol
 */
 typedef struct
 {
	 byte X:	 5;
	 byte cs: 3;
 	
 	
 }td_SDOAbortSDOB0;
 
 typedef struct
 {
 	td_SDOAbortSDOB0 byte0;
 	byte m0;
 	byte m1;
 	byte m2;
 	byte d[4];
 	
 }td_SDOAbortSDO;
 
/*====================================
 * Struttura per SDO Download Protocol
 *====================================*/
 
 /* Initiate SDO Download Protocol Request*/
 
typedef struct 
{
	
	byte s:		1;
	byte e:		1;
	byte n:		2;
	byte X:		1;
	byte ccs:	3;
	
	
}td_SDOsInitDownloadReqB0;


typedef struct
{
	td_SDOsInitDownloadReqB0 byte0;
	
	byte m0;
	byte m1;
	byte m2;
	byte d[4];
	
	
}td_SDOsInitDownloadReq;



/* Initiate SDO Download Protocol Confirm */
 
typedef struct 
{		
	byte X:		5;	
	byte scs:	3;
	
}td_SDOsInitDownloadConfB0;


typedef struct
{
	td_SDOsInitDownloadConfB0 byte0;
	
	byte m0;
	byte m1;
	byte m2;
	byte reserved0;
	byte reserved1;
	byte reserved2;
	byte reserved3;
	
}td_SDOsInitDownloadConf;


/* Download SDO Segment Protoco Request */

typedef struct 
{
	byte c:		1;
	byte n:		3;
	byte t:		1;
	byte ccs:	3;
}td_SDOsDownloadReqB0;

 
typedef struct
{
	td_SDOsDownloadReqB0 byte0;
	byte segData[7];
	
}td_SDOsDownloadReq;
 
/* Download SDO Segment Protocol Confirm */
typedef struct
{
	byte X:		4;
	byte t:		1;
	byte scs:	3;
	
}td_SDOsDownloadConfB0;

typedef struct
{
	td_SDOsDownloadConfB0 byte0;
	byte reserved[7];
	
} td_SDOsDownloadConf;

/* Strutture per SDO Upload Protocol */

/* Initiate SDO Upload Protocol Request*/
 
typedef struct 
{
	
	byte X:		5;
	byte ccs:	3;
	
	
}td_SDOsInitUploadReqB0;


typedef struct
{
	byte byte0;
	
	byte m0;
	byte m1;
	byte m2;
	byte reserved0;
	byte reserved1;
	byte reserved2;
	byte reserved3;
	
}td_SDOsInitUploadReq;

/* Initiate SDO Upload Protocol Confirm */
 
typedef struct 
{
	byte s:		1;
	byte e:		1;
	byte n:		2;		
	byte X:		1;	
	byte scs:	3;
	
}td_SDOsInitUploadConfB0;



typedef struct
{
	td_SDOsInitUploadConfB0 byte0;
	
	byte m0;
	byte m1;
	byte m2;
	
	byte d[4];
	
}td_SDOsInitUploadConf;


/* SDO Upload Protocol Request */

typedef struct
{
	byte X:		4;
	byte t:		1;
	byte ccs:	3;
} td_SDOsUploadReqB0;

typedef struct
{
	td_SDOsUploadReqB0 byte0;
	byte reserved0;
	byte reserved1;
	byte reserved2;
	byte reserved3;
	byte reserved4;
	byte reserved5;
	byte reserved6;	
	
} td_SDOsUploadReq;

/* SDO Upload Protocol Confirm */
typedef struct
{
	byte c:		1;
	byte n:		3;
	byte t:		1;
	byte scs:	3;
} td_SDOsUploadConfB0;

typedef struct
{
	td_SDOsUploadConfB0 byte0;
	byte segData0;
	byte segData1;
	byte segData2;
	byte segData3;
	byte segData4;
	byte segData5;
	byte segData6;
	
}td_SDOsUploadConf;
/*
 * Struttura Initiate Block Download Request 
 */
typedef struct 
{
	byte cs:	1;
	byte s:		1;
	byte cc:	1;
	byte X:		2;
	byte ccs:	3;
	
}td_SDObInitDownloadReqB0;


typedef struct
{
	td_SDObInitDownloadReqB0 byte0;
	
	byte m[3];
	byte size[4];
	
}td_SDObInitDownloadReq;




typedef struct
{
			
	byte ss:		2;
	byte sc:		1;
	byte X:			2;
	byte scs:		3;
	
}td_SDObInitDownloadConfB0;

typedef struct 
{
	td_SDObInitDownloadConfB0 byte0;
	byte m[3];
	byte blksize;
	byte reserved[3];	
	
}td_SDObInitDownloadConf;

typedef struct td_SDOsInitUploadReq0
{
	byte cs:	2;
	byte cc:	1;
	byte X:		2;
	byte ccs:	3;
	byte dummy0;
	
	word index;
	byte subindex;
	byte blksize;
	byte pst;
	byte dummy1;
}SDOsInitUploadReq0Data;



extern word numSDOSupperted;

extern word currSDOFrame;

void SDOInit();

void SDOServer();


bool InitSDOBlockDownloadProtocol();

void SDOCommandDownload(int numStation, byte *buffOr, word index, byte subIndex, word size);

void SDOCommandUpload(int numStation, word index, byte subIndex);


#endif /* SDO_H_ */
