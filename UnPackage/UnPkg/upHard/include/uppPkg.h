// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppPkg.h
// ============================================================================
#ifndef UPP_H
#define UPP_H

#include "upArchive.h"
#include "uppTables.h"


// ============================================================================
// uppConst
// ============================================================================
namespace uppConst
{
	const dword signature	= 0x9E2A83C1;
	const dword minversion	= 68;
}


// ============================================================================
// uppHeader
// ============================================================================
class uppHeader
{
public:
	dword	Signature;
	word	PackageVersion;
	word	LicenseMode;
	dword	PackageFlags;
	uppGUID	GUID;

public:
	friend void operator << ( upArchive& A, uppHeader& D )
	{
		A << D.Signature;
		if( D.Signature != uppConst::signature )
			throw upexception(wxT("Unknown package signature"));

		A << D.PackageVersion;
		if( D.PackageVersion < uppConst::minversion )
			throw upexception(wxT("Unsupported package version"));

		A << D.LicenseMode;
		A << D.PackageFlags;

	}
};

// ============================================================================
// uppPkg
// ============================================================================
class uppPkg
{
public:
	uppHeader			Header;
	uppGenerationTable	GenerationTable;
	uppNameTable		NameTable;	
	uppExportTable		ExportTable;	
	uppImportTable		ImportTable;	

public:
    uppPkg();
    virtual ~uppPkg();

public: 
	virtual void Serialize( upArchive& A );

public: 
	wxString DebugInfo();

	wxString GetNameString( int i )
	{
		return NameTable.at(i).Name.str();
	}

	wxString GetObjectName( int r )
	{
		if( r > 0 )			return NameTable.at(ExportTable.at(r-1).ObjectName).Name.str();
		else if( r < 0 )	return NameTable.at(ImportTable.at(-r-1).ObjectName).Name.str();
		else				return wxT("");
	}

	wxString GetObjectPath( int r )
	{
		wxString S;

		if( r == 0 )
			return wxT("");

		// object name
		if( r > 0 )
		{
			S = NameTable.at(ExportTable.at(r-1).ObjectName).Name.str();
			r = ExportTable.at(r-1).Package;
		}
		else
		{
			S = NameTable.at(ImportTable.at(-r-1).ObjectName).Name.str();
			r = ImportTable.at(-r-1).Package;
		}

		if( r == 0 )
			return S;

		// package path
		for( int i=0; i!=255; ++i )
		{
			if( r > 0 )
			{
				S.Prepend( NameTable.at(ExportTable.at(r-1).ObjectName).Name.str() + wxT(".") );
				r = ExportTable.at(r-1).Package;
			}
			else
			{
				S.Prepend( NameTable.at(ImportTable.at(-r-1).ObjectName).Name.str() + wxT(".") );
				r = ImportTable.at(-r-1).Package;
			}

			if( r == 0 )
				return S;
		}

		throw upexception( wxString::Format( wxT("Object path depth > 255") ) );
	}
};





#endif UPP_H
// ============================================================================
//	The End.
// ============================================================================