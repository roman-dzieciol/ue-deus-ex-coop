// ============================================================================
//	Rp.h
//	Copyright 2007 Roman Dzieciol. All Rights Reserved.
// ============================================================================
#pragma once



extern FString RpGetDeltaPath(const TCHAR* Filename);
extern void RpOnMove( const TCHAR* Dest, const TCHAR* Src, UBOOL Replace, UBOOL EvenIfReadOnly, UBOOL Attributes );
extern void RpOnReceivedFile( UNetConnection* Connection, INT PackageIndex, const TCHAR* Error );
extern void RpUpdatePackageMap( int newsize );
extern void RpDeleteTempMaps();


// ============================================================================
//  EOF
// ============================================================================