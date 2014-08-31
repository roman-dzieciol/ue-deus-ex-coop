/*=============================================================================
	RpLineBatcher.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/
#pragma once




/*-----------------------------------------------------------------------------
	FRpLineBatcher.
-----------------------------------------------------------------------------*/


struct RPCOOP_API FRpLine
{
	FVector End;
	FVector Start;
	FColor Color;

	FRpLine( const FVector& end, const FVector& start, const FColor& color )
		: End(end), Start(start), Color(color){}
};

struct RPCOOP_API FRpLineBatcher
{
	TArray<FRpLine> Lines;
	bool bEnabled;

	FRpLineBatcher() : bEnabled(true)
	{
	}

	void Toggle()
	{
		bEnabled = !bEnabled;
	}

	void Add( const FRpLine& line ) { Lines.AddItem(line); }
	void Add( const FVector& end, const FVector& start, BYTE r, BYTE g, BYTE b ) { Lines.AddItem(FRpLine(end,start,FColor(r,g,b))); }
	void Empty() { Lines.Empty(); }
	
	void Render( FSceneNode* Frame )
	{
		guard(FRpLineBatcher::Render);

		if( !bEnabled )
			return;

		static float lw = 4;

		for( TArray<FRpLine>::TIterator Itb(GLineBatcher.Lines); Itb; ++Itb )
		{
			FVector vr = VRand();
			FVector start = (*Itb).Start+vr;
			FVector end = (*Itb).End+vr;
			GRenderDevice->Draw3DLine( Frame, (*Itb).Color.Plane(), LINE_DepthCued, end, start );
			GRenderDevice->Draw3DLine( Frame, (*Itb).Color.Plane(), LINE_DepthCued, start+FVector(-1,0,0)*lw, start+FVector(1,0,0)*lw );
			GRenderDevice->Draw3DLine( Frame, (*Itb).Color.Plane(), LINE_DepthCued, start+FVector(0,-1,0)*lw, start+FVector(0,1,0)*lw );
			GRenderDevice->Draw3DLine( Frame, (*Itb).Color.Plane(), LINE_DepthCued, start+FVector(0,0,-1)*lw, start+FVector(0,0,1)*lw );
		}

		unguard;
	}
};



/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
