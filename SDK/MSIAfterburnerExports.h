/////////////////////////////////////////////////////////////////////////////
//
// This header file defines functions exported by MSIAfterburner.exe 
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _MSIAFTERBURNER_EXPORTS_INCLUDED_
#define _MSIAFTERBURNER_EXPORTS_INCLUDED_

#ifdef MSIAFTERBURNER_EXPORTS
#define MSIAFTERBURNER_API extern "C" __declspec(dllexport)
#else
#define MSIAFTERBURNER_API extern "C" __declspec(dllimport)
#endif
 
/////////////////////////////////////////////////////////////////////////////
//
// host application IDs
//
/////////////////////////////////////////////////////////////////////////////
#define MSIAFTERBURNER_HOST_APP					'MSIA'
/////////////////////////////////////////////////////////////////////////////
//
// exported function names
//
/////////////////////////////////////////////////////////////////////////////
#define HOST_APP_PROPERTY_SKIN_COLOR_HEADER_BGND				0x0000C000
#define HOST_APP_PROPERTY_SKIN_COLOR_HEADER_TEXT				0x0000C001
/////////////////////////////////////////////////////////////////////////////
//R/O access to host application ID/version and properties

typedef DWORD (*GET_HOST_APP_ID_PROC		)();
	//DWORD GetHostAppID()
typedef DWORD (*GET_HOST_APP_VERSION_PROC	)();
	//DWORD GetHostAppVersion()
typedef BOOL  (*GET_HOST_APP_PROPERTY_PROC	)(DWORD, LPVOID, DWORD);
	//BOOL GetHostAppPropperty(DWORD dwProperty, LPVOID lpData, DWORD dwSize)

//R/O access to GPU identification

typedef DWORD	(*GET_GPU_NUM_PROC			)();
	//DWORD GetGpuNum()
typedef LPCSTR	(*GET_GPU_ID_PROC			)(DWORD);
	//LPCSTR GetGpuID(DWORD gpu)
typedef LPCSTR	(*GET_GPU_HANDLE_PROC		)(DWORD);
	//DWORD GetGpuHandle(DWORD gpu)

//R/W access to IO ports

typedef int				(*INORTP_PROC	)(unsigned short);
	//int inport(unsigned short port)
typedef unsigned short	(*INPORTW_PROC	)(unsigned short);
	//unsigned short inportw(unsigned short port)
typedef unsigned long	(*INPORTD_PROC	)(unsigned short);
	//unsigned long inportd(unsigned short port)

typedef int				(*OUTPORT_PROC	)(unsigned short, int);
	//int outport(unsigned short port, int data)
typedef unsigned short	(*OUTPORTW_PROC	)(unsigned short, unsigned short);
	//unsigned short outportw(unsigned short port, unsigned short data)
typedef unsigned long	(*OUTPORTD_PROC	)(unsigned short, unsigned long);
	//unsigned long outportd(unsigned short port, unsigned long data)

//R/W access to PCI bus configuration registers

typedef DWORD	(*READ_BUS_DATA_ULONG_PROC	)(DWORD, DWORD, DWORD);
	//DWORD ReadBusDataUlong(DWORD bus, DWORD devfn, DWORD offset)
typedef WORD	(*READ_BUS_DATA_USHORT_PROC	)(DWORD, DWORD, DWORD);
	//WORD ReadBusDataUshort(DWORD bus, DWORD devfn, DWORD offset)
typedef BYTE	(*READ_BUS_DATA_UCHAR_PROC	)(DWORD, DWORD, DWORD);
	//BYTE ReadBusDataUchar(DWORD bus, DWORD devfn, DWORD offset)

typedef void (*WRITE_BUS_DATA_ULONG_PROC	)(DWORD, DWORD, DWORD, DWORD);
	//void WriteBusDataUlong(DWORD bus, DWORD devfn, DWORD offset, DWORD data)
typedef void (*WRITE_BUS_DATA_USHORT_PROC	)(DWORD, DWORD, DWORD, WORD	);
	//void WriteBusDataUshort(DWORD bus, DWORD devfn, DWORD offset, WORD data)
typedef void (*WRITE_BUS_DATA_UCHAR_PROC	)(DWORD, DWORD, DWORD, BYTE	);
	//void WriteBusDataUchar(DWORD bus, DWORD devfn, DWORD offset, BYTE data)

//R/W access to MSR

typedef BOOL	(*READ_MSR_PROC	)(DWORD, LPDWORD, LPDWORD);
	//BOOL ReadMSR(DWORD dwIndex, LPDWORD lpHigh, LPDWORD lpLow)
typedef BOOL	(*WRITE_MSR_PROC)(DWORD, DWORD, DWORD);
	//BOOL WriteMSR(DWORD dwIndex, DWORD dwHigh, DWORD dwLow)

//R/W access to GPU registers 

typedef DWORD	(*READ_REGISTER_ULONG_PROC	)(DWORD, DWORD);
	//DWORD ReadRegisterUlong(DWORD gpu, DWORD address)
typedef WORD	(*READ_REGISTER_USHORT_PROC	)(DWORD, DWORD);
	//WORD ReadRegisterUshort(DWORD gpu, DWORD address)
typedef BYTE	(*READ_REGISTER_UCHAR_PROC	)(DWORD, DWORD);
	//BYTE ReadRegisterUchar(DWORD gpu, DWORD address)

typedef void	(*WRITE_REGISTER_ULONG_PROC	)(DWORD, DWORD, DWORD);
	//void WriteRegisterUlong(DWORD gpu, DWORD address, DWORD data)
typedef void	(*WRITE_REGISTER_USHORT_PROC)(DWORD, DWORD, WORD);
	//void WriteRegisterUshort(DWORD gpu, DWORD address, WORD data)
typedef void	(*WRITE_REGISTER_UCHAR_PROC	)(DWORD, DWORD, BYTE);
	//void WriteRegisterUchar(DWORD gpu, DWORD address, BYTE data)

//R/W access to GPU I2C buses

typedef BYTE	(*GET_I2C_BUS_NUM_PROC	)(DWORD);
	//BYTE GetI2CBusNum(DWORD gpu)
typedef BOOL	(*READ_I2C_PROC			)(DWORD, BYTE,BYTE,BYTE,LPBYTE,DWORD);
	//BOOL ReadI2C(DWORD gpu, BYTE bus, BYTE dev, BYTE off, LPBYTE lpData, DWORD dwSize)
typedef BOOL	(*WRITE_I2C_PROC		)(DWORD, BYTE,BYTE,BYTE,LPBYTE,DWORD);
	//BOOL WriteI2C(DWORD gpu, BYTE bus, BYTE dev, BYTE off, LPBYTE lpData, DWORD dwSize)

//localization

typedef void	(*LOCALIZEWND_PROC	)(HWND);
	//void LocalizeWnd(HWND hWnd)
typedef LPCSTR	(*LOCALIZESTR_PROC	)(LPCSTR);
	//LPCSTR LocalizeStr(LPCSTR lpStr)

//misc

typedef DWORD	(*GET_TIMESTAMP_PROC)();
	//get hardware monitoring timestamp

#endif

