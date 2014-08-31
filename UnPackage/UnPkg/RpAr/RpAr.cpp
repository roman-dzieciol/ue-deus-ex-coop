// ============================================================================
//  UnPkg :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	stdafx.cpp : source file that includes just the standard includes
//	upHard.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information
// ============================================================================

#include "stdafx.h"
#include "RpAr.h"
#include "uppPkg.h"
#include "updPkg.h"
#include "upFile.h"
#include "sha1.h"
#include "RpLzma.h"

// TODO: finish exceptions, ie on rename


BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			wxInitialize();
		break;

		case DLL_THREAD_ATTACH:
		break;

		case DLL_THREAD_DETACH:
		break;

		case DLL_PROCESS_DETACH:
			wxUninitialize();
		break;
	}
	return TRUE;
}

struct FIntException
{
	int Error;
	explicit FIntException( int err=-1 ) : Error(err){}
};


struct FDeltaHash
{
	dword Data[5];

	friend int operator == ( const FDeltaHash& left, const FDeltaHash& right )
	{
		return	left.Data[0] == right.Data[0]
		&&		left.Data[1] == right.Data[1]
		&&		left.Data[2] == right.Data[2]
		&&		left.Data[3] == right.Data[3]
		&&		left.Data[4] == right.Data[4];
	}

	friend int operator != ( const FDeltaHash& left, const FDeltaHash& right )
	{
		return	!(left == right);
	}

	void Write( wxFile& f )
	{
		if( f.Write( Data, GetHashSize() ) != GetHashSize() )
			throw FIntException(270);
	}

	void Read( wxFile& f )
	{
		if( f.Read( Data, GetHashSize() ) != GetHashSize() )
			throw FIntException(220);
	}

	static int GetHashSize()
	{
		return sizeof(dword)*5;
	}
};

struct FDeltaIntegrity
{
	FDeltaHash Map;
	FDeltaHash Save;
	FDeltaHash Delta;
	FDeltaHash Compressed;

	friend int operator == ( const FDeltaIntegrity& left, const FDeltaIntegrity& right )
	{
		return	left.Map == right.Map
		&&		left.Save == right.Save
		&&		left.Delta == right.Delta
		&&		left.Compressed == right.Compressed;
	}

	friend int operator != ( const FDeltaIntegrity& left, const FDeltaIntegrity& right )
	{
		return	!(left == right);
	}

	static int GetHashesSize()
	{
		return	FDeltaHash::GetHashSize() * 4;
	}

	void Write( wxString filename )
	{
		wxFile f;
		if( f.Open(filename,wxFile::write_append) )
		{
			Map.Write(f);
			Save.Write(f);
			Delta.Write(f);
			Compressed.Write(f);
			return;
		}
		throw FIntException(250);
	}

	void Read( wxString filename )
	{
		wxFile f;
		if( f.Open(filename,wxFile::read) )
		{
			if( f.SeekEnd(-GetHashesSize()) != wxInvalidOffset )
			{
				Map.Read(f);
				Save.Read(f);
				Delta.Read(f);
				Compressed.Read(f);
				return;
			}
		}
		throw FIntException(200);
	}

	void DumpHashes()
	{
		wxLogMessage( wxT("%.8x%.8x%.8x%.8x%.8x"), Map.Data[0], Map.Data[1], Map.Data[2], Map.Data[3], Map.Data[4] );
		wxLogMessage( wxT("%.8x%.8x%.8x%.8x%.8x"), Delta.Data[0], Delta.Data[1], Delta.Data[2], Delta.Data[3], Delta.Data[4] );
		wxLogMessage( wxT("%.8x%.8x%.8x%.8x%.8x"), Save.Data[0], Save.Data[1], Save.Data[2], Save.Data[3], Save.Data[4] );
		wxLogMessage( wxT("%.8x%.8x%.8x%.8x%.8x"), Compressed.Data[0], Compressed.Data[1], Compressed.Data[2], Compressed.Data[3], Compressed.Data[4] );
	}
};

void GetFileHash( wxString filename, FDeltaHash& hash, int offset=0 )
{
	wxFile f;
	if( f.Open(filename,wxFile::read) )
	{
		int length = f.Length() + offset;
		if( length > 1 )
		{
			vector<byte> buffer;
			buffer.reserve(length);
			buffer.resize(length);
			if( f.Read(&buffer[0],length) == length )
			{
				static SHA1 sha;
				sha.Reset();
				sha.Input(&buffer[0],length);
				sha.Result(hash.Data);
				return;
			}
		}
	}
	throw FIntException(150);
}

RPAR_API int RpPack( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int mode )
{
	try
	{
		wxString mapstr = map; // dx
		wxString savestr = save; // dxs
		wxString deltastr = delta; // dxd
		wxString tempstr = deltastr + wxT("t"); // dxdt

		// if delta needs to be created
		if( map != save )
		{
			// keep in block so handles are freed
			{
				// prepare files
				upFileReader mapfr( mapstr );
				upFileReader savefr( savestr );
				upFileWriter deltafw( deltastr, savefr.Length() );

				// load map
				uppPkg mappkg;
				mappkg.Serialize(mapfr);

				// load save
				uppPkg savepkg;
				savepkg.Serialize(savefr);

				// create delta
				updPkg deltapkg(mappkg,savepkg);
				deltapkg.Serialize(deltafw);
			}
				
			// compress
			RpCompress(deltastr,tempstr);

			// hashes
			FDeltaIntegrity integrity;
			GetFileHash( mapstr, integrity.Map );
			GetFileHash( savestr, integrity.Save );
			GetFileHash( deltastr, integrity.Delta );
			GetFileHash( tempstr, integrity.Compressed );
			integrity.Write( tempstr );
			//integrity.DumpHashes();

			// rename compressed to delta filename
			if( !wxRenameFile(tempstr,deltastr,true) )
				throw FIntException(300);
		}
		else
		{
			// Create 1-byte long file
			wxFile f;
			if( !f.Open(deltastr,wxFile::write) )
				throw FIntException(400);
			byte b = 1;
			f.Write(&b,1);
			f.Close();
		}
		
		return 0;
	
	}
	catch(FIntException err)
	{
		return err.Error;
	}
	catch(...)
	{
		return -1;
	}
}

RPAR_API int RpUnpack( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int mode )
{
	try
	{
		wxString mapstr = map; // dx
		wxString savestr = save; // dxs
		wxString deltastr = delta; // dxd
		wxString tempstr = deltastr + wxT("t"); // dxdt

		// get delta length
		wxFile f;
		if( !f.Open(deltastr,wxFile::read) )
			throw FIntException(400);
		int i = f.Length();
		f.Close();

		// if delta length == 1, copy the map
		if( i == 1 )
		{
			wxCopyFile(mapstr,savestr,true);
			return 2;
		}
		else
		{
			// delta hashes
			FDeltaIntegrity delta_integrity;
			delta_integrity.Read( deltastr );
			//delta_integrity.DumpHashes();

			// decompress
			RpDecompress(deltastr,tempstr);

			// partial local hashes
			FDeltaIntegrity integrity;
			GetFileHash( mapstr, integrity.Map );
			GetFileHash( tempstr, integrity.Delta );
			GetFileHash( deltastr, integrity.Compressed, -FDeltaIntegrity::GetHashesSize() );
			//integrity.DumpHashes();

			// verify hashes
			if( delta_integrity.Map != integrity.Map )					throw FIntException(101);
			if( delta_integrity.Delta != integrity.Delta )				throw FIntException(102);
			if( delta_integrity.Compressed != integrity.Compressed )	throw FIntException(103);
				
			// rename compressed to delta filename
			if( !wxRenameFile(tempstr,deltastr,true) )
				throw FIntException(300);
			
			// keep in block so handles are freed
			{
				// prepare files
				upFileReader mapfr( mapstr );
				upFileWriter savefw( savestr, mapfr.Length() );
				upFileReader deltafr( deltastr );

				// load map
				uppPkg mappkg;
				mappkg.Serialize(mapfr);

				// apply delta
				uppPkg savepkg;
				updPkg deltapkg(mappkg,savepkg);
				deltapkg.Serialize(deltafr);

				// write save
				savepkg.Serialize(savefw);
			}

			// verify savegame hash
			GetFileHash( savestr, integrity.Save );
			//integrity.DumpHashes();
			if( delta_integrity.Save != integrity.Save )
				throw FIntException(104);
		}


		return 0;
	
	}
	catch(FIntException err)
	{
		return err.Error;
	}
	catch(...)
	{
		return -1;
	}
}

RPAR_API int RpIsError( int idx )
{
	return idx < 0 || idx >= 100;
}

// This is an example of an exported variable
//RPAR_API int nRpAr=0;

// This is an example of an exported function.
//RPAR_API int fnRpAr(void)
//{
//	return 42;
//}

// This is the constructor of a class that has been exported.
// see RpAr.h for the class definition
//CRpAr::CRpAr()
//{
//	return;
//}



// ============================================================================
//	The End.
// ============================================================================