// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	updTran.h
// ============================================================================
#ifndef UPDTRAN_H
#define UPDTRAN_H

#include "uppPkg.h"


// ============================================================================
// updTran
// ============================================================================
class updTran
{
private:
	vector<int> BN, TN;
	vector<int> BE, TE;
	vector<int> BI, TI;

public:
	typedef bool (updTran::*updTranCmp)( int a, int b ) const;

public:
	updTran( uppPkg& basis, uppPkg& target )
	{
		Reset(basis, target);
	}

	void Reset( uppPkg& basis, uppPkg& target )
	{
		Init( BN, basis.NameTable.size() );
		Init( BE, basis.ExportTable.size() );
		Init( BI, basis.ImportTable.size() );
		Init( TN, target.NameTable.size() );
		Init( TE, target.ExportTable.size() );
		Init( TI, target.ImportTable.size() );
	}

	void Verify();

public:
	bool CompareNameB( int a, int b ) const		{ return BN.at(a) == b; }
	bool CompareExportB( int a, int b ) const	{ return BE.at(a) == b; }
	bool CompareImportB( int a, int b ) const	{ return BI.at(a) == b; }

	bool CompareNameT( int a, int b ) const		{ return TN.at(a) == b; }
	bool CompareExportT( int a, int b ) const	{ return TE.at(a) == b; }
	bool CompareImportT( int a, int b ) const	{ return TI.at(a) == b; }
	
	bool CompareObjectB( int a, int b ) const
	{
		if( a > 0 )			return BE.at(a-1) == b-1;
		else if( a < 0 )	return BI.at(-a-1) == -b-1;
		else				return a == b;
	}

	bool CompareObjectT( int a, int b ) const
	{
		if( a > 0 )			return BE.at(a-1) == b-1;
		else if( a < 0 )	return BI.at(-a-1) == -b-1;
		else				return a == b;
	}

public:
	int GetTargetName( int basis ) const { return BN UP_AT(basis); }
	int GetTargetExport( int basis ) const { return BE UP_AT(basis); }
	int GetTargetImport( int basis ) const { return BI UP_AT(basis); }

	int GetBasisName( int target ) const { return TN UP_AT(target); }
	int GetBasisExport( int target ) const { return TE UP_AT(target); }
	int GetBasisImport( int target ) const { return TI UP_AT(target); }

	int GetTargetItem( int basis ) const
	{
		if( basis > 0 )			return GetTargetExport(basis-1)+1;
		else if( basis < 0 )	return -GetTargetImport(-basis-1)-1;
		else					return 0;
	}

	int GetBasisItem( int target ) const
	{
		if( target > 0 )		return GetBasisExport(target-1)+1;
		else if( target < 0 )	return -GetBasisImport(-target-1)-1;
		else					return 0;
	}

	//

public:
	bool HasTargetName( int basis ) const { return (BN UP_AT(basis) != -1); }
	bool HasTargetExport( int basis ) const { return (BE UP_AT(basis) != -1); }
	bool HasTargetImport( int basis ) const { return (BI UP_AT(basis) != -1); }

	bool HasBasisName( int target ) const { return (TN UP_AT(target) != -1); }
	bool HasBasisExport( int target ) const { return (TE UP_AT(target) != -1); }
	bool HasBasisImport( int target ) const { return (TI UP_AT(target) != -1); }

	bool HasTargetItem( int basis ) const
	{
		if( basis > 0 )			return HasTargetExport(basis-1);
		else if( basis < 0 )	return HasTargetImport(-basis-1);
		else					return true;
	}

	bool HasBasisItem( int target ) const
	{
		if( target > 0 )		return HasBasisExport(target-1);
		else if( target < 0 )	return HasBasisImport(-target-1);
		else					return true;
	}

private:
	void Init( vector<int>& ref, size_t size, int def=-1 )
	{
		ref.reserve( size );
		ref.resize( size, def );
	}

	void DumpBasis();
	void DumpTarget();
	
private:
	friend class uppDiff;
	friend class updPkg;

	void SetName( int basis, int target )
	{
		BN UP_AT(basis) = target;
		TN UP_AT(target) = basis;
	}

	void SetExport( int basis, int target )
	{
		BE UP_AT(basis) = target;
		TE UP_AT(target) = basis;
	}

	void SetImport( int basis, int target )
	{
		BI UP_AT(basis) = target;
		TI UP_AT(target) = basis;
	}
};

//
//// ============================================================================
//// upfCmpIndexArray
//// ============================================================================
//template<class T>
//inline void upfCmpIndexArray( size_t count, T& basis, T& target, updTran& tran, uppBitVector& diff, updTran::updTranCmp cmpfunc )
//{
//	for( size_t i=0; i!=count; ++i )
//	{
//		if( !UP_CALL_MEMBER_FN(tran,cmpfunc)(basis[i],target[i]) )
//		{
//			diff.push_back( false );
//			return;
//		}
//	}
//	diff.push_back( true );
//};
//
//// ============================================================================
//// upfCmpIndexVector
//// ============================================================================
//template<class T>
//inline void upfCmpIndexVector( vector<T>& basis, vector<T>& target, updTran& tran, uppBitVector& diff, updTran::updTranCmp cmpfunc )
//{
//	if( basis.size() != target.size() )
//	{
//		diff.push_back( false );
//		return;
//	}
//
//	for( size_t i=0; i!=count; ++i )
//	{
//		if( !UP_CALL_MEMBER_FN(tran,cmpfunc)(basis UP_AT(i),target UP_AT(i)) )
//		{
//			diff.push_back( false );
//			return;
//		}
//	}
//	diff.push_back( true );
//};
//
//
//
//// ============================================================================
//// Macros
//// ============================================================================
//#define UP_DIFF_NAME(name,tran,diff,to)		diff.push_back( tran.CompareNameB(name,to.name) )
//#define UP_DIFF_OBJECT(name,tran,diff,to)	diff.push_back( tran.CompareObjectB(name,to.name) )
//#define UP_DIFF_VALUE(name,tran,diff,to)	diff.push_back( name == to.name )
//
//#define UP_DIFF_ARRAY(name,size,func,tran,diff,to)	upfCmpIndexArray( size, name, to.name, tran, diff, func );
//#define UP_DIFF_VECTOR(name,func,tran,diff,to)		upfCmpIndexVector( size, name, to.name, tran, diff, func );


#endif UPDTRAN_H
// ============================================================================
//	The End.
// ============================================================================