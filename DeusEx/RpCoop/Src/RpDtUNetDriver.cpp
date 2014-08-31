/*=============================================================================
	RpDt.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"
#include "RpDtMacros.h"


RP_DETOUR_CLASS( UNetDriver, void, LowLevelDestroy, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( LowLevelDestroy() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UNetDriver, FString, LowLevelGetNetworkNumber, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	FString r = RP_DETOUR_TP( LowLevelGetNetworkNumber() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( UNetDriver, void, AssertValid, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( AssertValid() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UNetDriver, UBOOL, InitConnect, ( FNetworkNotify* InNotify, FURL& ConnectURL, FString& Error ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s] [%s]"), RP_LOGP, *ToStr(InNotify), *ConnectURL.String(), *Error );
	RpDtFNetworkNotify(InNotify);
	UBOOL r = RP_DETOUR_TP( InitConnect(InNotify,ConnectURL,Error) );

	if( GRpEngine->GPendingLevel && GRpEngine->GPendingLevel->GetDriver() )
	{
		RpDtUNetConnection(GRpEngine->GPendingLevel->GetDriver()->ServerConnection);
	}
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s] [%s]"), RP_LOGP, *ToStr(InNotify), *ConnectURL.String(), *Error );
	return r;
}}

RP_DETOUR_CLASS( UNetDriver, UBOOL, InitListen, ( FNetworkNotify* InNotify, FURL& ConnectURL, FString& Error ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s] [%s]"), RP_LOGP, *ToStr(InNotify), *ConnectURL.String(), *Error );
	RpDtFNetworkNotify(InNotify);
	UBOOL r = RP_DETOUR_TP( InitListen(InNotify,ConnectURL,Error) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s] [%s]"), RP_LOGP, *ToStr(InNotify), *ConnectURL.String(), *Error );
	return r;
}}

RP_DETOUR_CLASS( UNetDriver, void, TickFlush, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( TickFlush() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UNetDriver, void, TickDispatch, ( FLOAT DeltaTime ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( TickDispatch(DeltaTime) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UNetDriver, UBOOL, Exec, ( const TCHAR* Cmd, FOutputDevice& Ar ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	UBOOL r = RP_DETOUR_TP( Exec(Cmd,Ar) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( UNetDriver, void, NotifyActorDestroyed, ( AActor* Actor ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( NotifyActorDestroyed(Actor) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}


void RpDtUNetDriver( UNetDriver* p )
{
	rpguard(RpDtUNetDriver);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: [%s]"), RP_LOGP, *ToStr(p) );
	if( !p )
		return;

	PBYTE UNetDriver = PBYTE(p);
	RP_DETOUR_VR( UNetDriver, 0x00, 0x58, LowLevelDestroy );
	//RP_DETOUR_VR( UNetDriver, 0x00, 0x5c, LowLevelGetNetworkNumber );
	RP_DETOUR_VR( UNetDriver, 0x00, 0x60, AssertValid );
	RP_DETOUR_VR( UNetDriver, 0x00, 0x64, InitConnect );
	RP_DETOUR_VR( UNetDriver, 0x00, 0x68, InitListen );
	//RP_DETOUR_VR( UNetDriver, 0x00, 0x6c, TickFlush );
	//RP_DETOUR_VR( UNetDriver, 0x00, 0x70, TickDispatch );
	//RP_DETOUR_VR( UNetDriver, 0x00, 0x74, NotifyActorDestroyed );
	//RP_DETOUR_VR( UNetDriver, 0x??, 0x??, Exec );
	unguard;
}


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/