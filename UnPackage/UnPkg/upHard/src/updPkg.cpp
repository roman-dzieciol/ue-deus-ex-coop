// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	updPkg.cpp
// ============================================================================
#include "upPrec.h"
#include "updPkg.h"
#include "updObject.h"


/* ============================================================================
	Delta Format
===============================================================================
	
============================================================================ */


// ============================================================================
//	updPkg
// ============================================================================
void updPkg::Serialize( upArchive& A )
{
	//UP_TIMER(t);

	if( A.IsLoading() )
	{
		// This is a .dxd file
		// Basis is a .dx file
		// Target is a new .dxs file

		// Load header
		A.Seek(0);
		A << Header;
		A << static_cast<uppTableHeader&>(NameTable);
		A << static_cast<uppTableHeader&>(ExportTable);
		A << static_cast<uppTableHeader&>(ImportTable);

		// Load tables
		A << GenerationTable;
		A << NameTable;
		A << ImportTable;
		A << ExportTable;

		// Set target table sizes using delta table sizes
		Target.GenerationTable.SetSize( GenerationTable.size() );
		Target.NameTable.SetSize( NameTable.size() );
		Target.ExportTable.SetSize( ExportTable.size() );
		Target.ImportTable.SetSize( ImportTable.size() );

		// Reset translator tables
		Tran.Reset(Basis,Target);
		
		// Apply delta
		ApplyHeader();
		ApplyNames();
		ApplyGenerations();
		ApplyExports();
		ApplyImports();
		ApplyExportsTranslation();
		ApplyImportsTranslation();

		//Tran.DumpBasis();
		//Tran.DumpTarget();

		// Create objects and apply delta
		for( updExportTableItr it=ExportTable.begin(); it!=ExportTable.end(); ++it )
		{
			updExportItem& dti = *it;

			if( dti.DeltaSize != 0 )
			{
				const int didx = it-ExportTable.begin();

				// create target object
				uppExportItem& tti = Target.ExportTable UP_AT(didx);
				uppObject* tobj = new uppObject(Target,tti);

				// create delta object
				A.Seek( dti.DeltaOffset );
				updObject* dobj = new updObject(*this,dti);

				// get basis object
				uppObject* bobj = Tran.HasBasisExport(didx) ? Basis.ExportTable UP_AT(Tran.GetBasisExport(didx)).Object : NULL;

				// serialize
				dobj->Serialize(A,bobj,*tobj,Tran);
			}
		}

		//wxLogMessage( wxT("NameTable: %d"), NameTable.size() );
		//wxLogMessage( wxT("GenerationTable: %d"), GenerationTable.size() );
		//wxLogMessage( wxT("ImportTable: %d"), ImportTable.size() );
		//wxLogMessage( wxT("ExportTable: %d"), ExportTable.size() );

		//Tran.DumpBasis();
	}
	else
	{
		// This is a new .dxd file
		// Basis is a .dx file
		// Target is a .dxs file

		// Create delta
		CreateHeader();
		CreateNames();
		CreateGenerations();
		CreateExports();
		CreateImports();

		// Save header
		A.Seek(0);
		A << Header;
		wxFileOffset headers = A.Tell();
		A << static_cast<uppTableHeader&>(NameTable);
		A << static_cast<uppTableHeader&>(ExportTable);
		A << static_cast<uppTableHeader&>(ImportTable);

		// Save tables
		A << GenerationTable;
		A << NameTable;

		dword nameend = A.Tell();
		wxLogMessage( wxT("SIZE NameTable: %d"), int(A.Tell()-NameTable.GetOffset()) );

		// Create & save delta objects
		for( updExportTableItr it=ExportTable.begin(); it!=ExportTable.end(); ++it )
		{
			updExportItem& dti = *it;
			const int didx = it-ExportTable.begin();

			// get target object
			uppObject* tobj = Target.ExportTable UP_AT(didx).Object;
			if( tobj != NULL )
			{
				// create delta object
				updObject* dobj = new updObject(*this,dti);

				// get basis object
				uppObject* bobj = Tran.HasBasisExport(didx) ? Basis.ExportTable UP_AT(Tran.GetBasisExport(didx)).Object : NULL;

				// serialize
				dti.DeltaOffset = A.Tell();
				dobj->Serialize(A,bobj,*tobj,Tran);
				dti.DeltaSize = A.Tell()-dti.DeltaOffset;
			}
			else
			{
				dti.DeltaOffset = 0;
				dti.DeltaSize = 0;
			}
		}
		wxLogMessage( wxT("SIZE Objects: %d"), int(A.Tell()-nameend) );

		// Save tables
		A << ImportTable;
		wxLogMessage( wxT("SIZE ImportTable: %d"), int(A.Tell()-ImportTable.GetOffset()) );

		A << ExportTable;
		wxLogMessage( wxT("SIZE ExportTable: %d"), int(A.Tell()-ExportTable.GetOffset()) );
		
		// Update table headers
		wxFileOffset resume = A.Tell();
		A.Seek(headers);
		A << static_cast<uppTableHeader&>(NameTable);
		A << static_cast<uppTableHeader&>(ExportTable);
		A << static_cast<uppTableHeader&>(ImportTable);
		A.Seek(resume);
	}
}


void updPkg::CreateHeader()
{
	// Delta header
	Header.Signature = updConst::signature;
	Header.Version = updConst::version;
	Header.Flags = 0x00000000;

	// package header
	Header.TargetVersion = Target.Header.PackageVersion;
	Header.TargetMode = Target.Header.LicenseMode;
	Header.TargetFlags = Target.Header.PackageFlags;
	Header.TargetGuid = Target.Header.GUID;
}



void updPkg::CreateNames()
{
	// Basis name table
	for( uppNameTableItr it=Basis.NameTable.begin(); it!=Basis.NameTable.end(); ++it )
	{
		// Map
		const upStrDwordMap::Insert_Result ir = BNmap.insert( upStrDwordMap::value_type( (*it).Name.str(), it-Basis.NameTable.begin() ) );
		if( ir.second == false )
			throw upexception( wxString::Format( wxT("Basis name already mapped: %s"), (*it).Name.str().c_str() ) );
	}

	// Target name table
	for( uppNameTableItr it=Target.NameTable.begin(); it!=Target.NameTable.end(); ++it )
	{
		const uppNameItem& tti = *it;
		const int tidx = it-Target.NameTable.begin();
		const wxString tstr = tti.Name.str();

		// Map
		const upStrDwordMap::Insert_Result ir = TNmap.insert( upStrDwordMap::value_type( tstr, tidx ) );
		if( ir.second == false )
			throw upexception( wxString::Format( wxT("Target name already mapped: %s"), tstr.c_str() ) );

		// Delta name table
		const upStrDwordMap::iterator bmi = BNmap.find( tti.Name.str() );
		if( bmi != BNmap.end() )
		{
			const int bidx = (*bmi).second;
			const uppNameItem& bti = Basis.NameTable UP_AT(bidx);
			if( tti.Flags == bti.Flags )
			{
				// push basis item pointer
				updNameItem dti;
				dti.Name = uppLASCIIZ(-bidx-1);
				NameTable.push_back(dti);

				// store translation
				Tran.SetName(bidx,tidx);

				//wxLogMessage( wxT("name old: %s"), bni.Name.str().c_str() );
				continue;
			}
			else
			{
				//wxLogMessage( wxT("name similar: %s"), bni.Name.str().c_str() );
			}
		}
		else
		{
			//wxLogMessage( wxT("name new: %s"), tni.Name.c_str().str() );
		}

		// push new item
		updNameItem dti;
		dti.Name = tti.Name;
		dti.Flags = tti.Flags;
		NameTable.push_back(dti);
	}
}



void updPkg::CreateGenerations()
{
	for( uppGenerationTableItr ti=Target.GenerationTable.begin(); ti!=Target.GenerationTable.end(); ++ti )
	{
		const uppGenerationItem& tti = *ti;

		// find in basis table
		uppGenerationTableItr bi = Basis.GenerationTable.end();
		for( bi=Basis.GenerationTable.begin(); bi!=Basis.GenerationTable.end(); ++bi )
		{
			const uppGenerationItem& bti = *bi;
			if( bti.ExportCount == tti.ExportCount && bti.NameCount == tti.NameCount )
				break;
		}

		if( bi != Basis.GenerationTable.end() )
		{
			const int bidx = bi - Basis.GenerationTable.begin();

			// push basis item pointer
			updGenerationItem dti;
			dti.ExportCount = -bidx-1;
			GenerationTable.push_back(dti);

			// store translation
			//Tran.SetName(bidx,tidx);
		}
		else
		{
			// push delta item
			updGenerationItem dti;
			dti.ExportCount = tti.ExportCount;
			dti.NameCount = tti.NameCount;
			GenerationTable.push_back(dti);
		}
	}
}

void updPkg::CreateExports()
{
	// Basis export table
	for( uppExportTableItr it=Basis.ExportTable.begin(); it!=Basis.ExportTable.end(); ++it )
	{
		// Map
		const int tidx = it-Basis.ExportTable.begin();
		const upStrDwordMap::Insert_Result ir = BEmap.insert( upStrDwordMap::value_type( Basis.GetObjectPath(tidx+1), tidx ) );
		if( ir.second == false )
			throw upexception( wxString::Format( wxT("Basis export already mapped: %s"), Basis.GetObjectPath(tidx+1).c_str() ) );
	}

	// Target export table
	for( uppExportTableItr it=Target.ExportTable.begin(); it!=Target.ExportTable.end(); ++it )
	{
		const int tidx = it-Target.ExportTable.begin();
		const wxString tstr = Target.GetObjectPath(tidx+1);

		// Map
		const upStrDwordMap::Insert_Result ir = TEmap.insert( upStrDwordMap::value_type( tstr, tidx ) );
		if( ir.second == false )
			throw upexception( wxString::Format( wxT("Target export already mapped: %s"), tstr.c_str() ) );

		// Delta exporttable
		const uppExportItem& tti = *it;
		const upStrDwordMap::iterator bmi = BEmap.find( tstr );
		if( bmi != BEmap.end() )
		{
			const int bidx = (*bmi).second;
			const uppExportItem& bti = Basis.ExportTable UP_AT(bidx);

			if( Target.GetObjectPath(tti.Class).IsSameAs( Basis.GetObjectPath(bti.Class), true )
			&&	Target.GetObjectPath(tti.Super).IsSameAs( Basis.GetObjectPath(bti.Super), true )
			//&&	Target.GetNameString(tti.Package).IsSameAs( Basis.GetNameString(bti.Package), true )
			&&	tti.ObjectFlags == bti.ObjectFlags
			)
			{
				updExportItem dti;
				dti.ObjectName = -bidx-1;
				dti.Class = 0;
				dti.Super = 0;
				dti.Package = 0;
				dti.ObjectFlags = 0;
				dti.SerialSize = tti.SerialSize;
				dti.SerialOffset = tti.SerialOffset;
				ExportTable.push_back(dti);
				Tran.SetExport(bidx,tidx);
				//wxLogMessage( wxT("export basis: %s"), tstr.c_str() );
				continue;
			}
			else
			{
				//wxLogMessage( wxT("export similar: %s"), tstr.c_str() );
			}
		}
		else
		{
			//wxLogMessage( wxT("export target: %s"), tstr.c_str() );
		}

		updExportItem dti;
		dti.ObjectName = tti.ObjectName;
		dti.Class = tti.Class;
		dti.Super = tti.Super;
		dti.Package = tti.Package;
		dti.ObjectFlags = tti.ObjectFlags;
		dti.SerialSize = tti.SerialSize;
		dti.SerialOffset = tti.SerialOffset;
		ExportTable.push_back(dti);
		//wxLogMessage( wxT("export target: #%d %s %d"), tidx, tstr.c_str(), dti.SerialSize );
	}
}



void updPkg::CreateImports()
{
	// Basis import table
	for( uppImportTableItr it=Basis.ImportTable.begin(); it!=Basis.ImportTable.end(); ++it )
	{
		// Map
		const int tidx = it-Basis.ImportTable.begin();
		const upStrDwordMap::Insert_Result ir = BImap.insert( upStrDwordMap::value_type( Basis.GetObjectPath(-tidx-1), tidx ) );
		if( ir.second == false )
			throw upexception( wxString::Format( wxT("Basis import already mapped: %s"), Basis.GetObjectPath(-tidx-1).c_str() ) );
	}

	// Target import table
	for( uppImportTableItr it=Target.ImportTable.begin(); it!=Target.ImportTable.end(); ++it )
	{
		const int tidx = it-Target.ImportTable.begin();
		const wxString tstr = Target.GetObjectPath(-tidx-1);

		// Map
		const upStrDwordMap::Insert_Result ir = TImap.insert( upStrDwordMap::value_type( tstr, tidx ) );
		if( ir.second == false )
			throw upexception( wxString::Format( wxT("Target import already mapped: %s"), tstr.c_str() ) );

		// Delta import table
		const uppImportItem& tti = *it;
		const upStrDwordMap::iterator bmi = BImap.find( tstr );
		if( bmi != BImap.end() )
		{
			const int bidx = (*bmi).second;
			const uppImportItem& bti = Basis.ImportTable UP_AT(bidx);

			if( Target.GetNameString(tti.Class).IsSameAs( Basis.GetNameString(bti.Class), true )
			&&	Target.GetNameString(tti.ClassPackage).IsSameAs( Basis.GetNameString(bti.ClassPackage), true )
			)
			{
				updImportItem dti;
				dti.ObjectName = -bidx-1;
				dti.Package = 0;
				dti.Class = 0;
				dti.ClassPackage = 0;
				ImportTable.push_back(dti);
				Tran.SetImport(bidx,tidx);
				//wxLogMessage( wxT("import old: %s"), tstr.c_str() );
				continue;
			}
			else
			{
				//wxLogMessage( wxT("import similar: %s"), tstr.c_str() );
			}
		}
		else
		{
			//wxLogMessage( wxT("import new: %s"), tstr.c_str() );
		}

		updImportItem dti;
		dti.ObjectName = tti.ObjectName;
		dti.Package = tti.Package;
		dti.Class = tti.Class;
		dti.ClassPackage = tti.ClassPackage;
		ImportTable.push_back(dti);
	}
}



//void updPkg::CreateObjects()
//{
//	for( uppExportTableItr it=Target.ExportTable.begin(); it!=Target.ExportTable.end(); ++it )
//	{
//		const int tidx = it-Target.ExportTable.begin();
//		const int bidx = Tran.GetBasisExport(tidx);
//		const uppExportItem& tti = *it;
//		const uppObject* tobj = tti.Object;
//		if( !tobj )
//			continue;
//		
//		// delta object
//		updExportItem& dti = ExportTable UP_AT(tidx);
//		updObject* dobj = new updObject(dti);
//
//		// modified object, use delta
//		if( bidx != -1 && Basis.ExportTable UP_AT(bidx).Object )
//		{
//			// basis object
//			const uppExportItem& bti = Basis.ExportTable UP_AT(bidx);
//			const uppObject* bobj = bti.Object;
//
//			// 1. StateFrame
//			dobj->StateFrame = tobj->StateFrame;
//
//			// 2. Properties
//			dobj->Properties.Create(*bobj,*tobj,Tran);
//
//			// 3. Class Data
//		}
//
//		// new object, no delta
//		else
//		{
//			// 1. StateFrame
//			dobj->StateFrame = tobj->StateFrame;
//
//			// 2. Properties
//
//			// 3. Class Data
//		}
//
//
//
//		//const wxString tstr = Target.GetObjectPath(tidx+1);
//	}
//}

void updPkg::VerifyDelta()
{
}

void updPkg::ApplyHeader()
{
	Target.Header.Signature = uppConst::signature;
	Target.Header.PackageVersion = Header.TargetVersion;
	Target.Header.PackageFlags = Header.TargetFlags;
	Target.Header.LicenseMode = Header.TargetMode;
	Target.Header.GUID = Header.TargetGuid;
}

void updPkg::ApplyNames()
{
	// FIXME: compiler refuses to zero-initialize non-static int in release build
	static int delta = 0;

	for( updNameTableItr di=NameTable.begin(); di!=NameTable.end(); ++di )
	{
		const updNameItem& dti = *di;
		const int didx = di - NameTable.begin();
		uppNameItem& tti = Target.NameTable UP_AT(didx);

		if( dti.Name.Length() < 0 )
		{
			const int bidx = -dti.Name.Length()-1;
			const uppNameItem& bti = Basis.NameTable UP_AT(bidx);

			//wxLogMessage( wxT("delta: %d %d %d "), didx, delta, static_cast<int>(dti.Name.Length()) );

			// use basis item
			tti.Name = bti.Name;
			tti.Flags = bti.Flags;

			// set translation
			Tran.SetName(bidx,didx);
		}
		else
		{
			// use delta item
			tti.Name = dti.Name;
			tti.Flags = dti.Flags;
		}
	}
}

void updPkg::ApplyGenerations()
{
	for( updGenerationTableItr di=GenerationTable.begin(); di!=GenerationTable.end(); ++di )
	{
		const updGenerationItem& dti = *di;
		const int didx = di - GenerationTable.begin();
		uppGenerationItem& tti = Target.GenerationTable UP_AT(didx);

		if( dti.ExportCount < 0 )
		{
			const int bidx = -dti.ExportCount-1;
			const uppGenerationItem& bti = Basis.GenerationTable UP_AT(bidx);

			// use basis item
			tti.ExportCount = bti.ExportCount;
			tti.NameCount = bti.NameCount;
		}
		else
		{
			// use delta item
			tti.ExportCount = dti.ExportCount;
			tti.NameCount = dti.NameCount;
		}
	}
}

void updPkg::ApplyExports()
{
	// Phase 1: load items, setup translation
	//int delta = 0;

	for( updExportTableItr di=ExportTable.begin(); di!=ExportTable.end(); ++di )
	{
		const updExportItem& dti = *di;
		const int didx = di - ExportTable.begin();
		uppExportItem& tti = Target.ExportTable UP_AT(didx);

		if( dti.ObjectName < 0 )
		{
			const int bidx = -dti.ObjectName-1;
			const uppExportItem& bti = Basis.ExportTable UP_AT(bidx);


			// use basis item
			tti.ObjectName = dti.ObjectName;		// translation index
			//tti.Class = bti.Class;				// basis untranslated 
			//tti.Super = bti.Super;				// basis untranslated 
			//tti.Package = bti.Package;			// basis untranslated 

			tti.ObjectFlags = bti.ObjectFlags;		// basis
			tti.SerialSize = dti.SerialSize;		// delta
			tti.SerialOffset = dti.SerialOffset;	// delta


			// set translation
			Tran.SetExport(bidx,didx);
		}
		else
		{
			// use delta item
			tti.ObjectName = dti.ObjectName;
			tti.Class = dti.Class;
			tti.Super = dti.Super;
			tti.Package = dti.Package;
			tti.ObjectFlags = dti.ObjectFlags;

			tti.SerialSize = dti.SerialSize;
			tti.SerialOffset = dti.SerialOffset;
		}

		//wxLogMessage( wxT("Apply Export: %d %d %d "), didx, tti.SerialSize, tti.SerialOffset );
	}
}

void updPkg::ApplyImports()
{
	// Phase 1: load items, setup translation
	//int delta = 0;

	for( updImportTableItr di=ImportTable.begin(); di!=ImportTable.end(); ++di )
	{
		const updImportItem& dti = *di;
		const int didx = di - ImportTable.begin();
		uppImportItem& tti = Target.ImportTable UP_AT(didx);

		if( dti.ObjectName < 0 )
		{
			const int bidx = -dti.ObjectName-1;
			//const uppImportItem& bti = Basis.ImportTable UP_AT(bidx);

			// use basis item
			tti.ObjectName = dti.ObjectName; // leave delta, don't translate yet
			//tti.Package = bti.Package;
			//tti.Class = bti.Class;
			//tti.ClassPackage = bti.ClassPackage;

			// set translation
			Tran.SetImport(bidx,didx);
		}
		else
		{
			// use delta item
			tti.ObjectName = dti.ObjectName;
			tti.Package = dti.Package;
			tti.Class = dti.Class;
			tti.ClassPackage = dti.ClassPackage;
			//wxLogMessage( wxT("Apply Import: [%d] %d %d %d %d %d"), (int)(dti.ObjectName < 0), didx, tti.ObjectName, tti.Package, tti.Class, tti.ClassPackage );
		}

	}
}


void updPkg::ApplyExportsTranslation()
{
	//int delta = 0;

	for( uppExportTableItr ti=Target.ExportTable.begin(); ti!=Target.ExportTable.end(); ++ti )
	{
		uppExportItem& tti = *ti;

		if( tti.ObjectName < 0 )
		{
			//const int tidx = ti-Target.ExportTable.begin();
			const int bidx = -tti.ObjectName-1;
			const uppExportItem& bti = Basis.ExportTable UP_AT(bidx);
			
			// apply translation
			tti.ObjectName = Tran.GetTargetName( bti.ObjectName );
			tti.Class = TranslateBasisItem( bti.Class );
			tti.Super = TranslateBasisItem( bti.Super );
			tti.Package = TranslateBasisItem( bti.Package );
		}
		//wxLogMessage( wxT("EXT: [%d] #%d N:%d C:%d S:%d P:%d F:%d S:%d O:%d"), (int)(tti.ObjectName < 0), (int)(ti-Target.ExportTable.begin()), tti.ObjectName, tti.Class, tti.Super, tti.Package, tti.ObjectFlags, tti.SerialSize, tti.SerialOffset );
	}
}


void updPkg::ApplyImportsTranslation()
{
	// Phase 2: apply translation
	//int delta = 0;

	for( uppImportTableItr ti=Target.ImportTable.begin(); ti!=Target.ImportTable.end(); ++ti )
	{
		uppImportItem& tti = *ti;

		if( tti.ObjectName < 0 )
		{
			const int bidx = -tti.ObjectName-1;
			const uppImportItem& bti = Basis.ImportTable UP_AT(bidx);
			
			// apply translation
			tti.ObjectName = Tran.GetTargetName( bti.ObjectName );
			tti.Package = TranslateBasisItem( bti.Package );
			tti.Class = Tran.GetTargetName( bti.Class );
			tti.ClassPackage = Tran.GetTargetName( bti.ClassPackage );
		}
		//wxLogMessage( wxT("Apply Import: [%d] %d %d %d %d %d"), (int)(tti.ObjectName < 0), (int)(ti-Target.ImportTable.begin()), tti.ObjectName, tti.Package, tti.Class, tti.ClassPackage );
	}
}


int updPkg::TranslateBasisItem( int i ) 
{
	if( i > 0 )			
	{
		int t = Tran.GetTargetExport(i-1);	
		//wxLogMessage( wxT("Tran Export: %d %d %s %s"), i, t, Basis.GetObjectPath(i).c_str(), Target.GetObjectPath(t+1).c_str() );
	
		if( t < 0 )
			throw upexception(wxString::Format(wxT("No translation for basis export #%d"),i));
		return t+1;
	}
	else if( i < 0 )	
	{
		int t = Tran.GetTargetImport(-i-1);
		//wxLogMessage( wxT("Tran Import: %d %d %s %s"), i, t, Basis.GetObjectPath(i).c_str(), Target.GetObjectPath(-t-1).c_str() );
		if( t < 0 )
			throw upexception(wxString::Format(wxT("No translation for basis import #%d"),i));
		return -t-1;
	}
	else				
		return 0;
}


void updPkg::DumpTranBasis()
{

	for( int i=0; i!=(int)Tran.BN.size(); ++i )
	{
		int t = Tran.BN.at(i);
		if( t < 0 )
			wxLogMessage( wxT("NAME: [%d: %s] -> NO TRANSLATION"), i, Basis.GetNameString(i).c_str() );
		else
			wxLogMessage( wxT("NAME: [%d: %s] -> [%d: %s]"), i, Basis.GetNameString(i).c_str(), t, Target.GetNameString(t).c_str() );
	}

	for( int i=0; i!=(int)Tran.BE.size(); ++i )
	{
		int t = Tran.BE.at(i);
		if( t < 0 )
			wxLogMessage( wxT("EXPORT: [%d: %s] -> NO TRANSLATION"), i, Basis.GetObjectPath(i+1).c_str() );
		else
			wxLogMessage( wxT("EXPORT: [%d: %s] -> [%d: %s]"), i, Basis.GetObjectPath(i+1).c_str(), t, Target.GetObjectPath(t+1).c_str() );
	}

	for( int i=0; i!=(int)Tran.BI.size(); ++i )
	{
		int t = Tran.BI.at(i);
		if( t < 0 )
			wxLogMessage( wxT("IMPORT: [%d: %s] -> NO TRANSLATION"), i, Basis.GetObjectPath(-i-1).c_str() );
		else
			wxLogMessage( wxT("IMPORT: [%d: %s] -> [%d: %s]"), i, Basis.GetObjectPath(-i-1).c_str(), t, Target.GetObjectPath(-t-1).c_str() );
	}
}



// ============================================================================
//	The End.
// ============================================================================