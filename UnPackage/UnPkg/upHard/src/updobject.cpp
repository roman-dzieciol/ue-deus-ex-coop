// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	updObject.cpp
// ============================================================================
#include "upPrec.h"
#include "updObject.h"
#include "uppObject.h"
#include "uppTables.h"
#include "updPkg.h"

enum EDeltaStatus
{
	DS_None
,	DS_Basis
,	DS_Target
};



// ============================================================================
//	updObject
// ============================================================================

updObject::updObject( updPkg& pkg, updExportItem& item ) 
: Pkg(pkg)
, Item(item)
, ObjData(0)
, ObjInfo(0)
{
	item.Object = this;
}

updObject::~updObject()
{
	if( ObjData )
		delete ObjData;
}



void updObject::Serialize( upArchive& A, uppObject* basis, uppObject& target, updTran& tran )
{
	if( target.Item.Class != 0 )
	{
		dword lastpos = A.Tell();

		if( A.IsLoading() )
		{
			//UP_OFFSET_GUARD( obj, A, Item.SerialOffset, Item.SerialOffset + Item.SerialSize );

			// Serialize state frame	
			if( (target.Item.ObjectFlags & RF_HasStack) == RF_HasStack )
			{
				if( basis )
					target.StateFrame.SerializeDelta(A,basis->StateFrame,tran);
				else
					target.StateFrame.Serialize(A);
				//A << target.StateFrame;
			}


			if( basis )
				target.Properties.SerializeDelta(A,Pkg,basis->Properties,tran);
			else
				target.Properties.Serialize(A,Pkg.Target);

			A << ObjInfo;

			//// Load class specific data
			//if( A.Tell() < Item.DeltaOffset + Item.DeltaSize )
			//{
			//	//UP_TIMER(t);

			//	target.ObjData = target.CreateObjData();
			//	target.ObjData->SetObject(&target);
			//	target.ObjData->Serialize(A,Pkg);

			//	//UP_TIMER_STAT(t,ObjData);

			//	if( A.Tell() < Item.DeltaOffset + Item.DeltaSize )
			//	{
			//		_asm int 3;
			//	}
			//}


			// Serialize class specific data
			if( ObjInfo == DS_Target )
			{
				//UP_TIMER(t);


				target.ObjData = GObjDataFactory.Create( Pkg.Target.GetObjectName(target.Item.Class) );
				if( !target.ObjData )
				{
					target.ObjData = GObjDataFactory.Create( wxT("Unknown") );
					dword size = Item.DeltaOffset + Item.DeltaSize - A.Tell();
					static_cast<odUnknown*>(target.ObjData)->SetSize( size );
					//wxLogMessage( wxT("size %d"), size );
				}

				target.ObjData->SetObject(&target);
				target.ObjData->Serialize(A);


				//UP_TIMER_STAT(t,ObjData);

				/*if( A.Tell() < Item.SerialOffset + Item.SerialSize )
				{
					wxLogMessage( GetInfo(A) );
				}*/
			}
			else if( ObjInfo == DS_Basis )
			{
				if( basis->ObjData )
				{
					//wxLogMessage( wxT("BASIS READ: %d"), target.Item.ObjectName );
					//target.ObjData = basis->ObjData->GetTranslated( tran );

					target.ObjData = GObjDataFactory.Create( Pkg.Target.GetObjectName(target.Item.Class) );
					if( !target.ObjData )
					{
						target.ObjData = GObjDataFactory.Create( wxT("Unknown") );
						dword size = Item.DeltaOffset + Item.DeltaSize - A.Tell();
						static_cast<odUnknown*>(target.ObjData)->SetSize( size );
						//wxLogMessage( wxT("size %d"), size );
					}

					target.ObjData->SetObject(&target);
					target.ObjData->SerializeDelta(A,*basis->ObjData,tran);
				}
			}

		}
		else
		{
			// Serialize state frame	
			if( (target.Item.ObjectFlags & RF_HasStack) == RF_HasStack )
			{
				if( basis )
					target.StateFrame.SerializeDelta(A,basis->StateFrame,tran);
				else
					target.StateFrame.Serialize(A);

			}

			if( basis )
				target.Properties.SerializeDelta(A,Pkg,basis->Properties,tran);
			else
				target.Properties.Serialize(A,Pkg.Target);

			if( target.ObjData )
			{
				if( basis && basis->ObjData )
				{
					ObjInfo = DS_Basis;
					A << ObjInfo;
					target.ObjData->SerializeDelta(A,*basis->ObjData,tran);
					//wxLogMessage( wxT("DELTA WRITE: %d [%d]"), target.Item.ObjectName, int(A.Tell()-lastpos) );
				}
				else
				{
					ObjInfo = DS_Target;
					A << ObjInfo;
					target.ObjData->Serialize(A);
				}
			}
			else
			{
				ObjInfo = DS_None;
				A << ObjInfo;
			}

			//wxLogMessage( wxT("OBJ WRITE: %d, %d [%d] [%.8xd]"), ObjInfo, target.Item.ItemIndex, int(A.Tell()-lastpos), lastpos );
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