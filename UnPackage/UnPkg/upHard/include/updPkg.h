// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	updPkg.h
// ============================================================================
#ifndef UPDPKG_H
#define UPDPKG_H

#include "upFile.h"
#include "updTypes.h"
#include "updTables.h"
#include "updTran.h"


// ============================================================================
// updConst
// ============================================================================
namespace updConst
{
	const qword signature = 0x0A1A0A0D44505589;
	const dword version	= 0x00000001;
}


// ============================================================================
// updHeader
// ============================================================================
class updHeader
{
public:
	qword Signature;
	dword Version;
	dword Flags;

	word TargetVersion;
	word TargetMode;
	dword TargetFlags;
	uppGUID TargetGuid;

	dword TargetFileSize;

	//dword NameTableSize;
	//dword NameTableOffset;

	//dword ExportTableSize;
	//dword ExportTableOffset;

	//dword ImportTableSize;
	//dword ImportTableOffset;

	//dword GenerationTableSize;
	//dword GenerationTableOffset;

	// TODO:: store target filesize for final maxsize

	friend void operator << ( upArchive& A, updHeader& D )
	{
		A << D.Signature;
		A << D.Version;
		A << D.Flags;

		A << D.TargetVersion;
		A << D.TargetMode;
		A << D.TargetFlags;
		A << D.TargetGuid;

		//A << D.TargetFileSize;
	}
};



// ============================================================================
// updPkg
// ============================================================================
class updPkg
{
public:
	updHeader Header;
	updNameTable NameTable;
	updExportTable ExportTable;
	updImportTable ImportTable;
	updGenerationTable GenerationTable;
	
	uppPkg& Basis;	// reference file (.dx)
	uppPkg& Target;	// modified reference file (.dxs)
	
	updTran	Tran;

	upStrDwordMap BNmap, TNmap;
	upStrDwordMap BEmap, TEmap;
	upStrDwordMap BImap, TImap;
	
public:
	updPkg( uppPkg& basis, uppPkg& target )
	: Basis(basis), Target(target), Tran(basis,target)
	{
		 
	}

public:
	void Serialize( upArchive& A );

private:
	int TranslateBasisItem( int i );
	void DumpTranBasis();

private:
	void CreateHeader();
	void CreateNames();
	void CreateGenerations();
	void CreateExports();
	void CreateImports();
	void VerifyDelta();

private:
	void ApplyHeader();
	void ApplyNames();
	void ApplyGenerations();
	void ApplyExports();
	void ApplyImports();
	void ApplyExportsTranslation();
	void ApplyImportsTranslation();

};



#endif UPDPKG_H
// ============================================================================
//	The End.
// ============================================================================