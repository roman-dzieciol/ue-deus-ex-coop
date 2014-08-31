// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upArchive.h
// ============================================================================
#ifndef UPARCHIVE_H
#define UPARCHIVE_H


// ============================================================================
// EByteInfo
// ============================================================================
enum EByteInfo
{
	BI_None					= 0x00,
	BI_Header				= 0x01,
	BI_NameTable			= 0x02,
	BI_ImportTable			= 0x04,
	BI_ExportTable			= 0x08,
	BI_GenerationTable		= 0x10,
	BI_Object				= 0x20,
};

inline wxString upGetByteInfoName( EByteInfo info )
{
	switch(info)
	{
	case BI_None:				return wxT("BI_None");
	case BI_Header:				return wxT("BI_Header");
	case BI_NameTable:			return wxT("BI_NameTable");
	case BI_ImportTable:		return wxT("BI_ImportTable");
	case BI_ExportTable:		return wxT("BI_ExportTable");
	case BI_GenerationTable:	return wxT("BI_GenerationTable");
	case BI_Object:				return wxT("BI_Object");
	default:					return wxT("BI_UNKNOWN");
	}
}


// ============================================================================
// upFileGuard
// ============================================================================
struct upFileGuard
{
	dword	From;
	dword	To;

    upFileGuard( dword from, dword to ) : From(from), To(to)
	{
	}
};


// ============================================================================
// upArchive
// ============================================================================
class upArchive
{
public:
	virtual ~upArchive() = 0;

public:
	virtual void			Serialize	( void* buffer, dword count ) = 0;

public:
	virtual void			Seek		( wxFileOffset pos, wxSeekMode from = wxFromStart ) = 0;
	virtual void			SeekEnd		( wxFileOffset pos = 0 ) = 0;
	virtual wxFileOffset	Tell		() = 0;
	virtual wxFileOffset	Length		() = 0;

public:
	virtual bool			IsOpened	() = 0;
	virtual bool			IsLoading	() = 0;
	virtual bool			IsSaving	() = 0;

public:
	virtual void			PushGuard	( wxFileOffset from, wxFileOffset to ) = 0;
	virtual void			PopGuard	() = 0;
	virtual upFileGuard		GetGuard	() = 0;

public:
	virtual void			SetByteInfo	( EByteInfo info ) = 0;
	virtual EByteInfo		GetByteInfo	() = 0;
	
public:
	virtual wxString		DebugInfo		() = 0;
	virtual void			DumpByteInfo	() = 0;
	
public:
	friend void operator << ( upArchive& A, qword& D )		{ A.Serialize( &D, sizeof(qword) ); /*wxUINT64_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, dword& D )		{ A.Serialize( &D, sizeof(dword) ); /*wxUINT32_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, word& D )		{ A.Serialize( &D, sizeof(word) ); /*wxUINT16_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, byte& D )		{ A.Serialize( &D, sizeof(byte) );  }
	friend void operator << ( upArchive& A, int64& D )		{ A.Serialize( &D, sizeof(int64) ); /*wxINT64_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, int32& D )		{ A.Serialize( &D, sizeof(int32) ); /*wxINT32_SWAP_ON_?E(D);*/  }
	friend void operator << ( upArchive& A, int16& D )		{ A.Serialize( &D, sizeof(int16) ); /*wxINT16_SWAP_ON_?E(D);*/ }
	friend void operator << ( upArchive& A, int8& D )		{ A.Serialize( &D, sizeof(int8) );  }
	friend void operator << ( upArchive& A, char& D )		{ A.Serialize( &D, sizeof(int8) );  }
	friend void operator << ( upArchive& A, float& D )		{ A.Serialize( &D, sizeof(dword) ); /*wxINT32_SWAP_ON_?E(D);*/ }

public:
	friend upArchive& operator << ( upArchive& A, EByteInfo D )		{ A.SetByteInfo(D); return A; }
};


// ============================================================================
// upOffsetGuard
// ============================================================================
class upOffsetGuard
{
private:
	upArchive& A; 

public:
	upOffsetGuard( upArchive& a, wxFileOffset from, wxFileOffset to ) : A(a)
	{
		A.PushGuard(from,to);
	}

	~upOffsetGuard()
	{
		A.PopGuard();
	}
};

#define UP_OFFSET_GUARD(name,arc,from,to) upOffsetGuard offsetguard_##name(arc,from,to);



#endif UPARCHIVE_H
// ============================================================================
//	The End.
// ============================================================================