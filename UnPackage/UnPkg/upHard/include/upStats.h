// ============================================================================
//  UnPackage :: Cross-platform toolkit for Unreal Engine packages
//  Copyright (C) 2006  Roman Dzieciol. All Rights Reserved.
// ============================================================================
//	upStats.h
// ============================================================================
#ifndef UPSTATS_H
#define UPSTATS_H




// ============================================================================
// upStats
// ============================================================================
class upStats
{
public:
    static upStats& Global();
    #define UPG_Stats upStats::Global() 
	
public:
	float ObjProp;
	float ObjData;
	int NewProps;
	int OldProps;

public:
    upStats()
	{
		Reset();
	}

	void Reset()
	{
		ObjProp = 0;
		ObjData = 0;
		NewProps = 0;
		OldProps = 0;
	}

	wxString c_str()
	{
		wxString S(wxT("STATS:"));
		//S += wxString::Format( wxT(" ObjProp=%.2f"), ObjProp );
		//S += wxString::Format( wxT(" ObjData=%.2f"), ObjData );
		S += wxString::Format( wxT(" NewProps=%d"), NewProps );
		S += wxString::Format( wxT(" OldProps=%d"), OldProps );
		return S;
	}
};



#endif UPSTATS_H
// ============================================================================
//	The End.
// ============================================================================