/*=============================================================================
	RpDt.h
	Copyright 2006 Roman Dzieciol. All Rights Reserved.
=============================================================================*/

// Includes.
#include "RpCoop.h"
#include "RpDtMacros.h"



RP_DETOUR_CLASS( UChannel, void, Init, ( UNetConnection* InConnection, INT InChIndex, UBOOL InOpenedLocally ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s],[%d],[%d]"), RP_LOGP, *ToStr(InConnection,true),InChIndex,InOpenedLocally  );
	RP_DETOUR_TP( Init(InConnection,InChIndex,InOpenedLocally) );
	UChannel* c = InConnection->Channels[InChIndex];
	
	RP_LOG( RP_NAME, TEXT("%s << %s :: [%s],[%d],[%d] [%s]"), RP_LOGP, *ToStr(InConnection,true),InChIndex,InOpenedLocally, *ToStr(c)  );
}}

RP_DETOUR_CLASS( UChannel, void, SetClosingFlag, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( SetClosingFlag() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UChannel, void, Close, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( Close() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UChannel, FString, Describe, () )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	FString r = RP_DETOUR_TP( Describe() );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
	return r;
}}

RP_DETOUR_CLASS( UChannel, void, ReceivedBunch, ( FInBunch& Bunch ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( ReceivedBunch(Bunch) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS( UChannel, void, ReceivedNak, ( INT NakPacketId ) )
{
	RP_LOG( RP_NAME, TEXT("%s >> %s :: "), RP_LOGP);
	RP_DETOUR_TP( ReceivedNak(NakPacketId) );
	RP_LOG( RP_NAME, TEXT("%s << %s :: "), RP_LOGP );
}}

RP_DETOUR_CLASS_SUPER( UFileChannel, void, Tick, () )
{
	UFileChannel* self = static_cast<UFileChannel*>(this);

	// Update temporarily PackageMap's FileSize as it contains the uncompressed length
	// This way it won't try to send uncompressed filesize amount of bytes

	/*if( self && self->IsValid()
	&& self->FileAr
	&& self->Connection && self->Connection->IsValid()
	&& self->Connection->PackageMap && self->Connection->PackageMap->IsValid() )
	{
		RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(self));

		int pidx = self->PackageIndex;
		UPackageMap* pkgmap = self->Connection->PackageMap;
		FPackageInfo& info = pkgmap->List(pidx);
		int realsize = info.FileSize;
		int compsize = self->FileAr->TotalSize();
		RP_LOG( RP_NAME, TEXT("%s -- %s :: [%d] [%d]"), RP_LOGP, realsize, compsize);

		info.FileSize = compsize;
		RP_DETOUR_TP( Tick() );
		if( pkgmap && pkgmap->IsValid() && pkgmap->List.Num() > pidx && pkgmap->List(pidx).URL == info.URL )
		{
			info.FileSize = realsize;
		}
		RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(self));
	}
	else*/
	{
		/*RP_LOG( RP_NAME, TEXT("%s -- %s :: [%s] [%d][%d][%p][%d][%d]"), RP_LOGP
		, *ToStr(self)
		, self->Connection->PackageMap->List(self->PackageIndex).FileSize
		, (int)self->OpenedLocally
		, self->FileAr
		, (int)self->IsNetReady(0)
		, (int)(self->MaxSendBytes())
		);*/
		//RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(self));
		//RP_DETOUR_TP( Tick() );
		//RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(self) );
		//RP_LOG( RP_NAME, TEXT("%s -- %s :: [%s] [%d]"), RP_LOGP, *ToStr(self), self->Connection->PackageMap->List(self->PackageIndex).FileSize );
	}


	UChannel::Tick();
	Connection->TimeSensitive = 1;
	INT Size;
	//TIM: IsNetReady(1) causes the client's bandwidth to be saturated. Good for clients, very bad
	// for bandwidth-limited servers. IsNetReady(0) caps the clients bandwidth.
	static UBOOL LanPlay = ParseParam(appCmdLine(),TEXT("lanplay"));
	while( !OpenedLocally && FileAr && IsNetReady(LanPlay) && (Size=MaxSendBytes())!=0 )
	{
		// Sending.
		INT Remaining = Connection->PackageMap->List(PackageIndex).FileSize-Transfered;
		FOutBunch Bunch( this, Size>=Remaining );
		Size = Min( Size, Remaining );
		BYTE* Buffer = (BYTE*)appAlloca( Size );
		FileAr->Serialize( Buffer, Size );
		if( FileAr->IsError() )
		{
			//!!
		}
		Transfered += Size;
		Bunch.Serialize( Buffer, Size );
		Bunch.bReliable = 1;
		check(!Bunch.IsError());
		SendBunch( &Bunch, 0 );
		Connection->FlushNet();
		if( Bunch.bClose )
		{
			// Finished.
			delete FileAr;
			FileAr = NULL;
		}
	}
}}

RP_DETOUR_CLASS_SUPER( UFileChannel, void, Destroy, () )
{
	UFileChannel* self = static_cast<UFileChannel*>(this);

	// Update temporarily PackageMap's FileSize as it contains the uncompressed length
	// This is for verification of downloaded file size

	/*if( self && self->IsValid()
	&& self->FileAr
	&& self->Connection && self->Connection->IsValid()
	&& self->Connection->PackageMap && self->Connection->PackageMap->IsValid() )
	{
		RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s]"), RP_LOGP, *ToStr(self));
		int pidx = self->PackageIndex;
		UPackageMap* pkgmap = self->Connection->PackageMap;
		FPackageInfo& info = pkgmap->List(pidx);
		int realsize = info.FileSize;
		int compsize = self->FileAr->TotalSize();
		RP_LOG( RP_NAME, TEXT("%s -- %s :: [%d] [%d]"), RP_LOGP, realsize, compsize);

		// TODO: Verify downloaded file integrity

		info.FileSize = compsize;
		RP_DETOUR_TP( Destroy() );
		if( pkgmap && pkgmap->IsValid() && pkgmap->List.Num() > pidx && pkgmap->List(pidx).URL == info.URL )
		{
			info.FileSize = realsize;
		}
		RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(self) );
	}
	else*/
	{

		RP_LOG( RP_NAME, TEXT("%s >> %s :: [%s][%d]"), RP_LOGP, *ToStr(self), self->Connection->PackageMap->List(self->PackageIndex).FileSize);
		RP_DETOUR_TP( Destroy() );
		RP_LOG( RP_NAME, TEXT("%s << %s :: [%s]"), RP_LOGP, *ToStr(self) );
	}

}}


void RpDtUFileChannel( UFileChannel* p )
{
	rpguard(RpDtUFileChannel);
	RP_LOG( RP_NAME, TEXT("%s ## %s :: [%s]"), RP_LOGP, *ToStr(p) );
	if( p == NULL )
		return;

	PBYTE UChannel = PBYTE(p);
	//RP_DETOUR_VR( UChannel, 0x00, 0x54, Init );
	//RP_DETOUR_VR( UChannel, 0x00, 0x58, SetClosingFlag );
	//RP_DETOUR_VR( UChannel, 0x00, 0x5c, Close );
	//RP_DETOUR_VR( UChannel, 0x00, 0x60, Describe );
	//RP_DETOUR_VR( UChannel, 0x00, 0x64, ReceivedBunch );
	//RP_DETOUR_VR( UChannel, 0x00, 0x68, ReceivedNak );
	//RP_DETOUR_VR( UChannel, 0x00, 0x6C, Tick );

	PBYTE UFileChannel = PBYTE(p);
	//RP_DETOUR_VR( UFileChannel, 0x00, 0x24, Destroy );
	//RP_DETOUR_VR( UFileChannel, 0x00, 0x6C, Tick );


	unguard;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/