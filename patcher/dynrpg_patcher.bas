#Include "windows.bi"
#Include "win/commdlg.bi"
#Include "rsrc.bi"
#Include "file.bi"
#Include "string.bi"
#Include "useful/file_obj_standalone.bi"
#Define _NO_HPA
#Include "rm2kdev/hpd.bi"
#Define ExtractPath(_st_) Left((_st_), InStrRev((_st_), "\") - 1)

#Define VERSION "0.20"

Dim Shared hInstance As HINSTANCE
hInstance = GetModuleHandle(NULL)

Function CopyResourceToFile(id As Integer, filename As String) As Boolean
	Var hRsrc = FindResource(hInstance, id, RT_RCDATA)
	If hRsrc = NULL Then Return FALSE
	Var hResData = LoadResource(hInstance, hRsrc)
	If hResData = NULL Then Return FALSE
	Var reslen = SizeofResource(hInstance, hRsrc)
	If reslen = NULL Then FreeResource(hResData): Return FALSE
	Var resptr = LockResource(hResData)
	If resptr = NULL Then FreeResource(hResData): Return FALSE
	Function = TRUE
	Kill(filename)
	Var ff = FreeFile()
	Open filename For Binary As #ff
	Put #ff, 1, *CPtr(UByte Ptr, resptr), reslen
	Close #ff
	If Err() Then Function = FALSE
	FreeResource(hResData)
End Function

If MessageBox(NULL, "This application will install the DynRPG Patch (version " & VERSION & ") on your RPG Maker 2003 game. After clicking ""OK"" in this window, you need to select your RPG_RT.exe file.", "DynRPG Patcher", MB_OKCANCEL Or MB_ICONINFORMATION) = IDCANCEL Then End(0)

Dim target As ZString * MAX_PATH + 1

Dim ofn As OPENFILENAME
Const FILTER = !"RPG_RT.exe\0RPG_RT.exe\0All files (*.*)\0*.*\0\0"
With ofn
	.lStructSize = SizeOf(ofn)
	.hwndOwner = NULL
	.hInstance = hInstance
	.lpstrFilter = @FILTER
	.lpstrFile = @target
	.nMaxFile = MAX_PATH
	.lpstrTitle = @"Select project's RPG_RT.exe file"
	.Flags = OFN_EXPLORER Or OFN_PATHMUSTEXIST Or OFN_FILEMUSTEXIST Or OFN_HIDEREADONLY
End With

If GetOpenFileName(@ofn) = FALSE Then End(0)

Var fileh = FileObj.File(target, FileObj.MODE_BINARY)
If fileh.valid = FALSE Then
	MessageBox(NULL, "The selected file couldn't be opened!", "DynRPG Patcher", MB_OK Or MB_ICONERROR)
	End(0)
EndIf
Var s = fileh.ReadString(fileh.length())
If InStr(s, "TLcfFieldScene") = 0 Then
	fileh.CloseF()
	MessageBox(NULL, "The selected file is no RPG Maker runtime file! Maybe you have renamed it in the past during installation of another patch...?", "DynRPG Patcher", MB_OK Or MB_ICONERROR)
	End(0)
EndIf
If InStr(s, "TLcfJob") = 0 Then
	fileh.CloseF()
	MessageBox(NULL, "Your project seems to use RPG Maker 2000. DynRPG does only work with RPG Maker 2003, I am sorry. However, it might be possible for you to upgrade to RPG Maker 2003, use Google to find out more.", "DynRPG Patcher", MB_OK Or MB_ICONERROR)
	End(0)
EndIf

Dim EntryPoint As Integer

If fileh.ReadByte(1) <> Asc("M") Or fileh.ReadByte(2) <> Asc("Z") Then GoTo Problem
Var pe_start = fileh.ReadInt(&h3C + 1)
If pe_start < 2 Or pe_start > fileh.Length() - &h14 Then GoTo Problem
Var opt_start = pe_start + &h18
If opt_start < 2 Or opt_start > fileh.Length() - &h40 Then GoTo Problem
EntryPoint = fileh.ReadInt(opt_start + &h1C + 1) + fileh.ReadInt(opt_start + &h10 + 1) ' ImageBase + RVA

Problem:
fileh.CloseF()

Dim ok As Boolean = FALSE

If EntryPoint <> &h4C9C1C Then
	If MessageBox(NULL, "Your RPG_RT.exe version is not 1.08. DynRPG only works with version 1.08. Do you want to install a RPG_RT.exe with version 1.08? THIS WILL REMOVE ALL EXISTING PATCHES!", "DynRPG Patcher", MB_YESNO Or MB_ICONWARNING) = IDYES Then
		Kill(target & ".bak")
		FileCopy(target, target & ".bak")
		Kill(target)
		CopyResourceToFile(IDR_EXE, target)
		ok = TRUE
	EndIf
Else
	Var tempfile = Environ("temp") & "\dynrpg.hpd"
	Kill(tempfile)
	CopyResourceToFile(IDR_HPD, tempfile)
	Kill(target & ".bak")
	FileCopy(target, target & ".bak")
	ok = HPD_Apply(tempfile, target, TRUE)
	Kill(tempfile)
EndIf

Var p = ExtractPath(target)

If ok Then
	If FileExists(p & "\dynloader.dll") Then
		Kill(p & "\dynloader.dll.bak")
		FileCopy(p & "\dynloader.dll", p & "\dynloader.dll.bak")
		Kill(p & "\dynloader.dll")
	EndIf
	CopyResourceToFile(IDR_DLL, p & "\dynloader.dll")
	MkDir(p & "\DynPlugins")
	MkDir(p & "\DynPatches")
	Open p & "\DynRPG.ini" For Binary As #90
	Var s = Space(Lof(90))
	Get #90, 1, s
	Close #90
	If InStr(s, "[QuickPatches]") = 0 Then
		Open p & "\DynRPG.ini" For Append As #90
		Print #90, "[QuickPatches]"
		Print #90, "; Paste quickpatches here"
		Close #90
	EndIf
EndIf

If ok = FALSE Then
	If FileExists(target & ".bak") Then
		Kill(target)
		FileCopy(target & ".bak", target)
		Kill(target & ".bak")
	EndIf
	If FileExists(p & "\dynloader.dll.bak") Then
		Kill(p & "\dynloader.dll")
		FileCopy(p & "\dynloader.dll.bak", p & "\dynloader.dll")
		Kill(p & "\dynloader.dll.bak")
	EndIf
	MessageBox(NULL, "There was an error during patching. The backup has been restored.", "DynRPG Patcher", MB_OK Or MB_ICONERROR)
Else
	MessageBox(NULL, "The DynRPG Patch was successfully installed! Have fun with the new world of plugins!", "DynRPG Patcher", MB_OK Or MB_ICONINFORMATION)
EndIf
