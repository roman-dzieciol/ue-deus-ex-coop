// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upFileBuffer.h
// ============================================================================
#ifndef UPFILEBUFFER_H
#define UPFILEBUFFER_H

#include "upArchive.h"


// ============================================================================
// upFile
// ============================================================================
class upFileBuffer
{
protected:
	dword Offset;
	dword Size;
	dword MaxSize;
	dword GuardMin;
	dword GuardMax;
	EByteInfo ByteInfo;
	vector<byte> Data;
	vector<byte> Markers;
	vector<upFileGuard> Guards;
	bool bOpened;

public:
	upFileBuffer( dword maxsize = 0x40000000 ) 
	:	Offset(0)
	,	Size(0)
	,	MaxSize(maxsize)
	,	GuardMin(0)
	,	GuardMax(0)
	,	ByteInfo(BI_None)
	,	Guards( 1, upFileGuard(0,0) )
	,	bOpened(false)
	{
		// Guard 0 is 0,0
		// Guard 1 is 0,MaxSize
	}

	~upFileBuffer()
	{
	}


public:
	void Seek( wxFileOffset offset, wxSeekMode mode = wxFromStart )
	{
		if( mode == wxFromStart )		Offset = offset;
		else if( mode > wxFromCurrent )	Offset += offset;
		else							Offset = static_cast<dword>(Size)-offset;
		
		if( Offset < 0 || Offset > Size )
		{
			throw upexception( wxString::Format( wxT("Invalid seek <0x%.8x> outside valid range <0x%.8x 0x%.8x>")
			, static_cast<dword>(Offset) 
			, static_cast<dword>(0) 
			, static_cast<dword>(Size) 
			) );
		}
	}

	wxFileOffset Tell() const { return Offset; }
	wxFileOffset Length() const { return Size; }
	
	byte* GetDataPtr() { return Data.empty() ? NULL : &Data UP_AT(0);  }

public:
	bool IsOpened() const { return bOpened; }

public:
    void PushGuard( dword from, dword to )
    {
		if( from >= 0 && from < MaxSize && to >= 0 && to <= MaxSize )
		{
			Guards.push_back( upFileGuard(from,to) );
			GuardMin = from;
			GuardMax = to;
		}
		else
		{
			throw upexception( wxString::Format( wxT("File guard <0x%.8x 0x%.8x> outside valid range <0x%.8x 0x%.8x>")
			, static_cast<dword>(from) 
			, static_cast<dword>(to) 
			, static_cast<dword>(0) 
			, static_cast<dword>(MaxSize) 
			) );
		}
    }

	void PopGuard()
	{
		Guards.pop_back();
		GuardMin = Guards.back().From;
		GuardMax = Guards.back().To;
	}

	upFileGuard GetGuard() const
	{
		return Guards.back();
	}

public:
	EByteInfo	GetByteInfo	() const		{ return ByteInfo; }
	void		SetByteInfo	( EByteInfo d )	{ ByteInfo = d; }

public:
	void DumpByteInfo();
};


// ============================================================================
// upFileReadBuffer
// ============================================================================
class upFileReadBuffer : public upFileBuffer
{
public:
	upFileReadBuffer( dword maxsize = 0x40000000 ) 
	:	upFileBuffer(maxsize)
	{
	}

	~upFileReadBuffer()
	{
	}

	void Init( wxFFile& file )
	{
		//UP_TIMER(t);

		if( !bOpened )
			bOpened = true;
		else
			throw upexception( wxString::Format( wxT("upFileBuffer already opened") ) );
		
		Size = file.Length();
		if( Size > MaxSize )
			throw upexception( wxString::Format( wxT("Excessive file size: %d"), Size ) );

		MaxSize = Size;
		
		Data.reserve(Size);
		Data.resize(Size);

		Markers.reserve(Size);
		Markers.resize(Size);
		
		PushGuard(0,MaxSize);

		file.Seek(0);
		file.Read(&Data UP_AT(0),Size);

		memset(&Markers UP_AT(0),0,Size);

		//UP_TIMER_LOGF(t,0);
	}

public:
    void Read( void* buffer, dword count ) 
	{  
		if( Offset >= GuardMin && Offset + count <= GuardMax )
		{
			// Mark byte as read
			memset( &Markers UP_AT(Offset), static_cast<byte>(ByteInfo), count );

			// Read
			memcpy( buffer, &Data UP_AT(Offset), count );
			Offset += count;
		}
		else
		{
			throw upexception( wxString::Format( wxT("File read <0x%.8x 0x%.8x> outside guarded range <0x%.8x 0x%.8x>")
			, static_cast<dword>(Offset) 
			, static_cast<dword>(Offset + count) 
			, static_cast<dword>(GuardMin) 
			, static_cast<dword>(GuardMax) 
			) );
		}
	}
};


// ============================================================================
// upFileWriteBuffer
// ============================================================================
class upFileWriteBuffer : public upFileBuffer
{
public:
	upFileWriteBuffer( dword maxsize = 0x40000000 ) 
	:	upFileBuffer(maxsize)
	{
	}

	~upFileWriteBuffer()
	{
	}

	void Init( wxFFile& WXUNUSED(file) )
	{
		//UP_TIMER(t);

		if( !bOpened )
			bOpened = true;
		else
			throw upexception( wxString::Format( wxT("upFileBuffer already opened") ) );
		
		// reserve 1024KB
		Data.reserve(0x100000); 

		PushGuard(0,MaxSize);

		//UP_TIMER_LOGF(t,0);
	}

public:
	void Save( wxFFile& file )
	{
		if( Size > 0 )
		{
			file.Seek(0);
			file.Write(&Data UP_AT(0),Size);
		}
	}

    void Write( void* buffer, dword count ) 
	{  
		const dword newpos = Offset + count;
		if( Offset >= GuardMin && newpos <= GuardMax )
		{
			if( newpos > Size )
			{
				Data.resize(newpos);
				Size = newpos;
			}
			memcpy( &Data UP_AT(Offset), buffer, count );
			Offset += count;
		}
		else
		{
			throw upexception( wxString::Format( wxT("File write <0x%.8x 0x%.8x> outside guarded range <0x%.8x 0x%.8x>")
			, static_cast<dword>(Offset) 
			, static_cast<dword>(newpos) 
			, static_cast<dword>(GuardMin) 
			, static_cast<dword>(GuardMax) 
			) );
		}
	}

};

#endif UPFILEBUFFER_H
// ============================================================================
//	The End.
// ============================================================================