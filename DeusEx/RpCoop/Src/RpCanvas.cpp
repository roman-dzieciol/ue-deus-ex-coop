// ============================================================================
//	Rp.cpp
//	Copyright 2007 Roman Dzieciol. All Rights Reserved.
// ============================================================================

// Includes.
#include "RpCoop.h"


void URpCanvas::Update( FSceneNode* Frame )
{
	rpguard(URpCanvas::Update);

	UCanvas::Update( Frame );

	if( !Frame )	return;

	GLineBatcher.Render( Frame );

	/*if( !Render )	return;
	if( !Viewport )	return;
	if( !Viewport->RenDev )	return;
	if( !Frame->Level )	return;
	if( !Frame->Level->Model )	return;*/

	unguard;
}




// ============================================================================
//  EOF
// ============================================================================