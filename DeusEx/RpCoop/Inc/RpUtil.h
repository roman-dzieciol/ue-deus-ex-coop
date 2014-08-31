/*=============================================================================
	RpUtil.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#pragma once




/*-----------------------------------------------------------------------------
	Files.
-----------------------------------------------------------------------------*/

extern void RpDeleteDirectoryContents( const TCHAR* Path );
extern void RpExplodePath( const FString& fullpath, FString& path, FString& file, FString& name, FString& ext );


/*-----------------------------------------------------------------------------
	Dumps.
-----------------------------------------------------------------------------*/

extern void RpDumpLevels();
extern void RpDumpTravelInfo( const TMap<FString,FString>* TravelInfo );
extern void RpDumpRefs();



/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
