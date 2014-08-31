// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upFileBuffer.cpp
// ============================================================================
#include "upPrec.h"
#include "upFileBuffer.h"


// ============================================================================
//	upFileBuffer
// ============================================================================
void upFileBuffer::DumpByteInfo()
{
	//wxLogMessage( wxT("ByteInfo:") );

	//if( !Markers )
	//	return;

	int from = -1;
	int to = from;
	EByteInfo lastinfo = static_cast<EByteInfo>(Markers[0]);

	for( dword i=0; i!=Data.size(); ++i )
	{
		if( Markers[i] == lastinfo )
		{
			if( from != -1 )
			{
				++to;
			}
			else
			{
				from = i;
				to = from;
			}
		}
		else
		{
			if( from != -1 )
			{
				wxLogMessage( wxString::Format( wxT("<0x%.8x 0x%.8x> %s")
				, static_cast<dword>(from) 
				, static_cast<dword>(to) 
				, upGetByteInfoName(lastinfo).c_str()
				) );
				lastinfo = static_cast<EByteInfo>(Markers[i]);
				from = -1;
				to = from;
			}
		}
	}

	if( from != -1 )
	{
		wxLogMessage( wxString::Format( wxT("<0x%.8x 0x%.8x> %s")
		, static_cast<dword>(from) 
		, static_cast<dword>(to) 
		, upGetByteInfoName(lastinfo).c_str()
		) );
		from = -1;
		to = from;
	}

	wxLogMessage( wxString::Format( wxT("<0x%.8x 0x%.8x> EOF")
	, static_cast<dword>(0) 
	, static_cast<dword>(Size) 
	) );
}




// ============================================================================
//	The End.
// ============================================================================