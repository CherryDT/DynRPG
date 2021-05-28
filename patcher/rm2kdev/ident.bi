Type TIdentData
	SizeOfCode As UInteger
	SizeOfInitializedData As UInteger
End Type

Function Ident(filename As String, identdata As TIdentData Ptr) As Boolean
	If identdata = NULL Then Return NULLPOINTER
	Dim fileh As FileObj.FILE = FileObj.FILE(filename, FileObj.MODE_BINARY)
	If fileh.valid = FALSE Then Return FILENOTFOUND
	If fileh.ReadByte(1) <> Asc("M") Or fileh.ReadByte(2) <> Asc("Z") Then Return INVALID
	Var pe_start = fileh.ReadInt(&h3C + 1)
	If pe_start < 2 Or pe_start > fileh.Length() - &h14 Then Return INVALID
	Var opt_start = pe_start + &h18 'fileh.ReadInt(pe_start + &h14 + 1)
	If opt_start < 2 Or opt_start > fileh.Length() - &h40 Then Return INVALID
	identdata->SizeOfCode = fileh.ReadInt(opt_start + &h4 + 1)
	identdata->SizeOfInitializedData = fileh.ReadInt(opt_start + &h8 + 1)
	Return FALSE
End Function

Type TRTVersion
	RMType As Integer
	Enum
		N_A = -2
		UNKNOWN
		RM2000 = 2000
		RM2003 = 2003
	End Enum
	Major As Integer
	Minor As Integer
	Details As Integer
	Enum
		MENUPATCH = &h1
		MAKERULTIMATIVE = &h2
		TRACKERP = &h4
		SSS = &h8
		POWERPATCH1 = &h10
		POWERPATCH2 = &h20
		GNAF = &h40
		SHELLSTART = &h80
		ALLINONE = &h100
		DESTINY = &h200
		FATALMIX = &h400
		MOVIEPRELOADER = &h800
		NEEDHARMONY = &h20000000
		MOLEBOX = &h40000000
		UPX = &h80000000
	End Enum
	ValueVer As Boolean
	VersionD As Double
	RTFilename As String
	Declare Sub Set(_RMType As Integer, _Details As Integer, _ValueVer As Boolean, _VersionD As Double)
	Found As Boolean
End Type

Sub TRTVersion.Set(_RMType As Integer, _Details As Integer, _ValueVer As Boolean, _VersionD As Double)
	RMType = _RMType
	Major = Int(_VersionD)
	Minor = Frac(_VersionD) * 100
	Details Or= _Details
	ValueVer = _ValueVer
	VersionD = _VersionD
End Sub

#Define TypeUL(_x_, _y_) ((CULngInt(_y_) Shl 32) Or CULngInt(_x_))

#Ifdef _RTVER
	Function GetRTVersion(filename As String, verstruct As TRTVersion Ptr = NULL, callback As Sub(n As Integer, f As String) = NULL) As String
		Dim tmpv As TRTVersion
		If verstruct = NULL Then verstruct = @tmpv
		verstruct->RTFilename = filename
		Dim identdata As TIdentData
		If callback <> NULL Then callback(0, filename)
		If FileExists(filename) = FALSE OrElse Ident(filename, @identdata) <> OKAY Then
			verstruct->Set(TRTVersion.N_A, NULL, FALSE, 0f)
			Return "Invalid"
		EndIf
		If callback <> NULL Then callback(1, filename)
		Dim dump As HackCode.Dump
		dump.FillFromFile(filename)
		Dim tmp As String
		Dim dummy As Integer
		If callback <> NULL Then callback(16, filename)
		tmp = "Destiny"
		dummy = 0
		tmp &= "Initialize"
		If dump.Find(tmp) <> HackCode.NOT_FOUND Then
			verstruct->Details Or= TRTVersion.DESTINY
		EndIf
		If callback <> NULL Then callback(24, filename)
		tmp = "Harmony"
		dummy = 0
		tmp &= "Create"
		If dump.Find(tmp) <> HackCode.NOT_FOUND Then
			verstruct->Details Or= TRTVersion.NEEDHARMONY
		EndIf
		If callback <> NULL Then callback(15, filename)
		tmp = "TFormLcfGameMain"
		dummy = 0
		tmp &= !"\&h0FFormLcfGameMain"
		If dump.Find(tmp) = HackCode.NOT_FOUND Then
			If callback <> NULL Then callback(17, filename)
			tmp = "Start"
			dummy = 0
			tmp &= "menü8"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Details Or= TRTVersion.MENUPATCH
				Return GetRTVersion(ExtractPath(filename) & "\rpg_rt.dat", verstruct)
			EndIf
			If callback <> NULL Then callback(18, filename)
			tmp = "rpg_rt_"
			dummy = 0
			tmp &= "original.exe"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Details Or= TRTVersion.SHELLSTART
				Return GetRTVersion(ExtractPath(filename) & "\" & tmp, verstruct)
			EndIf
			If callback <> NULL Then callback(19, filename)
			tmp = "KÌ%©Ö"
			dummy = 0
			tmp &= "ßñÜÐ3"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Set(TRTVersion.RM2000, TRTVersion.TRACKERP, FALSE, 0f)
				Return "TrackerP Patch"
			EndIf
			If callback <> NULL Then callback(20, filename)
			tmp = "varhack"
			dummy = 0
			tmp &= ".exe"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Details Or= TRTVersion.POWERPATCH1
				Return GetRTVersion(ExtractPath(filename) & "\rpg_rt.oex", verstruct)
			EndIf
			If callback <> NULL Then callback(21, filename)
			tmp = "im Forum"
			dummy = 0
			tmp &= " sagen"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Details Or= TRTVersion.ALLINONE
				Return GetRTVersion(ExtractPath(filename) & "\rpg_rt.dat", verstruct)
			EndIf
			If callback <> NULL Then callback(22, filename)
			tmp = "Mauspa"
			dummy = 0
			tmp &= "Pro"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Details Or= TRTVersion.MAKERULTIMATIVE
				Return GetRTVersion(ExtractPath(filename) & "\rpg.res", verstruct)
			EndIf
			If callback <> NULL Then callback(23, filename)
			tmp = "rpg_rt_real"
			dummy = 0
			tmp &= ".dat"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Details Or= TRTVersion.SSS
				Return GetRTVersion(ExtractPath(filename) & "\" & tmp, verstruct)
			EndIf
			If callback <> NULL Then callback(25, filename)
			tmp = "MoleBox "
			dummy = 0
			tmp &= " launcher"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Set(TRTVersion.UNKNOWN, TRTVersion.MOLEBOX, FALSE, 0f)
				Return "Unknown (MoleBox packed)"
			EndIf
			If callback <> NULL Then callback(26, filename)
			tmp = "UP"
			dummy = 0
			tmp &= "X1"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Set(TRTVersion.UNKNOWN, TRTVersion.UPX, FALSE, 0f)
				Return "Unknown (UPX packed)"
			EndIf
			If callback <> NULL Then callback(27, filename)
			tmp = "CPU "
			dummy = 0
			tmp &= "Clock:"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Details Or= TRTVersion.POWERPATCH2
				Return GetRTVersion(ExtractPath(filename) & "\rpg_rt.fwd", verstruct)
			EndIf
			If callback <> NULL Then callback(28, filename)
			tmp = "intro"
			dummy = 0
			tmp &= ".avi"
			If dump.Find(tmp) <> HackCode.NOT_FOUND Then
				verstruct->Details Or= TRTVersion.MOVIEPRELOADER
				Return GetRTVersion(ExtractPath(filename) & "\rpg_rt.ch1", verstruct)
			EndIf
		Else
			verstruct->Found = TRUE
		EndIf
		If callback <> NULL Then callback(64, filename)
		#Define HexTypeUL(_x_, _y_) (Hex(TypeUL(_x_, _y_)))
		Select Case Hex(*Cast(ULongInt Ptr, @identdata))
			Case HexTypeUL(&h96A00, &h1A600)
				verstruct->Set(TRTVersion.RM2000, NULL, FALSE, 1.03)
				Return "RM2000 v1.03"
			Case HexTypeUL(&h96E00, &h1BA00), HexTypeUL(&h96E00, &h1AC00)
				verstruct->Set(TRTVersion.RM2000, NULL, FALSE, 1.05)
				Return "RM2000 v1.05"
			Case HexTypeUL(&h96F23, &h2F70), HexTypeUL(&h96F53, &h2F70)
				verstruct->Set(TRTVersion.RM2000, TRTVersion.DESTINY, FALSE, 1.05)
				Return "RM2000 v1.05"
			Case HexTypeUL(&h96A00, &h1AC00), HexTypeUL(&h96A00, &h1AE00), HexTypeUL(&h96A00, &hFE00), HexTypeUL(&h96E00, &h1AA00), HexTypeUL(&h96A00, &h1A800)
				verstruct->Set(TRTVersion.RM2000, NULL, FALSE, 1.07)
				Return "RM2000 v1.07"
			Case HexTypeUL(&h96B17, &h2F70), HexTypeUL(&h96B47, &h2F70)  
				verstruct->Set(TRTVersion.RM2000, TRTVersion.DESTINY, FALSE, 1.07)
				Return "RM2000 v1.07"
			Case HexTypeUL(&h96600, &h1A600) 
				verstruct->Set(TRTVersion.RM2000, NULL, FALSE, 1.09)
				Return "RM2000 v1.09"
			Case HexTypeUL(&h96C00, &h1A600) 
				verstruct->Set(TRTVersion.RM2000, NULL, FALSE, 1.10)
				Return "RM2000 v1.10"
			Case HexTypeUL(&h9BC00, &h19C00)
				verstruct->Set(TRTVersion.RM2000, NULL, TRUE, 1.50)
				Return "RM2000 Value! v1.50"
			Case HexTypeUL(&h9BC00, &h21600)
				verstruct->Set(TRTVersion.RM2000, TRTVersion.GNAF, TRUE, 1.50)
				Return "RM2000 Value! v1.50 (Gnaf)"
			Case HexTypeUL(&h9BE00, &h1AC00), HexTypeUL(&h9BE00, &h1A200), HexTypeUL(&h9BE00, &h1B200)
				verstruct->Set(TRTVersion.RM2000, NULL, TRUE, 1.51)
				Return "RM2000 Value! v1.51"
			Case HexTypeUL(&h9BE00, &h1EE00) 
				verstruct->Set(TRTVersion.RM2000, TRTVersion.FATALMIX, TRUE, 1.51)
				Return "RM2000 Value! v1.51 (FatalMix)"
			Case HexTypeUL(&hC0800, &h1E400)
				verstruct->Set(TRTVersion.RM2003, NULL, FALSE, 2.03)
				Return "RM2003 v1.03"
			Case HexTypeUL(&hC7400, &h23600)
				verstruct->Set(TRTVersion.RM2003, NULL, FALSE, 2.05)
				Return "RM2003 v1.05"
			Case HexTypeUL(&hC8A00, &h24A00)
				verstruct->Set(TRTVersion.RM2003, NULL, FALSE, 2.06)
				Return "RM2003 v1.06"
			Case HexTypeUL(&hC8A00, &h1F000)
				verstruct->Set(TRTVersion.RM2003, NULL, FALSE, 2.07)
				Return "RM2003 v1.07"
			Case HexTypeUL(&hC8E00, &h1EE00)
				verstruct->Set(TRTVersion.RM2003, NULL, FALSE, 2.08)
				Return "RM2003 v1.08"
			Case HexTypeUL(&hC9000, &h23A00), HexTypeUL(&hC9000, &h1EE00)
				verstruct->Set(TRTVersion.RM2003, NULL, FALSE, 2.091)
				Return "RM2003 v1.09a"
			Case Else
				verstruct->Set(TRTVersion.UNKNOWN, NULL, FALSE, 0f)
				Return "Unknown"
		End Select
	End Function
#EndIf