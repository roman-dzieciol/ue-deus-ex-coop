// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upTypes.h
// ============================================================================
#ifndef UPTYPES_H
#define UPTYPES_H


// ============================================================================
// POD
// ============================================================================
typedef unsigned __int8		byte;
typedef unsigned __int16	word;
typedef unsigned __int32	dword;
typedef unsigned __int64	qword;

typedef signed __int8		int8;
typedef signed __int16		int16;
typedef signed __int32		int32;
typedef signed __int64		int64;

typedef unsigned int		uint;
typedef const wxChar*		cstr;


// ============================================================================
// Hash maps
// ============================================================================
WX_DECLARE_HASH_MAP( wxString,	wxString,	wxStringHash,	wxStringEqual,	upStrStrMap );
WX_DECLARE_HASH_MAP( wxString,	dword,		wxStringHash,	wxStringEqual,	upStrDwordMap );
WX_DECLARE_HASH_MAP( wxString,	int,		wxStringHash,	wxStringEqual,	upStrIntMap );
WX_DECLARE_HASH_MAP( wxString,	void*,		wxStringHash,	wxStringEqual,	upStrPtrMap );
WX_DECLARE_HASH_MAP( int,		wxString,	wxIntegerHash,	wxIntegerEqual,	upIntStrMap );
WX_DECLARE_HASH_MAP( int,		int,		wxIntegerHash,	wxIntegerEqual,	upIntIntMap );
WX_DECLARE_HASH_MAP( int,		dword,		wxIntegerHash,	wxIntegerEqual,	upIntDwordMap );
WX_DECLARE_HASH_MAP( int,		qword,		wxIntegerHash,	wxIntegerEqual,	upIntQwordMap );
WX_DECLARE_HASH_MAP( void*,		wxString,	wxPointerHash,	wxPointerEqual,	upPtrStrMap );


#endif UPTYPES_H
// ============================================================================
//	The End.
// ============================================================================