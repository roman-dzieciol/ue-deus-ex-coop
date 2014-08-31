/*=============================================================================
	RpDt.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
	
// Includes.
#include "RpCoop.h"
#include "RpDtMacros.h"



RP_DETOUR_CLASS( XWindow, void, AddActorRef, ( class AActor *actor ) )
{
	if( actor )
	{
		RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(actor) );
		RP_DETOUR_STC( AddActorRef(actor) );
		RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(actor) );
	}
	else
	{
		RP_DETOUR_STC( AddActorRef(actor) );
	}
}}

RP_DETOUR_CLASS( XWindow, void, RemoveActorRef, ( class AActor *actor ) )
{
	if( actor )
	{
		RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(actor) );
		RP_DETOUR_STC( RemoveActorRef(actor) );
		RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(actor) );
	}
	else
	{
		RP_DETOUR_STC( RemoveActorRef(actor) );
	}
}}

RP_DETOUR_CLASS( XWindow, UBOOL, IsActorValid, ( class AActor *actor ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(actor) );
	UBOOL r = RP_DETOUR_STC( IsActorValid(actor) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s] [%d]"), RP_LOGP, *ToStr(actor), r );
	return r;
}}
	

void RpDtsExtWindow()
{
	rpguard(RpDtsExtWindow);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: "), RP_LOGP );
	//RP_DETOUR_MF(XWindow,AddActorRef);
	//RP_DETOUR_MF(XWindow,RemoveActorRef);
	//RP_DETOUR_MF(XWindow,IsActorValid);
	unguard;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/