/*=============================================================================
	RpDt.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"
#include "RpDtMacros.h"





/*-----------------------------------------------------------------------------
	FNetworkNotify
-----------------------------------------------------------------------------*/


RP_DETOUR_CLASS( FNetworkNotify, EAcceptConnection, NotifyAcceptingConnection, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [rv]"), RP_LOGP );
	EAcceptConnection r = RP_DETOUR_TP( NotifyAcceptingConnection() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d]"), RP_LOGP, (int)r );
	return r;
}}

RP_DETOUR_CLASS( FNetworkNotify, void, NotifyAcceptedConnection, (UNetConnection* Connection) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(Connection) );
	RpDtUNetConnection(Connection);
	RP_DETOUR_TP( NotifyAcceptedConnection(Connection) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(Connection) );
}}

RP_DETOUR_CLASS( FNetworkNotify, UBOOL, NotifyAcceptingChannel, ( class UChannel* Channel ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [rv]"), RP_LOGP, *ToStr(Channel) );
	UBOOL r = RP_DETOUR_TP( NotifyAcceptingChannel(Channel) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%d]"), RP_LOGP, *ToStr(Channel), r );
	return r;
}}

RP_DETOUR_CLASS( FNetworkNotify, ULevel*, NotifyGetLevel, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [rv]"), RP_LOGP );
	ULevel* r = RP_DETOUR_TP( NotifyGetLevel() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%p]"), RP_LOGP, r );
	return r;
}}

RP_DETOUR_CLASS( FNetworkNotify, void, NotifyReceivedText, ( UNetConnection* Connection, const TCHAR* Text ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s]"), RP_LOGP, *ToStr(Connection,true), Text );
	RP_DETOUR_TP( NotifyReceivedText(Connection,Text) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s]"), RP_LOGP, *ToStr(Connection,true), Text );
}}

RP_DETOUR_CLASS( FNetworkNotify, UBOOL, NotifySendingFile, ( UNetConnection* Connection, FGuid GUID ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s] [rv]"), RP_LOGP, *ToStr(Connection), GUID.String() );

	UBOOL r = RP_DETOUR_TP( NotifySendingFile(Connection,GUID) );

	GRpFileOverride = true;

	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s] [%d]"), RP_LOGP, *ToStr(Connection), GUID.String(), r );
	return r;
}}



RP_DETOUR_CLASS( FNetworkNotify, void, NotifyReceivedFile, ( UNetConnection* Connection, INT PackageIndex, const TCHAR* Error ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%d] [%s]"), RP_LOGP, *ToStr(Connection), PackageIndex, Error );
	
	if( GRpCacheDest && GRpCacheSrc )
	{
		RpOnReceivedFile(Connection,PackageIndex,Error);
		GRpCacheDest.Empty();
		GRpCacheSrc.Empty();
	}

	RP_DETOUR_TP( NotifyReceivedFile(Connection,PackageIndex,Error) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%d] [%s]"), RP_LOGP, *ToStr(Connection), PackageIndex, Error );
}}

RP_DETOUR_CLASS( FNetworkNotify, void, NotifyProgress, ( const TCHAR* Str1, const TCHAR* Str2, FLOAT Seconds ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s] [%f]"), RP_LOGP, Str1, Str2, Seconds );
	RP_DETOUR_TP( NotifyProgress(Str1,Str2,Seconds) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s] [%f]"), RP_LOGP, Str1, Str2, Seconds );
}}



/*-----------------------------------------------------------------------------
	ULevel
-----------------------------------------------------------------------------*/
/*
RP_DETOUR_CLASS( ULevel, EAcceptConnection, NotifyAcceptingConnection, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	EAcceptConnection r = RP_DETOUR_TP( NotifyAcceptingConnection() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( ULevel, void, NotifyAcceptedConnection, (UNetConnection* Connection) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	RP_DETOUR_TP( NotifyAcceptedConnection(Connection) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( ULevel, UBOOL, NotifyAcceptingChannel, ( class UChannel* Channel ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(Channel) );

	if( Channel->ChType == CHTYPE_File )
	{
		UFileChannel* fc = (UFileChannel*)Channel;
	}

	UBOOL r = RP_DETOUR_TP( NotifyAcceptingChannel(Channel) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(Channel) );
	return r;
}}

RP_DETOUR_CLASS( ULevel, ULevel*, NotifyGetLevel, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	ULevel* r = RP_DETOUR_TP( NotifyGetLevel() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;

}}

RP_DETOUR_CLASS( ULevel, void, NotifyReceivedText, ( UNetConnection* Connection, const TCHAR* Text ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, Text );
	RP_DETOUR_TP( NotifyReceivedText(Connection,Text) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, Text );
}}

RP_DETOUR_CLASS( ULevel, UBOOL, NotifySendingFile, ( UNetConnection* Connection, FGuid GUID ) )
{
	FString newfile = TEXT("test.123");

	for( TArray<UChannel*>::TIterator it=Connection->OpenChannels; it; ++it )
	{
		UChannel* c = *it;
		if( c->ChType == CHTYPE_File )
		{
			UFileChannel* fc = (UFileChannel*)c;
			appStrncpy( fc->Filename, *newfile, 255 );
			break;
		}
		//s += FString::Printf(TEXT(" | %s"),*ToStr(c));
	}RpDtUChannel

	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s]"), RP_LOGP, *ToStr(Connection), GUID.String() );
	UBOOL r = RP_DETOUR_TP( NotifySendingFile(Connection,GUID) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s]"), RP_LOGP, *ToStr(Connection), GUID.String() );
	return r;
}}

RP_DETOUR_CLASS( ULevel, void, NotifyReceivedFile, ( UNetConnection* Connection, INT PackageIndex, const TCHAR* Error ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d] [%s]"), RP_LOGP, PackageIndex, Error );
	RP_DETOUR_TP( NotifyReceivedFile(Connection,PackageIndex,Error) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d] [%s]"), RP_LOGP, PackageIndex, Error );
	
	
}}

RP_DETOUR_CLASS( ULevel, void, NotifyProgress, ( const TCHAR* Str1, const TCHAR* Str2, FLOAT Seconds ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s] [%f]"), RP_LOGP, Str1, Str2, Seconds );
	RP_DETOUR_TP( NotifyProgress(Str1,Str2,Seconds) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s] [%f]"), RP_LOGP, Str1, Str2, Seconds );
}}

*/
/*-----------------------------------------------------------------------------
	UNetPendingLevel
-----------------------------------------------------------------------------*/
/*

RP_DETOUR_CLASS( UNetPendingLevel, EAcceptConnection, NotifyAcceptingConnection, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	EAcceptConnection r = RP_DETOUR_TP( NotifyAcceptingConnection() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( UNetPendingLevel, void, NotifyAcceptedConnection, (UNetConnection* Connection) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	RP_DETOUR_TP( NotifyAcceptedConnection(Connection) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UNetPendingLevel, UBOOL, NotifyAcceptingChannel, ( class UChannel* Channel ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(Channel) );
	UBOOL r = RP_DETOUR_TP( NotifyAcceptingChannel(Channel) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(Channel) );
	return r;
}}

RP_DETOUR_CLASS( UNetPendingLevel, ULevel*, NotifyGetLevel, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP );
	ULevel* r = RP_DETOUR_TP( NotifyGetLevel() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( UNetPendingLevel, void, NotifyReceivedText, ( UNetConnection* Connection, const TCHAR* Text ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, Text );
	RP_DETOUR_TP( NotifyReceivedText(Connection,Text) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, Text );
}}

RP_DETOUR_CLASS( UNetPendingLevel, UBOOL, NotifySendingFile, ( UNetConnection* Connection, FGuid GUID ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s]"), RP_LOGP, *ToStr(Connection), GUID.String() );
	UBOOL r = RP_DETOUR_TP( NotifySendingFile(Connection,GUID) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s]"), RP_LOGP, *ToStr(Connection), GUID.String() );
	return r;
}}

RP_DETOUR_CLASS( UNetPendingLevel, void, NotifyReceivedFile, ( UNetConnection* Connection, INT PackageIndex, const TCHAR* Error ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d] [%s]"), RP_LOGP, PackageIndex, Error );
	RP_DETOUR_TP( NotifyReceivedFile(Connection,PackageIndex,Error) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d] [%s]"), RP_LOGP, PackageIndex, Error );
}}

RP_DETOUR_CLASS( UNetPendingLevel, void, NotifyProgress, ( const TCHAR* Str1, const TCHAR* Str2, FLOAT Seconds ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s] [%s] [%f]"), RP_LOGP, Str1, Str2, Seconds );
	RP_DETOUR_TP( NotifyProgress(Str1,Str2,Seconds) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%s] [%f]"), RP_LOGP, Str1, Str2, Seconds );
}}*/


void RpDtFNetworkNotify( FNetworkNotify* p )
{
	rpguard(RpDtFNetworkNotify);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: [%s]"), RP_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	PBYTE FNetworkNotify = PBYTE(p);

	RP_DETOUR_VR( FNetworkNotify, 0x00, 0x00, NotifyAcceptingConnection );
	RP_DETOUR_VR( FNetworkNotify, 0x00, 0x04, NotifyAcceptedConnection );
	RP_DETOUR_VR( FNetworkNotify, 0x00, 0x08, NotifyAcceptingChannel );
	RP_DETOUR_VR( FNetworkNotify, 0x00, 0x0c, NotifyGetLevel );
	//RP_DETOUR_VR( FNetworkNotify, 0x00, 0x10, NotifyReceivedText );
	RP_DETOUR_VR( FNetworkNotify, 0x00, 0x14, NotifySendingFile );
	RP_DETOUR_VR( FNetworkNotify, 0x00, 0x18, NotifyReceivedFile );
	//RP_DETOUR_VR( FNetworkNotify, 0x28, 0x1c, NotifyProgress );
	unguard;
}



/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/