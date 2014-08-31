// RpArCl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if( argc == 5 )
	{
		if( _tcscmp(argv[1],_T("p")) == 0 )
		{
			int err = RpPack(argv[2],argv[3],argv[4],0);
			if( RpIsError(err) )
			{
				printf( "error: %d", err );
			}
		}
		else if( _tcscmp(argv[1],_T("u")) == 0 )
		{
			int err = RpUnpack(argv[2],argv[3],argv[4],0);
			if( RpIsError(err) )
			{
				printf( "error: %d", err );
			}
		}
	}

	return 0;
}

