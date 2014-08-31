// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	uppObject.cpp
// ============================================================================
#include "upPrec.h"
#include "uppObject.h"


// ============================================================================
//	uppObject
// ============================================================================


wxString uppObject::GetInfo( upArchive& A )
{
	wxString S;
	S += wxString::Format( wxT(" <0x%.8x-0x%.8x>"), static_cast<dword>(Item.SerialOffset), static_cast<dword>(Item.SerialOffset+Item.SerialSize) );
	S += wxString::Format( wxT(" 0x%.8x"), static_cast<dword>(A.Tell()) );
	S += wxString::Format( wxT(" %s"), Pkg.GetNameString(Item.ObjectName).c_str() );
	S += wxString::Format( wxT(" %s"), Pkg.GetObjectName(Item.Class).c_str() );
	return S;
}



void uppObject::Serialize( upArchive& A )
{
	if( Item.Class != 0 )
	{
		if( A.IsLoading() )
		{
			//UP_OFFSET_GUARD( obj, A, Item.SerialOffset, Item.SerialOffset + Item.SerialSize );

			// Read state frame	
			if( (Item.ObjectFlags & RF_HasStack) == RF_HasStack )
			{
				A << StateFrame;
			}

			Properties.Serialize(A,Pkg);

			// Load class specific data
			if( A.Tell() < Item.SerialOffset + Item.SerialSize )
			{
				//UP_TIMER(t);

				ObjData = GObjDataFactory.Create( Pkg.GetObjectName(Item.Class) );
				if( !ObjData )
				{
					dword size = Item.SerialOffset + Item.SerialSize - A.Tell();
					ObjData = GObjDataFactory.Create( wxT("Unknown") );
					static_cast<odUnknown*>(ObjData)->SetSize( size );
					//wxLogMessage( wxT("size %d"), size );
				}
				ObjData->Serialize(A);

				//UP_TIMER_STAT(t,ObjData);

				if( A.Tell() < Item.SerialOffset + Item.SerialSize )
				{
					wxLogMessage( GetInfo(A) );
				}
			}

		}
		else
		{
			// Read state frame	
			if( (Item.ObjectFlags & RF_HasStack) == RF_HasStack )
			{
				A << StateFrame;
			}
			//else _asm int 3;

			Properties.Serialize(A,Pkg);

			if( ObjData )
			{
				ObjData->Serialize(A);
			}
		}
	}
	else
	{
		throw upexception(wxT("Embedded classes are unsupported"));
	}

	
}


// ============================================================================
//	The End.
// ============================================================================