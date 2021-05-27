Const HPD_SIGNATURE = "HPD1"
Const HPA_SIGNATURE = "HPA1"

Sub HPD_Create(patchfile_ As String, original_ As String, modified_ As String)
	Kill(patchfile_)
	Var patchfile = FileObj.File(patchfile_, FileObj.MODE_BINARY)
	Var original = FileObj.File(original_, FileObj.MODE_BINARY)
	Var modified = FileObj.File(modified_, FileObj.MODE_BINARY)
	patchfile.WriteInt(*Cast(Integer Ptr, @HPD_SIGNATURE))
	patchfile.WriteInt(original.Length())
	Var ol = original.Length()
	Var ml = modified.Length()
	For i As Integer = 1 To max(ol, ml)
		If original.ReadByte() <> modified.ReadByte() Or i > ol Then
			Dim l As Integer
			For j As Integer = i To max(ol, ml)
				If original.ReadByte(j) = modified.ReadByte(j) And i <= ol Then Exit For
				l += 1
			Next
			patchfile.WriteInt(i)
			patchfile.WriteInt(l)
			For j As Integer = 0 To l - 1
				patchfile.WriteByte(modified.ReadByte(j + i))
			Next
			i += l - 1
			original.position = i + 1
			modified.position = i + 1
		EndIf
	Next
End Sub

Function HPD_Apply(patchfile_ As String, target_ As String, ignoreLength As Boolean = FALSE) As Boolean
	Var patchfile = FileObj.File(patchfile_, FileObj.MODE_BINARY)
	Var target = FileObj.File(target_, FileObj.MODE_BINARY)
	If patchfile.ReadInt() <> *Cast(Integer Ptr, @HPD_SIGNATURE) Then Return FALSE
	If patchfile.ReadInt() <> target.Length() And ignoreLength = FALSE Then Return FALSE
	Do Until patchfile.EndReached()
		Var start = patchfile.ReadInt()
		Var Length = patchfile.ReadInt()
		For i As Integer = 0 To Length - 1
			target.WriteByte(patchfile.ReadByte(), start + i)
		Next
	Loop
	Return TRUE
End Function

Sub HPA_Create(archfile_ As String)
	Kill(archfile_)
	Var archfile = FileObj.File(archfile_, FileObj.MODE_BINARY)
	archfile.WriteInt(*Cast(Integer Ptr, @HPA_SIGNATURE))
	For i As Integer = 1 To 4096
		archfile.WriteInt(0) ' Start Ptr
		archfile.WriteInt(0) ' Ident->SizeOfCode
		archfile.WriteInt(0) ' Ident->SizeOfInitializedData
	Next
End Sub

Function HPA_Add(archfile_ As String, patchfile_ As String, name_ As String, ident_ As TIdentData) As Boolean
	Var archfile = FileObj.File(archfile_, FileObj.MODE_BINARY)
	Var patchfile = FileObj.File(patchfile_, FileObj.MODE_BINARY)
	If archfile.ReadInt() <> *Cast(Integer Ptr, @HPA_SIGNATURE) Then Return FALSE
	If patchfile.ReadInt() <> *Cast(Integer Ptr, @HPD_SIGNATURE) Then Return FALSE
	For i As Integer = 1 To 4096
		Var startptr = archfile.ReadInt()
		archfile.position = archfile.position + 8
		If startptr = 0 Then
			startptr = archfile.Length() + 1
			archfile.position = archfile.position - 12
			archfile.WriteInt(startptr)
			archfile.WriteInt(ident_.SizeOfCode)
			archfile.WriteInt(ident_.SizeOfInitializedData)
			archfile.position = startptr
			archfile.WriteByte(CUByte(Len(name_)))
			archfile.WriteString(Left(name_, CUByte(Len(name_))))
			archfile.WriteInt(patchfile.Length() - 4)
			archfile.WriteInt(patchfile.ReadInt())
			For j As Integer = 9 To patchfile.Length()
				archfile.WriteByte(patchfile.ReadByte())
			Next
			Return TRUE
		EndIf
	Next
	Return FALSE
End Function

Function HPA_Get(archfile_ As String, patchfile_ As String, name_ As String, ident_ As TIdentData) As Boolean
	Var archfile = FileObj.File(archfile_, FileObj.MODE_BINARY)
	Var patchfile = FileObj.File(patchfile_, FileObj.MODE_BINARY)
	patchfile.WriteInt(*Cast(Integer Ptr, @HPD_SIGNATURE))
	If archfile.ReadInt() <> *Cast(Integer Ptr, @HPA_SIGNATURE) Then Return FALSE
	For i As Integer = 1 To 4096
		Var startptr = archfile.ReadInt()
		Var ident2_ = Type<TIdentData>(archfile.ReadInt(), archfile.ReadInt())
		If ident2_.SizeOfCode = ident_.SizeOfCode And ident2_.SizeOfInitializedData = ident_.SizeOfInitializedData Then
			Var oldpos = archfile.position
			archfile.position = startptr
			Var textlen = archfile.ReadByte()
			Var name2_ = archfile.ReadString(textlen)
			If Trim(UCase(name2_)) = Trim(UCase(name_)) Then
				Var patchlen = archfile.ReadInt()
				patchfile.WriteInt(archfile.ReadInt())
				For j As Integer = 3 To patchlen
					patchfile.WriteByte(archfile.ReadByte())
				Next
				Return TRUE
			EndIf
			archfile.position = oldpos
		EndIf
	Next
	Return FALSE
End Function

Function HPA_GetMem(archfile_ As String, name_ As String, ident_ As TIdentData) As UInteger Ptr
	Var archfile = FileObj.File(archfile_, FileObj.MODE_BINARY)
	If archfile.ReadInt() <> *Cast(Integer Ptr, @HPA_SIGNATURE) Then Return NULL
	For i As Integer = 1 To 4096
		Var startptr = archfile.ReadInt()
		Var ident2_ = Type<TIdentData>(archfile.ReadInt(), archfile.ReadInt())
		If ident2_.SizeOfCode = ident_.SizeOfCode And ident2_.SizeOfInitializedData = ident_.SizeOfInitializedData Then
			Var oldpos = archfile.position
			archfile.position = startptr
			Var textlen = archfile.ReadByte()
			Var name2_ = archfile.ReadString(textlen)
			If Trim(UCase(name2_)) = Trim(UCase(name_)) Then
				Var patchlen = archfile.ReadInt()
				archfile.position = archfile.position + 4
				Dim buffer As UByte Ptr = Callocate(max(patchlen, 1024))
				For j As Integer = 3 To patchlen
					buffer[j - 3] = archfile.ReadByte()
				Next
				Return buffer
			EndIf
			archfile.position = oldpos
		EndIf
	Next
	Return NULL
End Function

Function HPA_GetInt(archfile As String, patchname As String, ident_ As TIdentData, id As Integer) As UInteger
	If FileExists(archfile) = FALSE Then
		Return 0
	EndIf
	Var buffer = RM2kDev.HPA_GetMem(archfile, patchname, ident_)
	If buffer = NULL Then Return 0
	Var ret = buffer[id]
	DeAllocate(buffer)
	Return ret
End Function

Function HPA_Check(archfile_ As String, name_ As String, ident_ As TIdentData) As Boolean
	Var archfile = FileObj.File(archfile_, FileObj.MODE_BINARY)
	If archfile.ReadInt() <> *Cast(Integer Ptr, @HPA_SIGNATURE) Then Return NULL
	For i As Integer = 1 To 4096
		Var startptr = archfile.ReadInt()
		Var ident2_ = Type<TIdentData>(archfile.ReadInt(), archfile.ReadInt())
		If ident2_.SizeOfCode = ident_.SizeOfCode And ident2_.SizeOfInitializedData = ident_.SizeOfInitializedData Then
			Var oldpos = archfile.position
			archfile.position = startptr
			Var textlen = archfile.ReadByte()
			Var name2_ = archfile.ReadString(textlen)
			If Trim(UCase(name2_)) = Trim(UCase(name_)) Then
				Return TRUE
			EndIf
			archfile.position = oldpos
		EndIf
	Next
	Return FALSE
End Function

Function HPA_Apply(archfile_ As String, target_ As String, name_ As String, ident_ As TIdentData, ignoreLength As Boolean = FALSE) As Boolean
	Var tmp = Environ("Temp") & "\~hpa" & Hex(&h1000 + Rnd * &hEFFF) & ".tmp"
	Var ret = HPA_Get(archfile_, tmp, name_, ident_)
	If ret Then
		ret And= HPD_Apply(tmp, target_, ignoreLength)
		Kill(tmp)
	EndIf
	Return ret
End Function