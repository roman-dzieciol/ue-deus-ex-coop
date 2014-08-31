// ============================================================================
//	RpDetours.h
//  Copyright 2006 Roman Dzieciol. All Rights Reserved.
// ============================================================================
#pragma once

// ----------------------------------------------------------------------------
//	Forward declarations
// ----------------------------------------------------------------------------

// member/virtual detours
extern void RpDtFFileManager( FFileManager* p );
extern void RpDtFNetworkNotify( FNetworkNotify* p );
extern void RpDtUFileChannel( UFileChannel* p );
extern void RpDtULevel( ULevel* p );
extern void RpDtUEntryLevel( ULevel* p );
extern void RpDtUPendingLevel( UPendingLevel* p );
extern void RpDtUNetConnection( UNetConnection* p );
extern void RpDtUNetDriver( UNetDriver* p );

// static/global detours
extern void RpDtsDDeusExGameEngine();
extern void RpDtsExtWindow();
extern void RpDtsUNetConnection();
extern void RpDtsUObject();


// helper functions
extern void RpDtStatic();




// ============================================================================
//  EOF
// ============================================================================