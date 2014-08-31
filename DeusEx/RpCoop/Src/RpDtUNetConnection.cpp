/*=============================================================================
	RpDt.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"
#include "RpDtMacros.h"





RP_DETOUR_CLASS( UNetConnection, FString, LowLevelGetRemoteAddress, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [rv]"), RP_LOGP );
	FString r = RP_DETOUR_TP( LowLevelGetRemoteAddress() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *r );
	return r;
}}

RP_DETOUR_CLASS( UNetConnection, FString, LowLevelDescribe, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [rv]"), RP_LOGP );
	FString r = RP_DETOUR_TP( LowLevelDescribe() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *r );
	return r;
}}

RP_DETOUR_CLASS( UNetConnection, void, LowLevelSend, ( void* Data, INT Count ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%p] [%d]"), RP_LOGP, Data, Count);
	RP_DETOUR_TP( LowLevelSend(Data,Count) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%p] [%d]"), RP_LOGP, Data, Count);
}}

RP_DETOUR_CLASS( UNetConnection, void, InitOut, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( InitOut() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UNetConnection, void, AssertValid, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( AssertValid() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UNetConnection, void, SendAck, ( INT PacketId, UBOOL FirstTime ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d] [%d]"), RP_LOGP, PacketId, FirstTime);
	RP_DETOUR_TP( SendAck(PacketId,FirstTime) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d] [%d]"), RP_LOGP, PacketId, FirstTime);
}}

RP_DETOUR_CLASS( UNetConnection, void, FlushNet, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( FlushNet() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UNetConnection, void, Tick, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( Tick() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UNetConnection, INT, IsNetReady, ( UBOOL Saturate ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d] [rv]"), RP_LOGP, Saturate);
	INT r = RP_DETOUR_TP( IsNetReady(Saturate) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d] [%d]"), RP_LOGP, Saturate, r );
	return r;
}}

RP_DETOUR_CLASS( UNetConnection, void, HandleClientPlayer, ( APlayerPawn* Pawn ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(Pawn) );
	RP_DETOUR_TP( HandleClientPlayer(Pawn) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(Pawn) );
}}

//
// member
//


RP_DETOUR_CLASS( UNetConnection, void, ReceiveFile, ( INT PackageIndex ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d]"), RP_LOGP, PackageIndex );
	RP_DETOUR_TP( ReceiveFile(PackageIndex) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d]"), RP_LOGP, PackageIndex );
}}


RP_DETOUR_CLASS( UNetConnection, UChannel*, CreateChannel, ( enum EChannelType Type, UBOOL bOpenedLocally, INT ChannelIndex ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%d],[%d],[%d]"), RP_LOGP, (int)Type, bOpenedLocally, ChannelIndex );
	UChannel* r = RP_DETOUR_TP( CreateChannel(Type,bOpenedLocally,ChannelIndex) );
	if( Type == 3 )
	{
		RpDtUFileChannel((UFileChannel*)r);

	}
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%d],[%d],[%d] [%s]"), RP_LOGP, (int)Type, bOpenedLocally, ChannelIndex, *ToStr(r) );
	return r;
}}


void RpDtUNetConnection( UNetConnection* p )
{
	rpguard(RpDtUNetConnection);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: [%s]"), RP_LOGP, *ToStr(p) );
	if( p == NULL )
		return;


	RP_DETOUR_MF(UNetConnection,ReceiveFile);

	PBYTE UNetConnection = PBYTE(p);
	//RP_DETOUR_VR( UNetConnection, 0x00, 0x58, LowLevelGetRemoteAddress );
	//RP_DETOUR_VR( UNetConnection, 0x00, 0x5c, LowLevelDescribe );
	//RP_DETOUR_VR( UNetConnection, 0x00, 0x60, LowLevelSend );
	//RP_DETOUR_VR( UNetConnection, 0x00, 0x64, InitOut );
	//RP_DETOUR_VR( UNetConnection, 0x00, 0x68, AssertValid );
	//RP_DETOUR_VR( UNetConnection, 0x00, 0x6c, SendAck );
	//RP_DETOUR_VR( UNetConnection, 0x00, 0x70, FlushNet );
	//RP_DETOUR_VR( UNetConnection, 0x00, 0x74, Tick );
	//RP_DETOUR_VR( UNetConnection, 0x00, 0x78, IsNetReady );
	RP_DETOUR_VR( UNetConnection, 0x00, 0x7c, HandleClientPlayer );
	unguard;
	
}

void RpDtsUNetConnection()
{
	rpguard(RpDtsUNetConnection);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: "), RP_LOGP );
	//RP_DETOUR_MF(UNetConnection,CreateChannel);
	unguard;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
