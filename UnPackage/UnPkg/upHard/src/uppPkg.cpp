// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppPkg.cpp
// ============================================================================
#include "upPrec.h"
#include "uppPkg.h"
#include "uppObject.h"


// ============================================================================
//	uppPkg
// ============================================================================
uppPkg::uppPkg()
{    
}

uppPkg::~uppPkg()
{
}

wxString uppPkg::DebugInfo()
{
	wxString S;
	return S;
}

void uppPkg::Serialize( upArchive& A )
{
	//UP_TIMER(t);

	if( A.IsLoading() )
	{
		A << BI_Header;
		A << Header;
		A << static_cast<uppTableHeader&>(NameTable);
		A << static_cast<uppTableHeader&>(ExportTable);
		A << static_cast<uppTableHeader&>(ImportTable);
		A << Header.GUID;
		A << BI_GenerationTable << GenerationTable;

		// Notice the order
		A << BI_NameTable << NameTable;
		A << BI_ImportTable << ImportTable;
		A << BI_ExportTable << ExportTable;

		A << BI_Object;
		for( uppExportTableItr it=ExportTable.begin(); it!=ExportTable.end(); ++it )
		{
			uppExportItem& item = (*it);
			if( item.SerialOffset != 0 )
			{
				A.Seek( item.SerialOffset );
				uppObject* obj = new uppObject(*this,item);
				obj->Serialize(A);
			}
		}

		A << BI_None;
	}
	else
	{
		A << Header;
		wxFileOffset headers = A.Tell();
		A << static_cast<uppTableHeader&>(NameTable);
		A << static_cast<uppTableHeader&>(ExportTable);
		A << static_cast<uppTableHeader&>(ImportTable);
		A << Header.GUID;
		A << GenerationTable;

		// Notice the order
		A << NameTable;

		//dword count = 0x30e0db - A.Tell();
		//vector<byte> v;
		//v.reserve(count);
		//v.resize(count,0);
		//A.Serialize( &v[0], count ); // obj

		for( uppExportTableItr it=ExportTable.begin(); it!=ExportTable.end(); ++it )
		{
			uppExportItem& item = (*it);

			//wxLogMessage( wxT("Object: #%d S:%d O:%xd R:%xd"), (int)(it-ExportTable.begin()), item.SerialSize, item.SerialOffset, A.Tell() );

			if( item.Object )
			{
				item.SerialOffset = A.Tell();
				item.Object->Serialize(A);
				item.SerialSize = A.Tell()-item.SerialOffset;
				//vector<byte> buffer;
				//buffer.resize( item.SerialSize, 0 );
				//A.Serialize( &buffer[0], item.SerialSize );
			}
			else
			{
				item.SerialOffset = 0;
				item.SerialSize = 0;
			}
		}

		A << ImportTable;
		A << ExportTable;

		// Table headers are incomplete before this point
		wxFileOffset resume = A.Tell();
		A.Seek(headers);
		A << static_cast<uppTableHeader&>(NameTable);
		A << static_cast<uppTableHeader&>(ExportTable);
		A << static_cast<uppTableHeader&>(ImportTable);
		A.Seek(resume);
	}
	//UP_TIMER_LOGF(t,0);
}

// ============================================================================
//	The End.
// ============================================================================