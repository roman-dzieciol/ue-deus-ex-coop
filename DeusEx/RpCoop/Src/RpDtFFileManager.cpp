/*=============================================================================
	RpDt.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"
#include "RpDtMacros.h"




/*-----------------------------------------------------------------------------
	FFileManager
-----------------------------------------------------------------------------*/

RP_DETOUR_CLASS( FFileManager, FArchive*, CreateFileReader, ( const TCHAR* Filename, DWORD ReadFlags, FOutputDevice* Error ) )
{
	if( GRpFileOverride )
	{
		GRpFileOverride = FALSE;
		RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%.8x],[%p]"), RP_LOGP, Filename, ReadFlags, Error );
		FString newname = RpGetDeltaPath(Filename);
		FArchive* r = RP_DETOUR_TP( CreateFileReader(*newname,ReadFlags,Error) );
		RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%.8x],[%p] [%p]"), RP_LOGP, *newname, ReadFlags, Error, r );
		return r;
	}
	else
	{
		//RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%.8x],[%p]"), RP_LOGP, Filename, ReadFlags, Error );
		FArchive* r = RP_DETOUR_TP( CreateFileReader(Filename,ReadFlags,Error) );
		//RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%.8x],[%p] [%p]"), RP_LOGP, Filename, ReadFlags, Error, r );
		return r;
	}
}}

RP_DETOUR_CLASS( FFileManager, FArchive*, CreateFileWriter, ( const TCHAR* Filename, DWORD WriteFlags, FOutputDevice* Error ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%.8x],[%p]"), RP_LOGP, Filename, WriteFlags, Error );
	FArchive* r = RP_DETOUR_TP( CreateFileWriter(Filename,WriteFlags,Error) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%.8x],[%p] [%p]"), RP_LOGP, Filename, WriteFlags, Error, r );
	return r;
}}

RP_DETOUR_CLASS( FFileManager, INT, FileSize, ( const TCHAR* Filename ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, Filename );
	INT r = RP_DETOUR_TP( FileSize(Filename) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%d]"), RP_LOGP, Filename, r );
	return r;
}}

RP_DETOUR_CLASS( FFileManager, UBOOL, Delete, ( const TCHAR* Filename, UBOOL RequireExists, UBOOL EvenReadOnly ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%d],[%d]"), RP_LOGP, Filename,RequireExists,EvenReadOnly );
	UBOOL r = RP_DETOUR_TP( Delete(Filename,RequireExists,EvenReadOnly) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%d],[%d] [%d]"), RP_LOGP, Filename,RequireExists,EvenReadOnly, r );
	return r;
}}

RP_DETOUR_CLASS( FFileManager, UBOOL, Copy, ( const TCHAR* Dest, const TCHAR* Src, UBOOL Replace, UBOOL EvenIfReadOnly, UBOOL Attributes, void (*Progress)(FLOAT Fraction) ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%s],[%d],[%d],[%d],[%p]"), RP_LOGP, Dest,Src,Replace,EvenIfReadOnly,Attributes,(void*)Progress );
	UBOOL r = RP_DETOUR_TP( Copy(Dest,Src,Replace,EvenIfReadOnly,Attributes,Progress) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%s],[%d],[%d],[%d],[%p] [%d]"), RP_LOGP, Dest,Src,Replace,EvenIfReadOnly,Attributes,(void*)Progress, r );
	return r;
}}

RP_DETOUR_CLASS( FFileManager, UBOOL, Move, ( const TCHAR* Dest, const TCHAR* Src, UBOOL Replace, UBOOL EvenIfReadOnly, UBOOL Attributes ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%s],[%d],[%d],[%d]"), RP_LOGP, Dest,Src,Replace,EvenIfReadOnly,Attributes );
	if( Replace )
	{
		FString dest = Dest;
		FString src = Src;
		if( dest.Caps().Right(4) == TEXT(".UXX") && src.Caps().Right(4) == TEXT(".TMP") )
		{
			RpOnMove(Dest,Src,Replace,EvenIfReadOnly,Attributes);
		}
	}
	UBOOL r = RP_DETOUR_TP( Move(Dest,Src,Replace,EvenIfReadOnly,Attributes) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%s],[%d],[%d],[%d] [%d]"), RP_LOGP, Dest,Src,Replace,EvenIfReadOnly,Attributes, r );
	return r;
}}


void RpDtFFileManager( FFileManager* p )
{
	rpguard(RpDtFFileManager);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: [%s]"), RP_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	PBYTE FFileManager = PBYTE(p);

	RP_DETOUR_VR( FFileManager, 0x00, 0x00, CreateFileReader );
	//RP_DETOUR_VR( FFileManager, 0x00, 0x04, CreateFileWriter );
	//RP_DETOUR_VR( FFileManager, 0x00, 0x08, FileSize );
	RP_DETOUR_VR( FFileManager, 0x00, 0x0C, Delete );
	//RP_DETOUR_VR( FFileManager, 0x00, 0x10, Copy );
	RP_DETOUR_VR( FFileManager, 0x00, 0x14, Move );

	unguard;
}

/*
	virtual FArchive* CreateFileReader( const TCHAR* Filename, DWORD ReadFlags=0, FOutputDevice* Error=GNull )=0;
	virtual FArchive* CreateFileWriter( const TCHAR* Filename, DWORD WriteFlags=0, FOutputDevice* Error=GNull )=0;
	virtual INT FileSize( const TCHAR* Filename )=0;
	virtual UBOOL Delete( const TCHAR* Filename, UBOOL RequireExists=0, UBOOL EvenReadOnly=0 )=0;
	virtual UBOOL Copy( const TCHAR* Dest, const TCHAR* Src, UBOOL Replace=1, UBOOL EvenIfReadOnly=0, UBOOL Attributes=0, void (*Progress)(FLOAT Fraction)=NULL )=0;
	virtual UBOOL Move( const TCHAR* Dest, const TCHAR* Src, UBOOL Replace=1, UBOOL EvenIfReadOnly=0, UBOOL Attributes=0 )=0;
	virtual SQWORD GetGlobalTime( const TCHAR* Filename )=0;
	virtual UBOOL SetGlobalTime( const TCHAR* Filename )=0;
	virtual UBOOL MakeDirectory( const TCHAR* Path, UBOOL Tree=0 )=0;
	virtual UBOOL DeleteDirectory( const TCHAR* Path, UBOOL RequireExists=0, UBOOL Tree=0 )=0;
	virtual TArray<FString> FindFiles( const TCHAR* Filename, UBOOL Files, UBOOL Directories )=0;
	virtual UBOOL SetDefaultDirectory( const TCHAR* Filename )=0;
	virtual FString GetDefaultDirectory()=0;
*/

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/