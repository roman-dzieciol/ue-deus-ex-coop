// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upFile.h
// ============================================================================
#ifndef UPFILE_H
#define UPFILE_H

#include "upArchive.h"
#include "upFileBuffer.h"


// ============================================================================
// upFile
// ============================================================================
class upFile : public upArchive
{
protected:
	wxFFile			File;
	wxFileName		FileName;

public:
	explicit upFile( const wxFileName& filename );
	virtual ~upFile();

public:
	virtual wxString		DebugInfo	();
	virtual byte*			GetDataPtr	() = 0;

public:
	wxFFile&	GetFile		()	{ return File; }
    wxFileName	GetFileName	()	{ return FileName; }
    wxString	GetName		()	{ return FileName.GetName(); }
};


// ============================================================================
// upFileReader
// ============================================================================
class upFileReader : public upFile
{
protected:
	upFileReadBuffer	Buf;

public:
	explicit upFileReader( const wxFileName& filename );

public:
	virtual void			Serialize	( void* buffer, dword count ) { Buf.Read(buffer,count); }
	virtual byte*			GetDataPtr	() { return Buf.GetDataPtr(); }
	virtual void			DumpByteInfo();

public:
	virtual void			Seek		( wxFileOffset pos, wxSeekMode from = wxFromStart ) { Buf.Seek(pos,from); }
	virtual void			SeekEnd		( wxFileOffset pos = 0 ) { Buf.Seek(pos,wxFromEnd); }
	virtual wxFileOffset	Tell		() { return Buf.Tell(); }
	virtual wxFileOffset	Length		() { return Buf.Length(); }

public:
	virtual bool			IsOpened	() { return Buf.IsOpened(); }
	virtual bool			IsLoading	() { return true; }
	virtual bool			IsSaving	() { return false; }

public:
	virtual void			PushGuard	( wxFileOffset from, wxFileOffset to ) { Buf.PushGuard(from,to); }
	virtual void			PopGuard	() { Buf.PopGuard(); }
	virtual upFileGuard		GetGuard	() { return Buf.GetGuard(); }

public:
	virtual void			SetByteInfo	( EByteInfo info ) { Buf.SetByteInfo(info); }
	virtual EByteInfo		GetByteInfo	() { return Buf.GetByteInfo(); }
};


// ============================================================================
// upFileWriter
// ============================================================================
class upFileWriter : public upFile
{
protected:
	upFileWriteBuffer	Buf;

public:
	explicit upFileWriter( const wxFileName& filename );
	explicit upFileWriter( const wxFileName& filename, dword maxsize );
	virtual ~upFileWriter();

public:
	virtual void			Serialize	( void* buffer, dword count ) { Buf.Write(buffer,count); }
	virtual byte*			GetDataPtr	() { return Buf.GetDataPtr(); }
	virtual void			DumpByteInfo() {}

public:
	virtual void			Seek		( wxFileOffset pos, wxSeekMode from = wxFromStart ) { Buf.Seek(pos,from); }
	virtual void			SeekEnd		( wxFileOffset pos = 0 ) { Buf.Seek(pos,wxFromEnd); }
	virtual wxFileOffset	Tell		() { return Buf.Tell(); }
	virtual wxFileOffset	Length		() { return Buf.Length(); }

public:
	virtual bool			IsOpened	() { return Buf.IsOpened(); }
	virtual bool			IsLoading	() { return false; }
	virtual bool			IsSaving	() { return true; }

public:
	virtual void			PushGuard	( wxFileOffset from, wxFileOffset to ) { Buf.PushGuard(from,to); }
	virtual void			PopGuard	() { Buf.PopGuard(); }
	virtual upFileGuard		GetGuard	() { return Buf.GetGuard(); }

public:
	virtual void			SetByteInfo	( EByteInfo info ) { Buf.SetByteInfo(info); }
	virtual EByteInfo		GetByteInfo	() { return Buf.GetByteInfo(); }
};


#endif UPFILE_H
// ============================================================================
//	The End.
// ============================================================================