// ============================================================================
//	Rp.cpp
//	Copyright 2007 Roman Dzieciol. All Rights Reserved.
// ============================================================================

// Includes.
#include "RpCoop.h"

void RpDeleteDuplicates( ULevel* level )
{
	rpguard(RpDeleteDuplicates);

	if( level->IsServer() )
		return;

	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(level) );

	// some saved actors arent automatically deleted when client starts
	// server makes client spawn replicated versions, but savegame ones stay
	// delete savegame ones manually

	for( TTransArray<AActor*>::TIterator ai(level->Actors); ai; ++ai )
	{
		AActor* ap = *ai;
		if( !ap )
			continue;

		if( ap->bStatic
		||	ap->bNoDelete
		||	ap->RemoteRole == ROLE_None
		||	ap->RemoteRole == ROLE_Authority
		)	continue;

		/*if( ap->IsA(AInfo::StaticClass()) )
		{
			if( !ap->IsA(AReplicationInfo::StaticClass())
			&&  !ap->IsA(ADeusExLevelInfo::StaticClass())
			)
				continue;
		}*/
		//||	ap->IsA(AInfo::StaticClass()) )
		//	continue;

		if( ap->IsA(ADeusExLevelInfo::StaticClass()) )
				continue;


		RP_LOG( RP_NAME, TEXT("%s -- %s :: [%s] [%d] [%d]"), RP_LOGP, *ToStr(ap), ap->bJustTeleported, ap->bTicked  );
		level->DestroyActor(ap);


	}

	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(level) );
	unguard;
}


// ============================================================================
//  EOF
// ============================================================================