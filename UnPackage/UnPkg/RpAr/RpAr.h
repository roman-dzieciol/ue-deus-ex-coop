// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RPAR_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RPAR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef RPAR_EXPORTS
#define RPAR_API __declspec(dllexport)
#else
#define RPAR_API __declspec(dllimport)
#endif


enum EArchiveFlags
{
	AF_None					= 0x00000000
,	AM_SilentUnpack			= 0x00000010
,	AM_Verbose				= 0x00000020
,	AM_DumpHashes			= 0x00000040
,	AM_AllFlags				= 0xFFFFFFFF
};


extern "C" RPAR_API int RpPack( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int flags=0 );
extern "C" RPAR_API int RpUnpack( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int flags=0 );
extern "C" RPAR_API int RpIsError( int idx );

int RpPackInternal( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int flags=0 );
int RpUnpackInternal( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int flags=0 );

// This class is exported from the RpAr.dll
//class RPAR_API CRpAr {
//public:
//	CRpAr(void);
//	// TODO: add your methods here.
//};
//
//extern RPAR_API int nRpAr;
//
//RPAR_API int fnRpAr(void);
