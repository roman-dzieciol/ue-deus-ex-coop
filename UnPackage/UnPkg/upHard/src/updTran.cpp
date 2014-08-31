// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	updTran.cpp
// ============================================================================
#include "upPrec.h"
#include "updTran.h"




// ============================================================================
//	updTran
// ============================================================================
/*updTran::updTran( uppPkg& basis, uppPkg& target )
{
}*/
void updTran::Verify()
{
}

void updTran::DumpBasis()
{

	for( int i=0; i!=(int)BN.size(); ++i )
	{
		wxLogMessage( wxT("BASIS NAME[%d]:\t%d"), i, BN[i] );
	}

	for( int i=0; i!=(int)BE.size(); ++i )
	{
		wxLogMessage( wxT("BASIS EXPORT[%d]:\t%d"), i, BE[i] );
	}

	for( int i=0; i!=(int)BI.size(); ++i )
	{
		wxLogMessage( wxT("BASIS IMPORT[%d]:\t%d"), i, BI[i] );
	}
}

void updTran::DumpTarget()
{

	for( int i=0; i!=(int)TN.size(); ++i )
	{
		wxLogMessage( wxT("TARGET NAME[%d]:\t%d"), i, TN[i] );
	}

	for( int i=0; i!=(int)TE.size(); ++i )
	{
		wxLogMessage( wxT("TARGET EXPORT[%d]:\t%d"), i, TE[i] );
	}

	for( int i=0; i!=(int)TI.size(); ++i )
	{
		wxLogMessage( wxT("TARGET IMPORT[%d]:\t%d"), i, TI[i] );
	}
}


// ============================================================================
//	The End.
// ============================================================================