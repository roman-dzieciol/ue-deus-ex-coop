// ============================================================================
//	Rp.cpp
//	Copyright 2007 Roman Dzieciol. All Rights Reserved.
// ============================================================================

// Includes.
#include "RpCoop.h"


FString RpGetDeltaPath(const TCHAR* Filename)
{
	rpguard(RpGetDeltaPath);			
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, Filename );
	
	// result
	FString r = Filename;

	// explode path
	FString filename = Filename;
	FString mappath;
	FString mapfile;
	FString mapname;
	FString mapext;

	if( filename.Split(TEXT("\\"),&mappath,&mapfile,TRUE)
	&&  mapfile.Split(TEXT("."),&mapname,&mapext,TRUE) )
	{
		r = FString::Printf(TEXT("..\\MapsTemp\\%s.dxh"),*mapname);
	}

	//RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s] [%s] [%s]"), RP_LOGP, *mappath, *mapfile, *mapname, *mapext );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s]"), RP_LOGP, Filename, *r );
	return r;
	unguard;
}


void RpOnMove( const TCHAR* Dest, const TCHAR* Src, UBOOL Replace, UBOOL EvenIfReadOnly, UBOOL Attributes )
{
	rpguard(RpOnMove);			
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%s],[%d],[%d],[%d]"), RP_LOGP, Dest,Src,Replace,EvenIfReadOnly,Attributes );

	GRpCacheDest = Dest;
	GRpCacheSrc = Src;

	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%s],[%d],[%d],[%d]"), RP_LOGP, Dest,Src,Replace,EvenIfReadOnly,Attributes );
	unguard;
}


void RpOnReceivedFile( UNetConnection* Connection, INT PackageIndex, const TCHAR* Error )
{
	rpguard(RpOnReceivedFile);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%d] [%s]"), RP_LOGP, *ToStr(Connection), PackageIndex, Error );

	// Explode cache name
	FString cachepath;
	FString cachefile;
	FString cachename;
	FString cacheext;

	if( !GRpCacheDest.Split(TEXT("\\"),&cachepath,&cachefile,TRUE)
	||  !cachefile.Split(TEXT("."),&cachename,&cacheext,TRUE) )
		appThrowf( TEXT("Invalid cache filename [%s]"), *GRpCacheDest );

	// Get map name
	FString mapname;
	for( TArray<UChannel*>::TIterator it=Connection->OpenChannels; it; ++it )
	{
		UChannel* c = *it;
		if( c->ChType == CHTYPE_File  )
		{
			UFileChannel* fc = (UFileChannel*)c;
			if( GRpCacheSrc == fc->Filename )
			{
				mapname = fc->PrettyName;
				break;
			}
		}
	}

	if( !mapname )
		appThrowf( TEXT("Unable to get map name for [%s] [%s]"), *GRpCacheSrc, *GRpCacheDest );

	
	//RpDeleteDirectoryContents(TEXT("..\\MapsTemp"));
	//RpDeleteDirectoryContents(TEXT("..\\Cache"));

	// Rename cache file to .dxd file
	FString movepath = FString::Printf(TEXT("..\\MapsTemp\\%s.dxc"),*mapname);
	if( !GFileManager->Move( *movepath, *GRpCacheDest, TRUE, TRUE ) )
		appThrowf( TEXT("Move failed [%s] -> [%s]"), *GRpCacheDest, *movepath );

	
	FString mappath = FString::Printf(TEXT("..\\MapsDB\\%s.dx"),*mapname);
	FString savepath = FString::Printf(TEXT("..\\MapsTemp\\%s.dx"),*mapname);;
	FString deltapath = movepath;

	//  Apply delta
	rpguard(RpUnpack);
	RP_LOG( RP_NAME, TEXT("%s >> RpUnpack :: [%s] [%s] [%s]"), *SwTimeStr(), *mappath,*savepath,*deltapath );
	RpUnpack(*mappath,*savepath,*deltapath);
	RP_LOG( RP_NAME, TEXT("%s << RpUnpack :: [%s] [%s] [%s]"), *SwTimeStr(), *mappath,*savepath,*deltapath );
	unguard;

	
	RP_LOG( RP_NAME, TEXT("%s -- %s :: Connection->PackageMap [%p]"), RP_LOGP, Connection->PackageMap );
	{
		for( TArray<FPackageInfo>::TIterator it(Connection->PackageMap->List); it; ++it )
		{
			FPackageInfo* f = &(*it);
			if( cachename.Caps() == FString(f->Guid.String()).Caps() )
			{
				f->FileSize = GFileManager->FileSize(*savepath);
				RP_LOG( RP_NAME, TEXT("%s !! %s :: [%d] [%s]"), RP_LOGP, it.GetIndex(), *ToStr(f) );
			}
			else
			{
				RP_LOG( RP_NAME, TEXT("%s -- %s :: [%d] [%s]"), RP_LOGP, it.GetIndex(), *ToStr(f) );
			}
		}
	}

	
//	GRpEngine->PruneTravelActors();
//	GRpEngine->DeleteSaveGameFiles();
	//GRpEngine->GLevel->CleanupDestroyed(TRUE);

	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%d] [%s]"), RP_LOGP, *ToStr(Connection), PackageIndex, Error );
	unguard;
}


void RpUpdatePackageMap( int newsize )
{
	rpguard(RpUpdatePackageMap);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d]"), RP_LOGP, newsize );
	
	// ASSUME: first entry is the map
	GRpEngine->GLevel->NetDriver->MasterMap->List(0).FileSize = newsize;

	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d]"), RP_LOGP, newsize );
	unguard;
}



void RpDeleteTempMaps()
{
	rpguard(RpDeleteTempMaps);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	RpDeleteDirectoryContents(TEXT("..\\MapsTemp"));
	RpDeleteDirectoryContents(TEXT("..\\Cache"));
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	unguard;
}

// ============================================================================
//  EOF
// ============================================================================