/*=============================================================================
	RpDt.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"
#include "RpDtMacros.h"


RP_DETOUR_CLASS( DDeusExGameEngine, void, DeleteGame, (INT Position ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	RP_DETOUR_TP( DeleteGame(Position) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( DDeusExGameEngine, UDeusExSaveInfo*, GetSaveInfo, (UPackage *saveInfoPackage) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	UDeusExSaveInfo* r = RP_DETOUR_TP( GetSaveInfo(saveInfoPackage) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( DDeusExGameEngine, void, CopySaveGameFiles, (FString &source, FString &dest) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	RP_DETOUR_TP( CopySaveGameFiles(source,dest) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( DDeusExGameEngine, void, DeleteSaveGameFiles, (FString saveDirectory ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	RP_DETOUR_TP( DeleteSaveGameFiles(saveDirectory) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( DDeusExGameEngine, UDeusExSaveInfo*, LoadSaveInfo, (int DirectoryIndex ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	UDeusExSaveInfo* r = RP_DETOUR_TP( LoadSaveInfo(DirectoryIndex) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( DDeusExGameEngine, void, PruneTravelActors, (void) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	RP_DETOUR_TP( PruneTravelActors() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( DDeusExGameEngine, void, SaveCurrentLevel, (int DirectoryIndex, bool bSavePlayer) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	RP_DETOUR_TP( SaveCurrentLevel(DirectoryIndex,bSavePlayer) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( DDeusExGameEngine, INT, GetCurrentMissionNumber, (void) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	INT r = RP_DETOUR_TP( GetCurrentMissionNumber() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( DDeusExGameEngine, INT, GetNextMissionNumber, (FString &mapName) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	INT r = RP_DETOUR_TP( GetNextMissionNumber(mapName) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( DDeusExGameEngine, ADeusExLevelInfo*, GetDeusExLevelInfo, (void) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	ADeusExLevelInfo* r = RP_DETOUR_TP( GetDeusExLevelInfo() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

void RpDtsDDeusExGameEngine()
{
	rpguard(RpDtsDDeusExGameEngine);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: "), RP_LOGP );
	RP_DETOUR_MF(DDeusExGameEngine,DeleteGame);
	RP_DETOUR_MF(DDeusExGameEngine,GetSaveInfo);
	RP_DETOUR_MF(DDeusExGameEngine,CopySaveGameFiles);
	RP_DETOUR_MF(DDeusExGameEngine,DeleteSaveGameFiles);
	RP_DETOUR_MF(DDeusExGameEngine,LoadSaveInfo);
	RP_DETOUR_MF(DDeusExGameEngine,PruneTravelActors);
	RP_DETOUR_MF(DDeusExGameEngine,SaveCurrentLevel);
	RP_DETOUR_MF(DDeusExGameEngine,GetCurrentMissionNumber);
	RP_DETOUR_MF(DDeusExGameEngine,GetNextMissionNumber);
	RP_DETOUR_MF(DDeusExGameEngine,GetDeusExLevelInfo);
	unguard;
}


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/