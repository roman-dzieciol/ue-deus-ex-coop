# Microsoft Developer Studio Project File - Name="RpCoop" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RpCoop - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RpCoop.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RpCoop.mak" CFG="RpCoop - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RpCoop - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RpCoop - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Rune/RpCoop", CUDBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RpCoop - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RpCoop_EXPORTS" /YX /FD /c
# ADD CPP /nologo /Zp4 /MD /W3 /vd0 /GX /O2 /I "..\Core\Inc" /I "..\Engine\Inc" /I "..\Render\Src" /I "..\Extension\Inc" /I "..\DeusEx\Inc" /I ".\Inc" /I "D:\proj\game tools\UnPackage\UnPkg\RpAr" /D "NDEBUG" /D ThisPackage=RpCoop /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR /FD /c
# SUBTRACT CPP /WX /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /incremental:yes /machine:I386 /out:"..\System\RpCoop.dll" /libpath:"..\System"

!ELSEIF  "$(CFG)" == "RpCoop - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RpCoop___Win32_Debug"
# PROP BASE Intermediate_Dir "RpCoop___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RpCoop___Win32_Debug"
# PROP Intermediate_Dir "RpCoop___Win32_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RpCoop_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /Zp4 /MDd /W4 /WX /vd0 /GX /Zi /Od /I "..\Core\Inc" /I "..\Engine\Inc" /I "Inc" /D "_DEBUG" /D ThisPackage=RpCoop /D "WIN32" /D "_WINDOWS" /D "UNICODE" /D "_UNICODE" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\Core\Lib\Core.lib ..\Engine\Lib\Engine.lib /nologo /dll /debug /machine:I386 /out:"..\System\RpCoop.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RpCoop - Win32 Release"
# Name "RpCoop - Win32 Debug"
# Begin Group "ref"

# PROP Default_Filter ""
# Begin Group "Core"

# PROP Default_Filter "*.*"
# Begin Source File

SOURCE=..\Core\Inc\Core.h
# End Source File
# Begin Source File

SOURCE=..\Core\Src\CorePrivate.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\DbgInfoCpp.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FCodec.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FConfigCacheIni.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FFeedbackContextAnsi.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FFeedbackContextWindows.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FFileManagerAnsi.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FFileManagerGeneric.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FFileManagerLinux.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FFileManagerWindows.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FMallocAnsi.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FMallocDebug.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FMallocWindows.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FOutputDeviceAnsiError.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FOutputDeviceFile.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FOutputDeviceNull.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FOutputDeviceStdout.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FOutputDeviceWindowsError.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\FRiffChunk.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\SerialNumber.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UDebugInfo.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UExporter.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UFactory.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnArc.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnBits.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnBuild.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnCache.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnCId.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnClass.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnCoreNet.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnCorObj.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnFile.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnGnuG.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnLinker.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnMath.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnMem.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnName.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnNames.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\unobjbas.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnObjVer.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnScript.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnStack.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnTemplate.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnType.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnUnix.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnVcWin32.h
# End Source File
# Begin Source File

SOURCE=..\Core\Inc\UnVcWn32SSE.h
# End Source File
# End Group
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Engine\Inc\AActor.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\ABrush.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\ACamera.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\AGameReplicationInfo.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\AInventory.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Src\Amd3d.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\AMover.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\APawn.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\APlayerPawn.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\APlayerReplicationInfo.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\AZoneInfo.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\Engine.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\EngineClasses.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Src\EnginePrivate.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\Palette.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\S3tc.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\ULevelSummary.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnActor.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnAudio.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnBunch.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnCamera.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnChan.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnCon.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnConn.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnDDraw.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnDemoPenLev.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnDemoRec.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnDynBsp.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnEngine.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnEngineGnuG.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnEngineWin.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnEventManager.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnGame.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnIn.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnLevel.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnMesh.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnModel.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnNet.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnNetDrv.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnObj.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Src\UnPath.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnPenLev.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnPlayer.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnPrim.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnReach.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnRender.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnRenderIterator.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnRenDev.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnScrTex.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnTex.h
# End Source File
# Begin Source File

SOURCE=..\Engine\Inc\UnURL.h
# End Source File
# End Group
# Begin Group "Extension"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Extension\Inc\ExtBorder.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtButton.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtCheckbox.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtClipWindow.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtComputerWindow.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtEdit.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\Extension.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtensionCore.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtFlag.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtFlagBase.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtGameEngine.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtGC.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtInput.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtLargeText.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtList.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtModal.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtObject.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtPlayerPawn.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtRadioBox.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtRoot.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtScale.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtScaleManager.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtScrollArea.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtString.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtTabGroup.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtText.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtTextLog.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtTile.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtToggle.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtViewport.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtWindow.h
# End Source File
# Begin Source File

SOURCE=..\Extension\Inc\ExtWindowCore.h
# End Source File
# End Group
# Begin Group "DeusEx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\DeusEx\Inc\AAugmentation.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\AAugmentationManager.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\ADeusExDecoration.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\ADeusExLevelInfo.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\ADeusExPlayer.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\AScriptedPawn.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\DeusEx.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\DeusExClasses.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\DeusExGameEngine.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\ExtGameDirectory.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\UDataVaultImageNote.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\UDeusExLog.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\UDeusExSaveInfo.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\UDumpLocation.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\ULaserIterator.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\uparticle.h
# End Source File
# Begin Source File

SOURCE=..\DeusEx\Inc\UParticleIterator.h
# End Source File
# End Group
# Begin Group "Render"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Render\Src\Amd3d.h
# End Source File
# Begin Source File

SOURCE=..\Render\Src\RenderPrivate.h
# End Source File
# Begin Source File

SOURCE=..\Render\Src\UnMeshRnLOD.h
# End Source File
# Begin Source File

SOURCE=..\Render\Src\UnSpan.h
# End Source File
# End Group
# Begin Group "IpDrv"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\IpDrv\Inc\AInternetLink.h
# End Source File
# Begin Source File

SOURCE=..\IpDrv\Inc\ATcpLink.h
# End Source File
# Begin Source File

SOURCE=..\IpDrv\Inc\AUdpLink.h
# End Source File
# Begin Source File

SOURCE=..\IpDrv\Inc\GameSpyClasses.h
# End Source File
# Begin Source File

SOURCE=..\IpDrv\Inc\GameSpyClassesPublic.h
# End Source File
# Begin Source File

SOURCE=..\IpDrv\Inc\IpDrvClasses.h
# End Source File
# Begin Source File

SOURCE=..\IpDrv\Src\IpDrvPrivate.h
# End Source File
# Begin Source File

SOURCE=..\IpDrv\Src\UnSocket.h
# End Source File
# End Group
# End Group
# Begin Group "Inc"

# PROP Default_Filter ""
# Begin Group "Inc Detours"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inc\RpDetours.h
# End Source File
# Begin Source File

SOURCE=.\Inc\RpDtMacros.h
# End Source File
# End Group
# Begin Group "Inc Script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inc\RpCoopClasses.h
# End Source File
# Begin Source File

SOURCE=.\Inc\RpCoopScript.h
# End Source File
# End Group
# Begin Group "Inc Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inc\RpLineBatcher.h
# End Source File
# Begin Source File

SOURCE=.\Inc\RpToStr.h
# End Source File
# Begin Source File

SOURCE=.\Inc\RpUtil.h
# End Source File
# End Group
# Begin Group "Inc Overrides"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Inc\RpFileTransfer.h
# End Source File
# Begin Source File

SOURCE=.\Inc\RpSaveGame.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Inc\RpCanvas.h
# End Source File
# Begin Source File

SOURCE=.\Inc\RpCoop.h
# End Source File
# Begin Source File

SOURCE=.\Inc\RpGameEngine.h
# End Source File
# End Group
# Begin Group "Src"

# PROP Default_Filter ""
# Begin Group "Src Detours "

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\RpDetours.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtDDeusExEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtExtWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtFFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtFNetworkNotify.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtUChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtUEntryLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtUFileChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtULevel.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtUNetConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtUNetDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpDtUObject.cpp
# End Source File
# End Group
# Begin Group "Src Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\RpToStr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpUtil.cpp
# End Source File
# End Group
# Begin Group "Src Overrides"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\RpFileTransfer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpSaveGame.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\RpCanvas.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpCoop.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\RpGameEngine.cpp
# End Source File
# End Group
# Begin Group "Script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Classes\RpCanvas.uc
# End Source File
# End Group
# End Target
# End Project
