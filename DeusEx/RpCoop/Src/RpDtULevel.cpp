/*=============================================================================
	RpDt.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"
#include "RpDtMacros.h"



/*-----------------------------------------------------------------------------
	ULevel
-----------------------------------------------------------------------------*/



RP_DETOUR_CLASS( ULevel, void, PostLoad, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );
	RP_DETOUR_TP( PostLoad() );
	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
}}

RP_DETOUR_CLASS( ULevel, void, Destroy, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );
	RP_DETOUR_TP( Destroy() );
	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
}}

RP_DETOUR_CLASS( ULevel, void, Tick, ( ELevelTick TickType, FLOAT DeltaSeconds ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	RP_DETOUR_TP( Tick(TickType,DeltaSeconds) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( ULevel, void, ReconcileActors, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );
	RP_DETOUR_TP( ReconcileActors() );
	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
}}

RP_DETOUR_CLASS( ULevel, void, RememberActors, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );
	RP_DETOUR_TP( RememberActors() );
	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
}}

RP_DETOUR_CLASS( ULevel, void, ShrinkLevel, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );
	RP_DETOUR_TP( ShrinkLevel() );
	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
}}

RP_DETOUR_CLASS( ULevel, void, CompactActors, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s ::"), RP_LOGP );
	RP_DETOUR_TP( CompactActors() );
	RP_LOG( RP_NAME, TEXT("%s << %s ::"), RP_LOGP );
}}

RP_DETOUR_CLASS( ULevel, UBOOL, Listen, ( FString& Error ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *Error );
	UBOOL r = RP_DETOUR_TP( Listen(Error) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] "), RP_LOGP, *Error );
	return r;
}}

RP_DETOUR_CLASS( ULevel, UBOOL, DestroyActor, ( AActor* Actor, UBOOL bNetForce ) )
{
	if( Cast<ADecal>(Actor) )
	{
		return RP_DETOUR_TP( DestroyActor(Actor,bNetForce) );
	}
	else
	{
		RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%d]"), RP_LOGP, *ToStr(Actor), bNetForce );
		UBOOL r = RP_DETOUR_TP( DestroyActor(Actor,bNetForce) );
		RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%d] [%d]"), RP_LOGP, *ToStr(Actor),bNetForce, r );
		return r;	
	}
}}

RP_DETOUR_CLASS( ULevel, void, CleanupDestroyed, ( UBOOL bForce ) )
{
	if( bForce )
	{
		RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d]"), RP_LOGP, bForce );
		RP_DETOUR_TP( CleanupDestroyed(bForce) );
		RP_LOG( RP_NAME, TEXT("%s << %s :: [%d]"), RP_LOGP, bForce );
	}
	else
	{
		RP_DETOUR_TP( CleanupDestroyed(bForce) );
	}
}}

RP_DETOUR_CLASS( ULevel, AActor*, SpawnActor, ( UClass* Class, FName InName, AActor* Owner, class APawn* Instigator, FVector Location, FRotator Rotation, AActor* Template, UBOOL bNoCollisionFail, UBOOL bRemoteOwned ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s] "), RP_LOGP, *SwGetName(Class),*ToStr(InName),*SwGetName(Owner),*SwGetName(Instigator),*ToStr(Location),*ToStr(Rotation),*SwGetName(Template),*ToStr(bNoCollisionFail),*ToStr(bRemoteOwned) );
	AActor* r = RP_DETOUR_TP( SpawnActor(Class,InName,Owner,Instigator,Location,Rotation,Template,bNoCollisionFail,bRemoteOwned) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s] [%s]"), RP_LOGP, *SwGetName(Class),*ToStr(InName),*SwGetName(Owner),*SwGetName(Instigator),*ToStr(Location),*ToStr(Rotation),*SwGetName(Template),*ToStr(bNoCollisionFail),*ToStr(bRemoteOwned), *ToStr(r) );
	return r;
}}
	
RP_DETOUR_CLASS( ULevel, void, SpawnViewActor, ( UViewport* Viewport ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(Viewport) );
	RP_DETOUR_TP( SpawnViewActor(Viewport) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(Viewport) );
}}

RP_DETOUR_CLASS( ULevel, APlayerPawn*, SpawnPlayActor, ( UPlayer* Viewport, ENetRole RemoteRole, const FURL& URL, FString& Error ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%s],[%s],[%s] "), RP_LOGP, *ToStr(Viewport),*ToStr(RemoteRole),*URL.String(),*Error );
	APlayerPawn* r = RP_DETOUR_TP( SpawnPlayActor(Viewport,RemoteRole,URL,Error) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%s],[%s],[%s] [%s] "), RP_LOGP, *ToStr(Viewport),*ToStr(RemoteRole),*URL.String(),*Error, *ToStr(r) );
	return r;
}}

RP_DETOUR_CLASS( ULevel, void, WelcomePlayer, ( UNetConnection* Connection, TCHAR* Optional ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%s] "), RP_LOGP, *ToStr(Connection), Optional );
	RP_DETOUR_TP( WelcomePlayer(Connection,Optional) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%s] "), RP_LOGP, *ToStr(Connection), Optional );
}}


void RpDtULevel( ULevel* p )
{
	rpguard(RpDtULevel);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: [%s]"), RP_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	PBYTE ULevel = PBYTE(p);

	RP_DETOUR_VR( ULevel, 0x00, 0x20, PostLoad );
	RP_DETOUR_VR( ULevel, 0x00, 0x24, Destroy );
	//RP_DETOUR_VR( ULevel, 0x00, 0x5C, Tick );
	RP_DETOUR_VR( ULevel, 0x00, 0x6c, ReconcileActors );
	RP_DETOUR_VR( ULevel, 0x00, 0x70, RememberActors );
	RP_DETOUR_VR( ULevel, 0x00, 0x78, ShrinkLevel );
	//RP_DETOUR_VR( ULevel, 0x00, 0x7c, CompactActors );
	RP_DETOUR_VR( ULevel, 0x00, 0x80, Listen );
	//RP_DETOUR_VR( ULevel, 0x00, 0x94, DestroyActor );
	RP_DETOUR_VR( ULevel, 0x00, 0x98, CleanupDestroyed );
	//RP_DETOUR_VR( ULevel, 0x00, 0x9c, SpawnActor );
	RP_DETOUR_VR( ULevel, 0x00, 0xa4, SpawnViewActor );
	RP_DETOUR_VR( ULevel, 0x00, 0xa8, SpawnPlayActor );
	RP_DETOUR_VR( ULevel, 0x00, 0xe4, WelcomePlayer );


	/*RP_DETOUR_VR( ULevel, 0x28, 0x00, NotifyAcceptingConnection );
	RP_DETOUR_VR( ULevel, 0x28, 0x04, NotifyAcceptedConnection );
	RP_DETOUR_VR( ULevel, 0x28, 0x08, NotifyAcceptingChannel );
	RP_DETOUR_VR( ULevel, 0x28, 0x0c, NotifyGetLevel );
	RP_DETOUR_VR( ULevel, 0x28, 0x10, NotifyReceivedText );
	RP_DETOUR_VR( ULevel, 0x28, 0x14, NotifySendingFile );
	RP_DETOUR_VR( ULevel, 0x28, 0x18, NotifyReceivedFile );
	//RP_DETOUR_VR( ULevel, 0x28, 0x1c, NotifyProgress );*/

	//RpDetoursNetDrv(GLevel->NetDriver);
	unguard;
}

	
void RpDtUEntryLevel( ULevel* p )
{
	rpguard(RpDtUEntryLevel);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: [%s]"), RP_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	PBYTE UEntryLevel = PBYTE(p);

	/*//RP_DETOUR_VR( UEntryLevel, 0x00, 0x5C, Tick );
	RP_DETOUR_VR( UEntryLevel, 0x00, 0x80, Listen );
	RP_DETOUR_VR( UEntryLevel, 0x00, 0xE4, WelcomePlayer );
	
	RP_DETOUR_VR( UEntryLevel, 0x28, 0x00, NotifyAcceptingConnection );
	RP_DETOUR_VR( UEntryLevel, 0x28, 0x04, NotifyAcceptedConnection );
	RP_DETOUR_VR( UEntryLevel, 0x28, 0x08, NotifyAcceptingChannel );
	RP_DETOUR_VR( UEntryLevel, 0x28, 0x0c, NotifyGetLevel );
	RP_DETOUR_VR( UEntryLevel, 0x28, 0x10, NotifyReceivedText );
	RP_DETOUR_VR( UEntryLevel, 0x28, 0x14, NotifySendingFile );
	RP_DETOUR_VR( UEntryLevel, 0x28, 0x18, NotifyReceivedFile );
	//RP_DETOUR_VR( UEntryLevel, 0x28, 0x1c, NotifyProgress );*/

	unguard;
}

void RpDtUPendingLevel( UPendingLevel* p )
{
	rpguard(RpDtUPendingLevel);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: [%s]"), RP_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	/*PBYTE UNetPendingLevel = PBYTE(p);

	// Tested with UNetPendingLevel only!
	RP_DETOUR_VR( UNetPendingLevel, 0x28, 0x00, NotifyAcceptingConnection );
	RP_DETOUR_VR( UNetPendingLevel, 0x28, 0x04, NotifyAcceptedConnection );
	RP_DETOUR_VR( UNetPendingLevel, 0x28, 0x08, NotifyAcceptingChannel );
	RP_DETOUR_VR( UNetPendingLevel, 0x28, 0x0c, NotifyGetLevel );
	RP_DETOUR_VR( UNetPendingLevel, 0x28, 0x10, NotifyReceivedText );
	RP_DETOUR_VR( UNetPendingLevel, 0x28, 0x14, NotifySendingFile );
	RP_DETOUR_VR( UNetPendingLevel, 0x28, 0x18, NotifyReceivedFile );
	RP_DETOUR_VR( UNetPendingLevel, 0x28, 0x1c, NotifyProgress );*/
	unguard;
}


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/