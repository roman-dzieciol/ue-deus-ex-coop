=======================================================================
         Restoration         Version 2.5.14         5/14/2002
=======================================================================

[Description]
 Restore files which are deleted from the recycle bin or deleted while
 holding down the Shift key by mistake.
 Conversely, this program has another function that makes it almost
 impossible to restore all deleted files.
 You can use it after deletion of confidential documents, embarrassing
 files and so on.

 You don't have to install it, so it doesn't leave any garbage in the
 PC.
 (You can also run it from a floppy disk.)
 Even if a file seems to be restored, there is no guarantee that the
 contents are correct.
 After you run the program, please check the files.
 You can probably restore it correctly if the drive has enough free
 space
 and if it's just after deletion.
 If you restore a folder, there is no guarantee that all files or
 folders which used to belong to the folder will be in it.
 If you can't find them in it, please search and restore them one by
 one.
 If you still can't find them, it's impossible to restore them.
 It will be almost impossible to restore any deleted files if you run
 the complete deletion function.
 To be exact, fragments of deleted files can be in last clusters of
 existing files or allocated and unwritten files.
 (Note that this function is not suitable for TOP SECRET information.)
 Even if you find the information of completely deleted files, their
 contents must be erased.
 If you want to erase the information too, please run the function
 again.

 When you use the restoration function, I don't think you will
 encounter any damage due to bugs, since the function doesn't write
 anything to physical sectors.
 However, the complete deletion function replaces physical sectors.
 Therefore, I confirmed that it worked without any problems by testing
 many times.
 As there is a rare possibility of damage or loss, please run the
 function at your own risk.

[Update History]
 Version 1.7.3
  Added Windows NT/2000 support.
  Added vacant clusters scan and all clusters scan function.
 Version 1.8.12
  Fixed two bugs concerned with reading specific extended dos partition
  or hidden partition.
 Version 2.2.12
  Added folder restoration function.
  Added NTFS partition support.
  Fixed a bug concerned with reading specific removable disk.
 Version 2.5.6
  Added comlete deletion function.
  Added support of disks with non 512 bytes sector size such as FAT32
  DVD-RAM.
  Fixed a bug concerned with non-residet attribute list attribute of
  NTFS.
  Fixed a bug concerned with reading specific FAT32 partition.
 Version 2.5.14
  Fixed a bug concerned with folder restoration of FAT partition.
  (The error occurd from Version 2.5.6.)

[Escape Clause]
 I'm not responsible for any damage or loss caused by the use of this
 software in any case.
 In addition, even if this software is defective, I'm not under
 obligation to correct or support it.

[Supported Platform]
 IBM-PC/AT Compatible PC

[Supported Devices]
 Hard disk
 Floppy disk
 ZIP drive (I tested by parallel port ZIP drive. I don't know the
  reason but if the OS is Windows 95/98/Me, it doesn't recognize the
  disk unless the disk is inside the drive when the PC boots up.
  However, you can change the disk after boot.)
 PD (A user reported that it worked correctly.)
 DVD-RAM (A user reported that it worked correctly.)
 Smart media (A user reported that it worked correctly.)
 Memory stick drive (A user reported that it worked correctly.)

  No other devices are tested.

[Supported OS]
 Windows 95/98/Me/NT/2000/XP

[Supported File System]
 FAT12/FAT16/FAT32/NTFS
  Compressed files of NTFS are supported.
  However, encrypted files of NTFS are not supported so far.

=======================================================================

[How to Run]
 Double click "Restoration.exe" file, which is extracted form the
 self-extracting file or the zip file.
 If the OS is Windows 95/98/Me, both the "DLL32.dll" file and the
 "DLL16.dll" file have to be in the same folder as the
 "Restoration.exe" file.
 (These two files are not required for Windows NT/2000/xp.)
 (If "Do not show hidden or System files" option is selected in
 "Folder Options" of "Explorer", these two files are invisible.)
 If you create the shortcut and run the program from it, you have to
 specify the path of the same folder as these two files
 (e.g. "C:\REST2514") for "Start in" by right clicking on the shortcut
 icon and opening properties.
 I think it can run on most PCs because a static library is used.
 However, if you get an error message when you start it, please install
 a specific MS product such as "Internet Explorer" or try to obtain and
 install the required DLL files.

 [Instructions]
 1. Select a drive. 
  If the first byte of the file is unknown, character "$" is used as a
  substitute. (FAT partition)
  If you specify all or part of the file name, all first byte unknown
  files which are possibly contain the text are displayed.
  You can't use wild card "*".  Therefore, if you want to search by
  extension, please enter something like ". TXT".
  If you check "Include used clusters by other files" box and search,
  you can find more files, but the file has little possibility of being
  restored correctly if the pointed cluster is used by another file.
 (FAT partition)
  If you check the "Include even if the file size is zero" box, you can
  find more files, but it's almost pointless to restore them because
  it's exactly the same as creating a new empty file of the same name
  manually.
  After finishing the regular search, a message will appear asking you
  if you want to scan vacant clusters, if you couldn't find your target
  file, please select yes. (FAT partition)
  All possible items (the ones that pass the specific condition) will
  be listed. (FAT partition)
  After showing the items, when you click on the heading for a column,
  that values in that column instantly sort in ascending sequence.
  If you click the heading a second time, the values are sorted in
  descending sequence.
  Subsequent clicks would continue to alternate the sort sequence
  between ascending and descending.

 2. Select a listed file.

 3. Click "Restore by copying" button.

 4. Specify the location you want to restore to.
  I recommend you if possible to select a different drive from the
  searched drive so not to overwrite original data.
  If the file is a compressed file of NTFS, it will be decompressed and
  restored.

All clusters scan (FAT partition)
 Scan all clusters unconditionally and list all candidates
 (the ones that pass the specific condition).

 1. Click "Others" and then "Scan All Clusters" from the menu bar.

 2. Answer the dialog boxes.

Complete deletion
 Erase all vacant clusters by overwriting with random numbers and then
 with zeros.
 After that, erase all deleted file information(FAT partition)/file
 records(NTFS partition) by using random numbers and zeros.

 1. Click "Others" and then "Delete Completely" from the menu bar.

 2. Answer the dialog boxes.

=======================================================================

[Copyright]
 This is freeware, but the copyright is reserved.
 You may distribute this program freely only in the original archive
(i.e. it must include this readme document without modification).
 You may not make profit by selling this program.

[Technical Support]
 I'll try to support if time allows.
 However, in most cases it's almost impossible to solve the problem
 without seeing the PC itself.
 My English is not perfect, so please use easy expressions if you send
 me an e-mail.
 In addition, if you are a native English speaker and find any wrong
 expressions in the program or this readme document, please correct it
 and let me know.

Author: Brian Kato    e-mail: bkato@fli.freeserve.ne.jp
