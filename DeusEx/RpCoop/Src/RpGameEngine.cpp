/*=============================================================================
	RpCoop.cpp
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"


/*=============================================================================
	URpGameEngine ::
=============================================================================*/

URpGameEngine::URpGameEngine() : DDeusExGameEngine()
{
	RegisterRpCoopNames();
	GRpFileOverride = FALSE;
	GRpCacheSrc.Empty();
	GRpCacheDest.Empty();
}

/*void URpGameEngine::CoopTravel(const TCHAR* Cmd)
{
	rpguard(URpGameEngine::CoopTravel);
	RP_LOG( RP_NAME, TEXT("%s >> CoopTravel() :: "), RP_LOGP );

	// get URL
	ParseNext(&Cmd);
	FURL curl(Cmd);
	RP_LOG( RP_NAME, TEXT("[%s]"), *curl.String(0) );
	SAVE(1,true);
	
	RP_LOG( RP_NAME, TEXT("%s << CoopTravel() :: "), RP_LOGP );
	unguard;
}
void URpGameEngine::SAVE(int DirectoryIndex, bool bSavePlayer)
{
	rpguard(URpGameEngine::SAVE);
	FString S;
	S = FString::Printf(TEXT("DX.dxs"));
	GLevel->CleanupDestroyed(true);
	SavePackage( GLevel->GetOuter(), GLevel, 0x00400000, *S, GWarn );
	unguard;
}*/


/*-----------------------------------------------------------------------------
	URpGameEngine :: UObject interface.
-----------------------------------------------------------------------------*/

void URpGameEngine::Destroy(void)
{
	rpguard(URpGameEngine::Destroy);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );

	DDeusExGameEngine::Destroy();

	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	unguard;
}


/*-----------------------------------------------------------------------------
	URpGameEngine :: UEngine interface.
-----------------------------------------------------------------------------*/

void URpGameEngine::Init(void)
{
	rpguard(URpGameEngine::Init);

	RegisterRpCoopNames();
	GRpEngine = this;

	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );

	RpDtStatic();
	RpDtFFileManager(GFileManager);

	DDeusExGameEngine::Init();

	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	unguard;
}

void URpGameEngine::Exit()
{
	rpguard(URpGameEngine::Exit);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );

	// TODO:: find out when is this called
	DDeusExGameEngine::Exit();

	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	unguard;
}

UBOOL URpGameEngine::Exec(const TCHAR* Cmd, FOutputDevice& Ar)
{
	rpguard(URpGameEngine::Exec);
	//RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, Cmd );

	if( DDeusExGameEngine::Exec(Cmd,Ar) )
	{
	}
	else if( ParseCommand(&Cmd,TEXT("lbe")) )
	{
		GLineBatcher.Empty();
	}
	else if( ParseCommand(&Cmd,TEXT("lbt")) )
	{
		GLineBatcher.Toggle();
	}
	else if( ParseCommand(&Cmd,TEXT("dmr")) )
	{
		RpDumpRefs();
	}
	else if( ParseCommand(&Cmd,TEXT("chans")) )
	{
		GLog->Logf( RP_NAME, TEXT("%s >> Channels"), *SwTimeStr() );
		Ar.Log( TEXT("Active channels:") );
		
		TMap<UClass*,int> ClassCount;
		
		if( GLevel && GLevel->NetDriver && GLevel->NetDriver->ServerConnection )
		{
			UNetConnection* np = GLevel->NetDriver->ServerConnection;

			for( TArray<UChannel*>::TIterator it(np->OpenChannels); it; ++it )
			{
				UChannel* cp = *it;
				if( !cp )
					continue;

				FString s;

				switch( cp->ChType )
				{
					case CHTYPE_Control:	s += TEXT("Control"); break;
					case CHTYPE_Actor:		s += TEXT("Actor"); break;
					case CHTYPE_File:		s += TEXT("File"); break;
					default:				s += TEXT("Unknown"); break;
				}

				s += RP_SEP + FString::Printf(TEXT("%d"), cp->OpenedLocally);
				s += FString::Printf(TEXT("%d"), cp->OpenTemporary);
				
				switch( cp->ChType )
				{
					case CHTYPE_Control:
					{
						UControlChannel* sp = (UControlChannel*)cp;
						break;
					}

					case CHTYPE_Actor:
					{
						UActorChannel* sp = (UActorChannel*)cp;
						//s += RP_SEP + FString::Printf(TEXT("%s"), *SwGetName(sp->ActorClass));
						s += RP_SEP + FString::Printf(TEXT("%s"), *SwGetName(sp->Actor));
						s += RP_SEP + FString::Printf(TEXT("%.02lf"), sp->RelevantTime);
						s += RP_SEP + FString::Printf(TEXT("%.02lf"), sp->LastUpdateTime);
						s += RP_SEP + FString::Printf(TEXT("%d"), sp->SpawnAcked);

						int* count = ClassCount.Find(sp->ActorClass);
						if( count )
						{
							++*count;
						}
						else
						{
							ClassCount.Set(sp->ActorClass, 1);
						}
						break;
					}

					case CHTYPE_File:
					{
						UFileChannel* sp = (UFileChannel*)cp;
						s += RP_SEP + FString::Printf(TEXT("%s"), sp->Filename);
						s += RP_SEP + FString::Printf(TEXT("%s"), sp->PrettyName);
						s += RP_SEP + FString::Printf(TEXT("%s"), sp->Error);
						s += RP_SEP + FString::Printf(TEXT("%d"), sp->Transfered);
						s += RP_SEP + FString::Printf(TEXT("%d"), sp->PackageIndex);
						break;
					}

					default:
					{
						break;
					}
				}
				GLog->Logf( RP_NAME, TEXT("%s -- Channels :: [%s]"), *SwTimeStr(), *s );
				Ar.Log( s );
			}

			for( TMap<UClass*,int>::TIterator ci(ClassCount); ci; ++ci )
			{
				GLog->Logf( RP_NAME, TEXT("%s -- Channels :: [%d]\t[%s]"), *SwTimeStr(), ci.Value(), *SwGetName(ci.Key()) );
				Ar.Logf( TEXT("    %d %s"), ci.Value(), *SwGetName(ci.Key()) );
			}

			GLog->Logf( RP_NAME, TEXT("%s << Channels"), *SwTimeStr() );
		}
		RpDumpRefs();
	}
	else
	{
		//RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, Cmd );
		return 0;
	}

	//RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, Cmd );
	return 1;
	unguard;
}

/*void URpGameEngine::Tick( FLOAT DeltaSeconds )
{
	rpguard(URpGameEngine::Tick);
	//RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );

	DDeusExGameEngine::Tick(DeltaSeconds);

	//RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	unguard;
}*/


void URpGameEngine::SetClientTravel( UPlayer* Viewport, const TCHAR* NextURL, UBOOL bItems, ETravelType TravelType )
{
	rpguard(URpGameEngine::SetClientTravel);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s] [%d] [%s]"), RP_LOGP, *ToStr(Viewport), NextURL, bItems, *ToStr(TravelType) );

	DDeusExGameEngine::SetClientTravel(Viewport,NextURL,bItems,TravelType);

	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s] [%d] [%s]"), RP_LOGP, *ToStr(Viewport), NextURL, bItems, *ToStr(TravelType) );
	unguard;
}

INT URpGameEngine::ChallengeResponse( INT Challenge )
{
	rpguard(URpGameEngine::ChallengeResponse);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d [rv]]"), RP_LOGP, Challenge );

	INT r = DDeusExGameEngine::ChallengeResponse(Challenge);

	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d] [%d]"), RP_LOGP, Challenge, r );
	return r;
	unguard;
}


/*-----------------------------------------------------------------------------
	URpGameEngine :: UGameEngine interface.
-----------------------------------------------------------------------------*/

UBOOL URpGameEngine::Browse( FURL URL, const TMap<FString,FString>* TravelInfo, FString& Error )
{
	rpguard(URpGameEngine:Browse);

	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s]"), RP_LOGP, *URL.String(), *Error );

	RpDumpTravelInfo(TravelInfo);
	//RpDumpLevels();

	// Fix localhost ip
	if( URL.Host == FString(TEXT("0.0.0.0")))
	{
		RP_LOG( RP_NAME, TEXT("%s ## %s :: Bypassing 0.0.0.0 host."), RP_LOGP );
		URL.Host.Empty();
	}
	
	RpDeleteDirectoryContents(TEXT("..\\MapsTemp"));

	RpDtUEntryLevel(GEntry);
	RpDtULevel(GLevel);

	/*if( !URL.HasOption(TEXT("listen")) )
	{
		PruneTravelActors();
		//GLevel->CleanupDestroyed(TRUE);
	}*/

	UBOOL bResult = DDeusExGameEngine::Browse(URL,TravelInfo,Error);
	RpDtUEntryLevel(GEntry);
	RpDtULevel(GLevel);
	RpDtUPendingLevel(GPendingLevel);

	RpDumpTravelInfo(TravelInfo);
	//RpDumpLevels();
	
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s]"), RP_LOGP, *URL.String(), *Error );
	return bResult;
	unguard;
}


int URpGameEngine::RpLoadListenMap( const FURL& URL, UPendingLevel* Pending, const TMap<FString,FString>* TravelInfo, FString& Error )
{
	rpguard(RpLoadListenMap);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] "), RP_LOGP, *URL.String() );

	// Clear temp folder
	RpDeleteTempMaps();

	// Explode URL
	FString url = URL.Map;
	FString urlpath;
	FString urlfile;
	FString urlname;
	FString urlext;
	FString urlfull;

	if( !url.Split(TEXT("\\"),&urlpath,&urlfile,TRUE) )
	{
		urlfile = url;
	}

	if( !urlfile.Split(TEXT("."),&urlname,&urlext,TRUE) )
	{
		urlname = urlfile;
	}

	// Create paths
	FString temppath = FString::Printf(TEXT("..\\MapsTemp\\%s.dx"),*urlname);
	FString mappath = FString::Printf(TEXT("..\\MapsDB\\%s.dx"),*urlname);
	FString savepath = FString::Printf(TEXT("..\\Save\\Current\\%s.dxs"),*urlname);
	FString deltapath = FString::Printf(TEXT("..\\MapsTemp\\%s.dxh"),*urlname);

	// original map must exist
	if( GFileManager->FileSize(*mappath) == -1 )
		appThrowf(TEXT("File not found [%s]"),*mappath);

	if( url.Caps() == savepath.Caps() )
	{
		urlfull = url;

		rpguard(RpPack);
		RP_LOG( RP_NAME, TEXT("%s >> RpPack :: [%s] [%s] [%s]"), *SwTimeStr(), *mappath,*savepath,*deltapath );
		RpPack(*mappath,*savepath,*deltapath);
		RP_LOG( RP_NAME, TEXT("%s << RpPack :: [%s] [%s] [%s]"), *SwTimeStr(), *mappath,*savepath,*deltapath );
		unguard;

	}
	else
	{
		// Copy real map
		GFileManager->Copy( *temppath, *mappath, TRUE, TRUE );
		urlfull = temppath;
		
		rpguard(RpPack);
		RP_LOG( RP_NAME, TEXT("%s >> RpPack :: [%s] [%s] [%s]"), *SwTimeStr(), *mappath,*mappath,*deltapath );
		RpPack(*mappath,*mappath,*deltapath);
		RP_LOG( RP_NAME, TEXT("%s << RpPack :: [%s] [%s] [%s]"), *SwTimeStr(), *mappath,*mappath,*deltapath );
		unguard;
	}
	
	/*for( TArray<FPackageInfo>::TIterator it(GLevel->NetDriver->MasterMap->List); it; ++it )
	{
		FPackageInfo* f = &(*it);
		if( urlfull == f->URL )
		{
			f->FileSize = GFileManager->FileSize(*deltapath);
			RP_LOG( RP_NAME, TEXT("%s -- %s :: [%d] [%s]"), RP_LOGP, it.GetIndex(), *ToStr(f) );
		}
	}*/


	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *URL.String() );
	return GFileManager->FileSize(*deltapath);;
	unguard;
}



ULevel* URpGameEngine::LoadMap( const FURL& URL, UPendingLevel* Pending, const TMap<FString,FString>* TravelInfo, FString& Error )
{
	rpguard(URpGameEngine::LoadMap);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s] [%s]"), RP_LOGP, *URL.String(), *Error, *URL.Map );

	RpDtUEntryLevel(GEntry);
	RpDtULevel(GLevel);
	ULevel* r;

	// if listen and real map, do some preprocessing
	if( URL.HasOption(TEXT("listen")) /*&& URL.Map.Mid(2,1) == TEXT("_")*/ )
	{
		int realsize = RpLoadListenMap(URL,Pending,TravelInfo,Error);
		r = DDeusExGameEngine::LoadMap(URL,Pending,TravelInfo,Error);
		RpUpdatePackageMap(realsize);
	}
	else
	{
		r = DDeusExGameEngine::LoadMap(URL,Pending,TravelInfo,Error);
	}

	RpDtUEntryLevel(GEntry);
	RpDtULevel(GLevel);
	RpDeleteDuplicates(GLevel);

	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s]"), RP_LOGP, *URL.String(), *Error );
	return r;
	unguard;
}

void URpGameEngine::SaveGame( INT Position )
{
	rpguard(URpGameEngine::SaveGame);
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d]"), RP_LOGP, Position );

	DDeusExGameEngine::SaveGame(Position);

	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d]"), RP_LOGP, Position );
	unguard;
}

void URpGameEngine::CancelPending()
{
	rpguard(URpGameEngine::CancelPending);
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );

	DDeusExGameEngine::CancelPending();

	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
	unguard;
}

/*void URpGameEngine::PaintProgress()
{
	rpguard(URpGameEngine::PaintProgress);
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );

	DDeusExGameEngine::PaintProgress();

	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
	unguard;
}*/

/*void URpGameEngine::UpdateConnectingMessage()
{
	rpguard(URpGameEngine::UpdateConnectingMessage);
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );

	DDeusExGameEngine::UpdateConnectingMessage();

	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
	unguard;
}*/

void URpGameEngine::BuildServerMasterMap( UNetDriver* NetDriver, ULevel* InLevel )
{
	rpguard(URpGameEngine::BuildServerMasterMap);
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );

	DDeusExGameEngine::BuildServerMasterMap(NetDriver,InLevel);

	// Dump master map
	RP_LOG( RP_NAME, TEXT("%s -- %s :: NetDriver->MasterMap [%p]"), RP_LOGP, NetDriver->MasterMap );
	for( TArray<FPackageInfo>::TIterator it(NetDriver->MasterMap->List); it; ++it )
	{
		FPackageInfo* f = &(*it);
		RP_LOG( RP_NAME, TEXT("%s -- %s :: [%d] [%s]"), RP_LOGP, it.GetIndex(), *ToStr(f) );
	}

	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
	unguard;
}

void URpGameEngine::NotifyLevelChange()
{
	rpguard(URpGameEngine::NotifyLevelChange);
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );

	DDeusExGameEngine::NotifyLevelChange();

	// Destroy leftover junk
	// TODO: find out why engine isn't doing this automatically !!!!!!!!!!!!!!!
	RP_LOG( RP_NAME, TEXT("%s -- %s :: GLevel=%s"), RP_LOGP, *ToStr(GLevel) );
	RP_LOG( RP_NAME, TEXT("%s -- %s :: GEntry=%s"), RP_LOGP, *ToStr(GEntry) );
	for( TObjectIterator<ULevel> oi; oi; ++oi )
	{
		PruneTravelActors();
		ULevel* op = *oi;
		if( op != GLevel && op != GEntry )
		{
			RP_LOG( RP_NAME, TEXT("%s !! %s :: Stray Level Found: [%s]"), RP_LOGP, *ToStr(op) );
			
			// Destroy the level, packages should be unloaded automatically
			op->Destroy();

			// Delete unused packages
			RpDeleteTempMaps();
		}
	}


	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
	unguard;
}
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
