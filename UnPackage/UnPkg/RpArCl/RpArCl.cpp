// RpArCl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if( argc == 6 )
	{
		if( _tcscmp(argv[1],_T("p")) == 0 )
		{
			int flags = _wtoi(argv[5]);
			int err = RpPack(argv[2],argv[3],argv[4],flags);
			if( RpIsError(err) )
			{
				printf( "error: %d", err );
			}
		}
		else if( _tcscmp(argv[1],_T("u")) == 0 )
		{
			int flags = _wtoi(argv[5]);
			int err = RpUnpack(argv[2],argv[3],argv[4],flags);
			if( RpIsError(err) )
			{
				printf( "error: %d", err );
			}
		}
	}

	return 0;
}

