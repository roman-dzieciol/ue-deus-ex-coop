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

inline int RpHasFlag( int val, int flag )
{
	return (val & flag) == flag;
}

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
	dword Length;

	friend int operator == ( const FDeltaHash& left, const FDeltaHash& right )
	{
		return	left.Data[0] == right.Data[0]
		&&		left.Data[1] == right.Data[1]
		&&		left.Data[2] == right.Data[2]
		&&		left.Data[3] == right.Data[3]
		&&		left.Data[4] == right.Data[4]
		&&		left.Length == right.Length;
	}

	friend int operator != ( const FDeltaHash& left, const FDeltaHash& right )
	{
		return	!(left == right);
	}

	void Write( wxFile& f )
	{
		if( f.Write( Data, GetHashSize() ) != GetHashSize() )
			throw FIntException(270);
		if( f.Write( &Length, sizeof(dword) ) != sizeof(dword) )
			throw FIntException(271);
	}

	void Read( wxFile& f )
	{
		if( f.Read( Data, GetHashSize() ) != GetHashSize() )
			throw FIntException(220);
		if( f.Read( &Length, sizeof(dword) ) != sizeof(dword) )
			throw FIntException(221);
	}

	static int GetHashSize()
	{
		return sizeof(dword)*5;
	}

	static int GetSerialSize()
	{
		return GetHashSize() + sizeof(dword)*1;
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

	static int GetSerialSize()
	{
		return	FDeltaHash::GetSerialSize() * 4;
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
			if( f.SeekEnd(-GetSerialSize()) != wxInvalidOffset )
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

	static void CalcHash( wxString filename, FDeltaHash& hash, int offset=0 )
	{
		wxFile f;
		if( f.Open(filename,wxFile::read) )
		{
			hash.Length = f.Length() + offset;
			if( hash.Length > 1 )
			{
				vector<byte> buffer;
				buffer.reserve(hash.Length);
				buffer.resize(hash.Length);
				if( f.Read(&buffer[0],hash.Length) == hash.Length )
				{
					static SHA1 sha;
					sha.Reset();
					sha.Input(&buffer[0],hash.Length);
					sha.Result(hash.Data);
					return;
				}
			}
		}
		throw FIntException(150);
	}

	void DumpHashes()
	{
		wxLogMessage( wxT("%.8x%.8x%.8x%.8x%.8x"), Map.Data[0], Map.Data[1], Map.Data[2], Map.Data[3], Map.Data[4], Map.Length );
		wxLogMessage( wxT("%.8x%.8x%.8x%.8x%.8x"), Delta.Data[0], Delta.Data[1], Delta.Data[2], Delta.Data[3], Delta.Data[4], Map.Length );
		wxLogMessage( wxT("%.8x%.8x%.8x%.8x%.8x"), Save.Data[0], Save.Data[1], Save.Data[2], Save.Data[3], Save.Data[4], Map.Length );
		wxLogMessage( wxT("%.8x%.8x%.8x%.8x%.8x"), Compressed.Data[0], Compressed.Data[1], Compressed.Data[2], Compressed.Data[3], Compressed.Data[4], Map.Length );
	}
};


RPAR_API int RpPack( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int flags )
{
	if( RpHasFlag(flags,AM_Verbose) )
	{
		try
		{
			wxLogMessage(wxT("RpPack %s %s %s %.8x"), map, save, delta, flags);
			return RpPackInternal(map,save,delta,flags);
		}
		catch( FIntException err ) { return err.Error; }
		catch( upexception& e ) { wxLogError(wxT("%s"), e.wwhat()); return -1; }		  
		catch( exception& e ) { wxLogError(wxT("%hs"), e.what()); return -1; }	  
		catch( ... ) { wxLogError(wxT("unknown exception")); return -1; }
	}
	else
	{
		try
		{
			return RpPackInternal(map,save,delta,flags);
		}
		catch( FIntException err ) { return err.Error; }
		catch(...) { return -1; }
	}
}

int RpPackInternal( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int flags )
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
			upFileWriter deltafw( deltastr, wxMax(mapfr.Length(),savefr.Length()) );

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
		FDeltaIntegrity::CalcHash( mapstr, integrity.Map );
		FDeltaIntegrity::CalcHash( savestr, integrity.Save );
		FDeltaIntegrity::CalcHash( deltastr, integrity.Delta );
		FDeltaIntegrity::CalcHash( tempstr, integrity.Compressed );
		integrity.Write( tempstr );

		if( RpHasFlag(flags,AM_DumpHashes) )
			integrity.DumpHashes();
		
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

RPAR_API int RpUnpack( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int flags )
{
	if( RpHasFlag(flags,AM_Verbose) )
	{
		try
		{
			wxLogMessage(wxT("RpUnpack %s %s %s %.8x"), map, save, delta, flags);
			return RpUnpackInternal(map,save,delta,flags);
		}
		catch( FIntException err ) { return err.Error; }
		catch( upexception& e ) { wxLogError(wxT("%s"), e.wwhat()); return -1; }		  
		catch( exception& e ) { wxLogError(wxT("%hs"), e.what()); return -1; }	  
		catch( ... ) { wxLogError(wxT("unknown exception")); return -1; }
	}
	else
	{
		try
		{
			return RpUnpackInternal(map,save,delta,flags);
		}
		catch( FIntException err ) { return err.Error; }
		catch(...) { return -1; }
	}
}

int RpUnpackInternal( const wchar_t* map, const wchar_t* save, const wchar_t* delta, int flags )
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
		if( RpHasFlag(flags,AM_DumpHashes) )
			delta_integrity.DumpHashes();

		// decompress
		RpDecompress(deltastr,tempstr);

		// partial local hashes
		FDeltaIntegrity integrity;
		FDeltaIntegrity::CalcHash( mapstr, integrity.Map );
		FDeltaIntegrity::CalcHash( tempstr, integrity.Delta );
		FDeltaIntegrity::CalcHash( deltastr, integrity.Compressed, -FDeltaIntegrity::GetSerialSize() );

		if( RpHasFlag(flags,AM_DumpHashes) )
			integrity.DumpHashes();


		// verify hashes
		if( delta_integrity.Map != integrity.Map )					throw FIntException(101);
		if( delta_integrity.Delta != integrity.Delta )				throw FIntException(102);
		if( delta_integrity.Compressed != integrity.Compressed )	throw FIntException(103);
		
		// handle compressed file
		if( !RpHasFlag(flags,AM_SilentUnpack) )
		{
			// rename compressed to delta filename
			if( !wxRenameFile(tempstr,deltastr,true) )
				throw FIntException(300);
		}
		else
		{
			deltastr = tempstr;
		}
		
		// keep in block so handles are freed
		{
			// prepare files
			upFileReader mapfr( mapstr );
			upFileWriter savefw( savestr, delta_integrity.Save.Length );
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
		FDeltaIntegrity::CalcHash( savestr, integrity.Save );

		if( RpHasFlag(flags,AM_DumpHashes) )
			integrity.DumpHashes();

		if( delta_integrity.Save != integrity.Save )
			throw FIntException(104);

		// handle compressed file
		if( RpHasFlag(flags,AM_SilentUnpack) )
		{
			if( !wxRemoveFile(tempstr) )
				throw FIntException(301);
		}
	}


	return 0;
}

RPAR_API int RpIsError( int idx )
{
	return idx < 0 || idx >= 100;
}

// ============================================================================
//	The End.
// ============================================================================