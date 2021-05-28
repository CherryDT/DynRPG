#Define LINK_VERSION 3
#Include "dynloader.bi"

Function StrToImg(s As String) As FB.IMAGE Ptr
	Var img = ImageCreate(Len(s) * 8, 14, &hFF00FF)
	Draw String img, (0, 0), s
	Return img
End Function

Function GetIcon() As FB.IMAGE Ptr
	Var img = ImageCreate(10, 10, &hFF00FF)
	For y As Integer = 0 To 8
		For x As Integer = 0 To 8
			Dim p As Integer
			Read p
			If p Then PSet img, (x, y), &h0000FF
		Next
	Next
	Return img
End Function

' MultiPut by D.J.Peters: https://www.freebasic.net/forum/viewtopic.php?f=8&t=19303
Sub MultiPut(Byval lpTarget As Any Ptr= 0, _
             Byval xMidPos  As Integer= 0, _
             Byval yMidPos  As Integer= 0, _
             Byval lpSource As Any Ptr   , _
             Byval xScale   As Single = 1, _
             Byval yScale   As Single = 1, _
             Byval Rotate   As Single = 0, _
             Byval Trans    As Integer= 0)

  If (screenptr=0) Or (lpSource=0) Then Exit Sub

  If xScale < 0.001 Then xScale=0.001
  If yScale < 0.001 Then yScale=0.001

  Dim As Integer MustLock,MustRotate

  If lpTarget= 0 Then MustLock  =1
  If Rotate  <>0 Then MustRotate=1

  Dim as Integer  TargetWidth,TargetHeight,TargetBytes,TargetPitch
  If MustLock Then
    ScreenInfo    _
    TargetWidth , _
    TargetHeight, _
    TargetBytes ,,_
    TargetPitch
    TargetBytes shr=3

    lpTarget=ScreenPtr
  Else
    TargetBytes  = cptr(uinteger Ptr,lpTarget)[1]
    TargetWidth  = cptr(uinteger Ptr,lpTarget)[2]
    TargetHeight = cptr(uinteger Ptr,lpTarget)[3]
    TargetPitch  = cptr(uinteger Ptr,lpTarget)[4]
    lpTarget    += 32
  End If
  If (TargetWidth<4) Or (TargetHeight<4) Then Exit Sub

  Dim As Integer   SourceWidth,SourceHeight,SourceBytes,SourcePitch
  if cptr(integer Ptr,lpSource)[0] = 7 then
    SourceBytes  = cptr(uinteger Ptr,lpSource)[1]
    SourceWidth  = cptr(uinteger Ptr,lpSource)[2]
    SourceHeight = cptr(uinteger Ptr,lpSource)[3]
    SourcePitch  = cptr(uinteger Ptr,lpSource)[4]
    lpSource    += 32
  else
    SourceBytes  = 1
    SourceWidth  = cptr(ushort Ptr,lpSource)[0] shr 3
    SourceHeight = cptr(ushort Ptr,lpSource)[1]
    SourcePitch  = SourceWidth
    lpSource    += 4
  end if
#if 0
  ? TargetWidth & "x" & TargetHeight & "x" & TargetBytes,TargetPitch
  ? SourceWidth & "x" & SourceHeight & "x" & SourceBytes,SourcePitch
  ? MustLock,Trans
  sleep:end 
#endif

  If (SourceWidth<2) Or (SourceHeight<2) Then Exit Sub
  If (TargetBytes<>SourceBytes) Then Exit Sub

#define xs 0 'screen
#define ys 1
#define xt 2 'texture
#define yt 3
  Dim As Single Points(3,3)
  points(0,xs)=-SourceWidth/2 * xScale
  points(1,xs)= SourceWidth/2 * xScale
  points(2,xs)= points(1,xs)
  points(3,xs)= points(0,xs)

  points(0,ys)=-SourceHeight/2 * yScale
  points(1,ys)= points(0,ys)
  points(2,ys)= SourceHeight/2 * yScale
  points(3,ys)= points(2,ys)

  points(1,xt)= SourceWidth-1
  points(2,xt)= points(1,xt)
  points(2,yt)= SourceHeight-1
  points(3,yt)= points(2,yt)

  Dim As Uinteger i
  Dim As Single x,y
  If MustRotate Then
    #ifndef UseRad
    Rotate*=0.017453292 'degre 2 rad
    #endif
    While Rotate< 0        :rotate+=6.2831853:Wend
    While Rotate>=6.2831853:rotate-=6.2831853:Wend
    For i=0 To 3
      x=points(i,xs)*Cos(Rotate) - points(i,ys)*Sin(Rotate)
      y=points(i,xs)*Sin(Rotate) + points(i,ys)*Cos(Rotate)
      points(i,xs)=x:points(i,ys)=y
    Next
  End If

  Dim As Integer yStart,yEnd,xStart,xEnd
  yStart=100000:yEnd=-yStart:xStart=yStart:xEnd=yEnd

#define LI 0   'LeftIndex
#define RI 1   'RightIndex
#define  IND 0 'Index
#define NIND 1 'NextIndex
  Dim As Integer CNS(1,1) 'Counters

  For i=0 To 3
    points(i,xs)=Int(points(i,xs)+xMidPos)
    points(i,ys)=Int(points(i,ys)+yMidPos)
    If points(i,ys)<yStart Then yStart=points(i,ys):CNS(LI,IND)=i
    If points(i,ys)>yEnd   Then yEnd  =points(i,ys)
    If points(i,xs)<xStart Then xStart=points(i,xs)
    If points(i,xs)>xEnd   Then xEnd  =points(i,xs)
  Next
  If yStart =yEnd         Then Exit Sub
  If yStart>=TargetHeight Then Exit Sub
  If yEnd   <0            Then Exit Sub
  If xStart = xEnd        Then Exit Sub
  If xStart>=TargetWidth  Then Exit Sub
  If xEnd   <0            Then Exit Sub

  Dim As uByte    Ptr t1,s1
  Dim As uShort   Ptr t2,s2
  Dim As uInteger Ptr t4,s4


#define ADD 0
#define CMP 1
#define SET 2
  Dim As Integer ACS(1,2) 'add compare and set
  ACS(LI,ADD)=-1:ACS(LI,CMP)=-1:ACS(LI,SET)=3
  ACS(RI,ADD)= 1:ACS(RI,CMP)= 4:ACS(RI,SET)=0

#define EX  0
#define EU  1
#define EV  2
#define EXS 3
#define EUS 4
#define EVS 5
  Dim As Single E(2,6),S(6),Length,uSlope,vSlope
  Dim As Integer U,UV,UA,UN,V,VV,VA,VN

  ' share the same highest point
  CNS(RI,IND)=CNS(LI,IND)
  If MustLock Then ScreenLock
  ' loop from Top to Bottom
  While yStart<yEnd
    'Scan Left and Right sides together
    For i=LI To RI
      ' bad to read but fast and short ;-)
      If yStart=points(CNS(i,IND),ys) Then
        CNS(i,NIND)=CNS(i,IND)+ACS(i,Add)
        If CNS(i,NIND)=ACS(i,CMP) Then CNS(i,NIND)=ACS(i,SET)
        While points(CNS(i,IND),ys) = points(CNS(i,NIND),ys)
          CNS(i, IND)=CNS(i,NIND)
          CNS(i,NIND)=CNS(i, IND)+ACS(i,Add)
          If CNS(i,NIND)=ACS(i,CMP) Then CNS(i,NIND)=ACS(i,SET)
        Wend
        E(i,EX) = points(CNS(i, IND),xs)
        E(i,EU) = points(CNS(i, IND),xt)
        E(i,EV) = points(CNS(i, IND),yt)
        Length  = points(CNS(i,NIND),ys)
        Length -= points(CNS(i, IND),ys)
        If Length <> 0.0 Then
          E(i,EXS) = points(CNS(i, NIND),xs)-E(i,EX):E(i,EXS)/=Length
          E(i,EUS) = points(CNS(i, NIND),xt)-E(i,EU):E(i,EUS)/=Length
          E(i,EVS) = points(CNS(i, NIND),yt)-E(i,EV):E(i,EVS)/=Length
        End If
        CNS(i,IND)=CNS(i,NIND)
      End If
    Next

    If (yStart<0)                              Then Goto SkipScanLine
    xStart=E(LI,EX)+0.5:If xStart>=TargetWidth Then Goto SkipScanLine
    xEnd  =E(RI,EX)-0.5:If xEnd  < 0           Then Goto SkipScanLine
    If (xStart=xEnd)                           Then Goto SkipScanLine
    'if xEnd  <xStart                           then goto SkipScanLine
    Length=xEnd-xStart
    uSlope=E(RI,EU)-E(LI,EU):uSlope/=Length
    vSlope=E(RI,EV)-E(LI,EV):vSlope/=Length
    If xstart<0 Then
      Length=Abs(xStart)
      U=Int(E(LI,EU)+uSlope*Length)
      V=Int(E(LI,EV)+vSlope*Length)
      xStart = 0
    Else
      U=Int(E(LI,EU)):V=Int(E(LI,EV))
    End If
    If xEnd>=TargetWidth Then xEnd=TargetWidth-1
    UV=Int(uSlope):UA=(uSlope-UV)*100000:UN=0
    VV=Int(vSlope):VA=(vSlope-VV)*100000:VN=0
    xEnd-=xStart
    Select Case TargetBytes
      Case 1
        t1=cptr(ubyte ptr,lpTarget)
        t1+=yStart*TargetPitch+xStart:xStart=0
        If Trans=0 Then
          While xStart<xEnd
            s1=lpSource+V*SourcePitch+U
            *t1=*s1
            U+=UV:UN+=UA:If UN>=100000 Then U+=1:UN-=100000
            V+=VV:VN+=VA:If VN>=100000 Then V+=1:VN-=100000
            If u<0 Then u=0
            If v<0 Then v=0
            xStart+=1:t1+=1
          Wend
        Else
          While xStart<xEnd
            s1=lpSource+V*SourcePitch+U
            If *s1 Then *t1=*s1
            U+=UV:UN+=UA:If UN>=100000 Then U+=1:UN-=100000
            V+=VV:VN+=VA:If VN>=100000 Then V+=1:VN-=100000
            If u<0 Then u=0
            If v<0 Then v=0
            xStart+=1:t1+=1
          Wend
        End If
      Case 2
        t2=cptr(Short Ptr,lpTarget)
        t2+=yStart*(TargetPitch shr 1)+xStart:xStart=0
        If Trans=0 Then
          While xStart<xEnd
            s2=cptr(Short Ptr,lpSource)+V*(SourcePitch shr 1)+U
            *t2=*s2
            U+=UV:UN+=UA:If UN>=100000 Then U+=1:UN-=100000
            V+=VV:VN+=VA:If VN>=100000 Then V+=1:VN-=100000
            If u<0 Then u=0
            If v<0 Then v=0
            xStart+=1:t2+=1
          Wend
        Else
          While xStart<xEnd
            s2=cptr(Short Ptr,lpSource)+V*(SourcePitch shr 1)+U
            If *s2<>&HF81F Then *t2=*s2
            U+=UV:UN+=UA:If UN>=100000 Then U+=1:UN-=100000
            V+=VV:VN+=VA:If VN>=100000 Then V+=1:VN-=100000
            If u<0 Then u=0
            If v<0 Then v=0
            xStart+=1:t2+=1
          Wend
        End If
      Case 4
        t4=cptr(Integer Ptr,lpTarget)+yStart*(TargetPitch shr 2)+xStart:xStart=0
        If Trans=0 Then
          While xStart<xEnd
            s4=cptr(Integer Ptr,lpSource)+V*(SourcePitch shr 2)+U
            *t4=*s4
            U+=UV:UN+=UA:If UN>=100000 Then U+=1:UN-=100000
            V+=VV:VN+=VA:If VN>=100000 Then V+=1:VN-=100000
            If u<0 Then u=0
            If v<0 Then v=0
            xStart+=1:t4+=1
          Wend
        Else
          While xStart<xEnd
            's4=cptr(Integer Ptr,lpSource):s4+=V*(SourcePitch shr 2):s4+=U
            s4=cptr(Integer Ptr,lpSource)+V*(SourcePitch shr 2)+U
            If *s4<>&HFFFF00FF Then *t4=*s4
            U+=UV:UN+=UA:If UN>=100000 Then U+=1:UN-=100000
            V+=VV:VN+=VA:If VN>=100000 Then V+=1:VN-=100000
            If u<0 Then u=0
            If v<0 Then v=0
            xStart+=1:t4+=1
          Wend
        End If
    End Select

SkipScanLine:
    E(LI,EX)+=E(LI,EXS):E(LI,EU)+=E(LI,EUS):E(LI,EV)+=E(LI,EVS)
    E(RI,EX)+=E(RI,EXS):E(RI,EU)+=E(RI,EUS):E(RI,EV)+=E(RI,EVS)
    yStart+=1:If yStart=TargetHeight Then yStart=yEnd 'exit loop
  Wend
If MustLock Then ScreenUnlock
End Sub

Sub ForceToForeground(hWnd As HWND)
	Var fgWnd = GetForegroundWindow()
	AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(hWnd, NULL), TRUE)
	AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(fgWnd, NULL), TRUE)
	BringWindowToTop(hWnd)
	SetForegroundWindow(hWnd)
	AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(fgWnd, NULL), FALSE)
	AttachThreadInput(GetCurrentThreadId(), GetWindowThreadProcessId(hWnd, NULL), FALSE)
End Sub

Function EnumWindowsProc(hWnd As HWND, lParam As HWND Ptr) As BOOL
	Dim cn As ZString * 1000
	GetClassName(hWnd, @cn, 1000)
	Dim pid As Integer
	GetWindowThreadProcessId(hWnd, @pid)
	If pid = GetCurrentProcessId() AndAlso GetWindowLong(hWnd, GWL_STYLE) And (WS_BORDER Or WS_VISIBLE) AndAlso InStr(cn, "IME") = 0 AndAlso cn <> "TApplication" Then
		If GetProp(hWnd, "wcheckedfg") <> TRUE Then
			SetProp(hWnd, "wcheckedfg", TRUE)
			Sleep(100, 1)
			ForceToForeground(hWnd)
		EndIf
		If cn = "TFormLcfGameMain" Then
			*lParam = hWnd
			Return FALSE
		EndIf
	EndIf
	Return TRUE
End Function

Sub WindowChecker()
	Do
		Dim hWnd As HWND = NULL
		EnumWindows(@EnumWindowsProc, @hWnd)
		If hWnd Then
			hWndMain = hWnd
			Exit Do
		EndIf
		
		Sleep(100, 1)
	Loop
End Sub

' VA/File mapping (VA range = 400000 - 4CF000):
' 400000 - 400400 = 00000 - 00400 HEADER
' 401000 - 4C9E00 = 00400 - C9200 CODE
' 4CA000 - 4CE200 = C9200 - CD400 DATA

Sub ApplyQuickPatches()
	Var INIFile = ExePathCached & "\DynRPG.ini"
	GetPrivateProfileString("QuickPatches", NULL, !"\0", @KeyNamesBuffer, SizeOf(KeyNamesBuffer), INIFile)
	Dim KeyName As ZString Ptr = @KeyNamesBuffer
	Do While KeyName[0]
		ZeroMemory(@ValueBuffer, SizeOf(ValueBuffer)) ' For easier handling of malformed code
		GetPrivateProfileString("QuickPatches", KeyName, !"\0", @ValueBuffer, SizeOf(ValueBuffer), INIFile)
		Dim p As Integer = 0
		Do While ValueBuffer[p]
			Dim AddrString As String
			Do While ValueBuffer[p] AndAlso ValueBuffer[p] <> Asc(",")
				AddrString += Chr(ValueBuffer[p])
				p += 1
			Loop
			If ValueBuffer[p] = Asc(",") Then p += 1
			Dim Address As UByte Ptr = ValUInt("&h" & AddrString)
			If Address = 0 Then
				MessageBox(NULL, "Error in quick patch """ & *KeyName & """: Malformed address """ & AddrString & """!", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
				Exit Do
			EndIf
			If ValueBuffer[p] = 0 OrElse ValueBuffer[p] = Asc(",") Then
				MessageBox(NULL, "Error in quick patch """ & *KeyName & """: Empty byte string for address """ & AddrString & """!", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
			EndIf
			If ValueBuffer[p] = Asc("%") OrElse ValueBuffer[p] = Asc("#") Then
				Var UseInt32 = IIf(ValueBuffer[p] = Asc("#"), TRUE, FALSE)
				p += 1
				Dim ValueString As String
				Do While ValueBuffer[p] AndAlso ValueBuffer[p] <> Asc(",")
					If ValueBuffer[p] = Asc(" ") Then
						p += 1
						Continue Do
					EndIf
					ValueString += Chr(ValueBuffer[p])
					p += 1
				Loop
				Var Value = ValUInt(ValueString)
				If Address = &h45CE00 OrElse (UseInt32 AndAlso ((CUInt(Address) + 3) And (Not 3) = &h45CE00))  Then
					MessageBox(NULL, "Error in quick patch """ & *KeyName & !""": Trying to write to protected address ""45CE00""!\r\nA possible cause would be the comparison of a DynRPG-patched RPG_RT.exe against a non-patched one at the time the IPS file was created.", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
					Exit Do
				EndIf
				If Address < &h400000 OrElse Address >= &h4CF000 - 3 Then
					MessageBox(NULL, "Error in quick patch """ & *KeyName & """: Address """ & Hex(Address) & !""" is outside of limits!", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
					Exit Do
				EndIf
				
				If UseInt32 Then
					*CPtr(UInteger Ptr, Address) = Value
				Else
					*Address = Value
				EndIf
			Else
				Do While ValueBuffer[p] AndAlso ValueBuffer[p] <> Asc(",")
					If ValueBuffer[p] = Asc(" ") Then
						p += 1
						Continue Do
					EndIf
					Var ByteStr = Chr(ValueBuffer[p]) + Chr(ValueBuffer[p + 1])
					Var ByteVal = CUByte("&h" & ByteStr)
					If ByteStr[1] = 0 OrElse ByteStr[1] = Asc(",") OrElse (ByteVal = 0 AndAlso ByteStr <> "00") OrElse (ByteStr[0] <> Asc("0") AndAlso ByteVal < 10) Then
						MessageBox(NULL, "Error in quick patch """ & *KeyName & """: Malformed byte value """ & ByteStr & """!", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
						Exit Do, Do
					EndIf
					
					If Address = &h45CE00 Then
						MessageBox(NULL, "Error in quick patch """ & *KeyName & !""": Trying to write to protected address ""45CE00""!\r\nA possible cause would be the comparison of a DynRPG-patched RPG_RT.exe against a non-patched one at the time the IPS file was created.", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
						Exit Do, Do
					EndIf
					If Address < &h400000 OrElse Address >= &h4CF000 Then
						MessageBox(NULL, "Error in quick patch """ & *KeyName & """: Address """ & Hex(Address) & !""" is outside of limits!", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
						Exit Do, Do
					EndIf
					*Address = ByteVal ' Write changed data to memory
					
					p += 2
					Address += 1
				Loop
			EndIf
			If ValueBuffer[p] = Asc(",") Then p += 1
		Loop
		KeyName += Len(*KeyName) + 1
	Loop
End Sub

Function OffsetToVA(offs As UInteger) As UByte Ptr
	Select Case offs
		Case &h00000 To &h003FF
			Return offs + &h400000
		Case &h00400 To &hC9200
			Return offs + &h400C00
		Case &hC9200 To &hCD400
			Return offs + &h400E00
		Case Else
			Return NULL
	End Select
End Function

Sub ApplyIPSPatch(filename As String)
	Var file = FileObj.File(ExePathCached & "\DynPatches\" & filename, FileObj.MODE_BINARY)
	If file.ReadString(5) <> "PATCH" Then
		MessageBox(NULL, "Error in patch file """ & filename & """: Invalid file header! Not an IPS file?", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
		Return
	EndIf
	Do Until file.EndReached()
		Dim OffsetInt As UInteger = 0
		For i As Integer = 0 To 2
			OffsetInt Shl= 8
			OffsetInt Or= file.ReadByte()
		Next
		If OffsetInt = &h454F46 Then Exit Do ' EOF
		Dim Length As UShort = 0
		For i As Integer = 0 To 1
			Length Shl= 8
			Length Or= file.ReadByte()
		Next
		Dim Bytes As String
		If Length = 0 Then
			For i As Integer = 0 To 1
				Length Shl= 8
				Length Or= file.ReadByte()
			Next
			Bytes = String(Length, file.ReadString(1))
		Else
			Bytes = file.ReadString(Length)
		EndIf
		For i As Integer = 0 To Length - 1
			Dim Address As UByte Ptr = OffsetToVA(OffsetInt + i)
			If Address = &h45CE00 Then
				MessageBox(NULL, "Error in patch file """ & filename & !""": Trying to write to protected address ""45CE00""!\r\nA possible cause would be the comparison of a DynRPG-patched RPG_RT.exe against a non-patched one at the time the IPS file was created.", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
				Return
			EndIf
			If Address = NULL Then
				MessageBox(NULL, "Error in patch file """ & filename & """: Offset """ & Hex(OffsetInt + i) & !""" is outside of limits!", "DynRPG Loader", MB_OK Or MB_ICONWARNING)
				Return
			EndIf
			
			*Address = Bytes[i] ' Write changed data to memory
		Next
	Loop
End Sub

Sub OnStartup()
	WindowTitle("DynRPG Loader")
	' Trick to reduce the "black window" moment
	ScreenRes 1, 1, 32, 1, FB.GFX_SHAPED_WINDOW Or FB.GFX_NO_FRAME Or FB.GFX_NO_SWITCH Or FB.GFX_ALWAYS_ON_TOP
	ScreenRes 320, 240, 32, 1, FB.GFX_SHAPED_WINDOW Or FB.GFX_NO_FRAME Or FB.GFX_NO_SWITCH Or FB.GFX_ALWAYS_ON_TOP
	Color &hFFFFFF, &hFF00FF
	Cls()
	
	Color &hFF8000
	Dim st As String = Space(10)
	st[0] = Asc("P") - 55
	st[1] = Asc("o") - 55
	st[2] = Asc("w") - 55
	st[3] = Asc("e") - 55
	st[4] = Asc("r") - 55
	st[5] = Asc("e") - 55
	st[6] = Asc("d") - 55
	st[7] = Asc(" ") - 55
	st[8] = Asc("b") - 55
	st[9] = Asc("y") - 55
	For i As Integer = 0 To 9
		st[i] += 55
	Next
	Var text0 = StrToImg(st) 'StrToImg("Powered by")
	Color &hFF0000
	st = Space(6)
	st[0] = Asc("D") - 66
	st[1] = Asc("y") - 66
	st[2] = Asc("n") - 66
	st[3] = Asc("R") - 66
	st[4] = Asc("P") - 66
	st[5] = Asc("G") - 66
	For i As Integer = 0 To 5
		st[i] += 66
	Next
	Var text1 = StrToImg(st) 'StrToImg("DynRPG")
	Color &h00FE00
	st = Space(9)
	st[0] = Asc("B") - 77
	st[1] = Asc("y") - 77
	st[2] = Asc(" ") - 77
	st[3] = Asc("C") - 77
	st[4] = Asc("h") - 77
	st[5] = Asc("e") - 77
	st[6] = Asc("r") - 77
	st[7] = Asc("r") - 77
	st[8] = Asc("y") - 77
	For i As Integer = 0 To 8
		st[i] += 77
	Next
	Var text2 = StrToImg(st) 'StrToImg("By Cherry")
	Var icon = GetIcon()
	
	Var IsTestPlay = InStr(*GetCommandLine(), "TestPlay") > 0
	
	For i As Integer = 0 To 200
		If IsTestPlay AndAlso i <> 50 Then Continue For
		ScreenLock()
		Color &hFFFFFF, &hFF00FF
		Cls()
		MultiPut(0, 60 + i * 2, 9, text0, 1, 1, i * 8 - 400, 1)
		MultiPut(0, 110 + i, i * 2 - 50, text1, 3 + i / 50, 3 + i / 50, 200 - i * 4, 1)
		MultiPut(0, 210 - i, 360 - i * 3, text2, 3 - i / 50, 3 - i / 50, i * 6 - 300, 1)
		MultiPut(0, 160, 125, icon, IIf(i < 50, i / 5, 10 - (i - 50) / 15), IIf(i < 50, i / 5, 10 - (i - 50) / 15), 50 * 12 - i * 12, 1)
		ScreenUnLock()
		If i Mod 5 = 0 Then Sleep(20, 1)
		
		If i = 50 AndAlso IsTestPlay = FALSE Then
			For i As Integer = -1 To 4
				If i > -1 Then
					ScreenLock()
					For y As Integer = 0 To 239
						For x As Integer = 0 To 319
							Var c = Point(x, y)
							If c <> &hFF00FF Then
								PSet (x, y), c Xor -1
							EndIf
						Next
					Next
					ScreenUnLock()
				EndIf
				Sleep(300, 1)
			Next
		EndIf
	Next
	
	If IsTestPlay Then Sleep(1000, 1)
	
	Screen 0
	Sleep(100, 1)
	
	ThreadCall WindowChecker()
	
	' Apply patches
	Var IPSFileName = Dir(ExePathCached & "\DynPatches\*.ips")
	Do While IPSFileName <> ""
		ApplyIPSPatch(IPSFileName)
		IPSFileName = Dir()
	Loop
	ApplyQuickPatches()
	
	
	Dim FnNames(...) As String = { _
			"onStartup", _
			"onInitFinished", _
			"onInitTitleScreen", _
			"onNewGame", _
			"onLoadGame", _
			"onSaveGame", _
			"onExit", _
			"onFrame", _
			"onSetVariable", _
			"onGetVariable", _
			"onSetSwitch", _
			"onGetSwitch", _
			"onEventCommand", _
			"onComment", _
			"onDrawScreen", _
			"onDrawPicture", _
			"onPictureDrawn", _
			"onCheckEventVisibility", _
			"onDrawEvent", _
			"onEventDrawn", _
			"onDrawBattler", _
			"onBattlerDrawn", _
			"onDrawBattleActionWindow", _
			"onBattleStatusWindowDrawn", _
			"onDrawBattleActionWindow", _
			"onDoBattlerAction", _
			"onBattlerActionDone", _
			"onSystemBackgroundDrawn" _
	}
	
	Var p = ExePathCached & "\DynPlugins"
	Var s = Dir(p & "\*.dll")
	For Each(Plugin In Plugins)
		If s = "" Then Exit For
		Plugin = LoadLibrary(p & "\" & s)
		If Plugin = NULL Then
			Plugin = NULL
			MessageBox(NULL, "Loading plugin """ & s & """ failed! (" & GetLastError() & ")", "DynRPG Loader", MB_OK Or MB_ICONERROR)
		Else
			Dim P_LinkVersion As Integer Ptr = GetProcAddress(Plugin, "linkVersion")
			If P_LinkVersion = NULL Then
				If InStr(s, ".plugin.") Then
					MessageBox(NULL, "Plugin """ & s & """ is invalid!", "DynRPG Loader", MB_OK Or MB_ICONERROR)
				EndIf
				FreeLibrary(Plugin)
				Plugin = NULL
			ElseIf *P_LinkVersion > LINK_VERSION Then
				MessageBox(NULL, "Plugin """ & s & !""" is not compatible with this version of DynRPG!\r\nPlugin link version: " &  *P_LinkVersion & !"\r\nLoader link version: " & LINK_VERSION, "DynRPG Loader", MB_OK Or MB_ICONERROR)
				FreeLibrary(Plugin)
				Plugin = NULL
			Else
				PluginNames(__index) = Left(s, Len(s) - 4)
				PluginLinkVersions(__index) = *P_LinkVersion
				For i As Integer = 0 To UBound(FnNames)
					PluginFn(__index, i) = GetProcAddress(Plugin, FnNames(i))
				Next
				Dim P_OnStartup As Function Cdecl (As ZString Ptr) As Boolean = PluginFn(__index, 0)
				If P_OnStartup Then
					ChDir(ExePathCached)
					If P_OnStartup(StrPtr(PluginNames(__index))) = FALSE Then
						OnExit()
						End(29)
					EndIf
				EndIf
			EndIf
		EndIf
		s = Dir()
	Next
End Sub

Sub OnInitFinished()
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnInitFinished As Sub Cdecl () = PluginFn(__index, 1)
			If P_OnInitFinished Then
				ChDir2(ExePathCached)
				P_OnInitFinished()
			EndIf
		EndIf
	Next
End Sub

Sub OnInitTitleScreen()
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnInitTitleScreen As Sub Cdecl () = PluginFn(__index, 2)
			If P_OnInitTitleScreen Then
				ChDir2(ExePathCached)
				P_OnInitTitleScreen()
			EndIf
		EndIf
	Next
End Sub

Sub OnNewGame()
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnNewGame As Sub Cdecl () = PluginFn(__index, 3)
			If P_OnNewGame Then
				ChDir2(ExePathCached)
				P_OnNewGame()
			EndIf
		EndIf
	Next
End Sub

Sub OnLoadGame(id As Integer)
	If StopCallbacks Then Return
	ChDir(ExePathCached)
	Var file = FileObj.File("Save" & Format(id, "00") & ".dyn", FileObj.MODE_BINARY)
	Dim fileok As Boolean
	If file.ReadString(8) = "DYNSAVE1" AndAlso file.length() > 8 Then fileok = TRUE 
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim plugindata As String = ""
			If fileok Then
				file.position = 9
				Do Until file.EndReached()
					Var pname = file.ReadStringAuto()
					Var pdata = file.ReadStringAuto()
					If pname = PluginNames(__index) Then
						plugindata = pdata
						Exit Do
					EndIf
					'If file.EndReached() Then
					'	file.position = 9
					'EndIf
				Loop
			EndIf
			Dim P_OnLoadGame As Sub Cdecl (As Integer, As UByte Ptr, As Integer) = PluginFn(__index, 4)
			If P_OnLoadGame Then
				ChDir2(ExePathCached)
				P_OnLoadGame(id, StrPtr(plugindata), Len(plugindata))
			EndIf
		EndIf
	Next
	file.CloseF()
End Sub

Sub SavePluginData Cdecl (pdata As ZString Ptr, length As Integer)
	PluginData = String(length, 0)
	CopyMemory(StrPtr(PluginData), pdata, length)
End Sub

Sub OnSaveGame(id As Integer)
	If StopCallbacks Then Return
	ChDir(ExePathCached)
	Var file = FileObj.File("Save" & Format(id, "00") & ".dyn", FileObj.MODE_BINARY)
	file.WriteString("DYNSAVE1")
	For Each(Plugin In Plugins)
		If Plugin Then
			PluginData = ""
			Dim P_OnSaveGame As Sub Cdecl (As Integer, As Any Ptr) = PluginFn(__index, 5)
			If P_OnSaveGame Then
				ChDir2(ExePathCached)
				P_OnSaveGame(id, @SavePluginData)
			EndIf
			file.WriteStringAuto(PluginNames(__index))
			file.WriteStringAuto(PluginData)
		EndIf
	Next
End Sub

Sub OnExit()
	If StopCallbacks Then Return
	StopCallbacks = TRUE
	
	ShowWindow(hWndMain, SW_HIDE)
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnExit As Sub Cdecl () = PluginFn(__index, 6)
			If P_OnExit Then
				ChDir2(ExePathCached)
				P_OnExit()
			EndIf
		EndIf
	Next
	
	Sleep(300, TRUE)
	
	For Each(Plugin In Plugins)
		If Plugin Then
			FreeLibrary(Plugin)
			Plugin = NULL
		EndIf
	Next
End Sub

Sub OnFrame(scene As Integer)
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnFrame As Sub Cdecl (As Integer) = PluginFn(__index, 7)
			If P_OnFrame Then
				ChDir2(ExePathCached)
				P_OnFrame(scene)
			EndIf
		EndIf
	Next
End Sub

Function OnSetVariable(id As Integer, value As Integer) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnSetVariable As Function Cdecl (As Integer, As Integer) As Boolean = PluginFn(__index, 8)
			If P_OnSetVariable Then
				ChDir2(ExePathCached)
				If P_OnSetVariable(id, value) = FALSE Then Return FALSE
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Sub OnGetVariable(id As Integer)
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnGetVariable As Function Cdecl (As Integer) As Boolean = PluginFn(__index, 9)
			If P_OnGetVariable Then
				ChDir2(ExePathCached)
				If P_OnGetVariable(id) = FALSE Then Return
			EndIf
		EndIf
	Next
End Sub

Function OnSetSwitch(id As Integer, value As Boolean) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnSetSwitch As Function Cdecl (As Integer, As Boolean) As Boolean = PluginFn(__index, 10)
			If P_OnSetSwitch Then
				ChDir2(ExePathCached)
				If P_OnSetSwitch(id, value) = FALSE Then Return FALSE
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Sub OnGetSwitch(id As Integer)
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnGetSwitch As Function Cdecl (As Integer) As Boolean = PluginFn(__index, 11)
			If P_OnGetSwitch Then
				ChDir2(ExePathCached)
				If P_OnGetSwitch(id) = FALSE Then Return
			EndIf
		EndIf
	Next
End Sub

Function OnEventCommand(scriptLine As Any Ptr, scriptData As Any Ptr, eventId As Integer, pageId As Integer, lineId As Integer, nextLineId As Integer Ptr) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnEventCommand As Function Cdecl (As Any Ptr, As Any Ptr, As Integer, As Integer, As Integer, As Integer Ptr) As Boolean = PluginFn(__index, 12)
			If P_OnEventCommand Then
				ChDir2(ExePathCached)
				If P_OnEventCommand(scriptLine, scriptData, eventId, pageId, lineId, nextLineId) = FALSE Then Return FALSE
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Sub ParseComment(text As ZString Ptr)
	With CommentData
		.command[0] = 0
		.parametersCount = 0
		If text = NULL Then Return
		If text[0] <> Asc("@") Then Return
		Dim p As Integer = 0
		Dim strend As Boolean = FALSE
		.command[SizeOf(.command) - 1] = 0
		Do
			p += 1
			Select Case (*text)[p]
				Case 0
					strend = TRUE
					Exit Do
				Case Asc(" ")
					Exit Do
				Case Asc("A") To Asc("Z")
					If p < SizeOf(.command) - 1 Then
						.command[p - 1] = (*text)[p] + 32
					EndIf
				Case Else
					If p < SizeOf(.command) - 1 Then
						.command[p - 1] = (*text)[p]
					EndIf
			End Select
		Loop
		If p < SizeOf(.command) - 1 Then
			.command[p - 1] = 0
		EndIf
		If strend = FALSE Then
			Dim v_deref As Integer = 0
			Dim n_deref As Boolean = FALSE
			Do Until strend
				If .parametersCount >= UBound(.parameters) Then
					strend = TRUE
					Exit Do
				EndIf
				p += 1
				Select Case (*text)[p]
					Case 0
						strend = TRUE
						Exit Do
					Case Asc(" "), 13, 10
						Continue Do
					Case Asc("0") To Asc("9"), Asc("."), Asc("-")
						' Number
						Static n As String * 20 = ""
						n = ""
						p -= 1
						Do
							p += 1
							Select Case (*text)[p]
								Case Asc(" "), 13, 10
									Continue Do
								Case 0
									strend = TRUE
									Exit Do
								Case Asc(",")
									Exit Do
								Case Else
									n += Chr((*text)[p])
							End Select
						Loop
						Var d = Val(n)
						Do Until v_deref = 0
							d = GetVar(d)
							v_deref -= 1
						Loop
						If n_deref Then
							n_deref = FALSE
							With .parameters(.parametersCount)
								.ptype = PARAM_STRING
								.text = GetHeroName(d)
							End With
						Else
							With .parameters(.parametersCount)
								.ptype = PARAM_NUMBER
								.number = d
							End With
						EndIf
						.parametersCount += 1
					Case Asc("""")
						' String
						With .parameters(.parametersCount)
							.ptype = PARAM_STRING
							Dim sp As Integer = 0
							Dim paramend As Boolean = FALSE
							Do Until paramend OrElse strend ' ADDED: OrElse strend
								p += 1
								Select Case (*text)[p]
									Case 0
										strend = TRUE
										Exit Do
									Case 13, 10
										Continue Do
									Case Asc("""")
										If text[p + 1] = Asc("""") Then
											If sp < SizeOf(.text) - 1 Then
												.text[sp] = Asc("""")
												sp += 1
											EndIf
											p += 1
											Continue Do
										Else
											Do
												p += 1
												Select Case (*text)[p]
													Case 0
														strend = TRUE
														Exit Do
													Case Asc(",")
														paramend = TRUE
														Exit Do
												End Select
											Loop
										EndIf
									Case Else
										If sp < SizeOf(.text) - 1 Then
											.text[sp] = (*text)[p]
											sp += 1
										EndIf
								End Select
							Loop
							.text[sp] = 0
						End With
						.parametersCount += 1
						v_deref = 0
						n_deref = FALSE
					Case Asc("v"), Asc("V")
						' Variable
						v_deref += 1
						Continue Do
					Case Asc("n"), Asc("N")
						' Hero name
						If v_deref = 0 AndAlso n_deref = FALSE Then
							n_deref = TRUE
							Continue Do
						Else
							GoTo IsToken
						EndIf
					Case Else
						' Token
IsToken:
						With .parameters(.parametersCount)
							.ptype = PARAM_TOKEN
							Dim sp As Integer = v_deref
							If n_deref Then
								sp += 1
								.text = "n" + String(v_deref, Asc("v"))
							Else
								.text = String(v_deref, Asc("v"))								
							EndIf
							p -= 1
							Do
								p += 1
								Select Case (*text)[p]
									Case 0
										strend = TRUE
										Exit Do
									Case Asc(" "), 13, 10
										Continue Do
									Case Asc(",")
										Exit Do
									Case Asc("A") To Asc("Z")
										If sp < SizeOf(.text) - 1 Then
											.text[sp] = (*text)[p] + 32
											sp += 1
										EndIf
									Case Else
										If sp < SizeOf(.text) - 1 Then
											.text[sp] = (*text)[p]
											sp += 1
										EndIf
								End Select
							Loop
							If sp < SizeOf(.text) Then ' ADDED!
								.text[sp] = 0
							EndIf
						End With
						.parametersCount += 1
						v_deref = 0
						n_deref = FALSE
				End Select
			Loop
		EndIf
	End With
End Sub

Sub OnComment(text As ZString Ptr, nextScriptLine As Any Ptr, scriptData As Any Ptr, eventId As Integer, pageId As Integer, lineId As Integer, nextLineId As Integer Ptr)
	If StopCallbacks Then Return
	ParseComment(text)
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnComment As Function Cdecl (As ZString Ptr, As TCommentData Ptr, As Any Ptr, As Any Ptr, As Integer, As Integer, As Integer, As Integer Ptr) As Boolean = PluginFn(__index, 13)
			If P_OnComment Then
				ChDir2(ExePathCached)
				If P_OnComment(text, @CommentData, nextScriptLine, scriptData, eventId, pageId, lineId, nextLineId) = FALSE Then Return
			EndIf
		EndIf
	Next
End Sub

Sub OnDrawScreen()
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnDrawScreen As Sub Cdecl () = PluginFn(__index, 14)
			If P_OnDrawScreen Then
				ChDir2(ExePathCached)
				P_OnDrawScreen()
			EndIf
		EndIf
	Next
End Sub

Function OnDrawPicture(picture As Any Ptr) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnDrawPicture As Function Cdecl (As Any Ptr) As Boolean = PluginFn(__index, 15)
			If P_OnDrawPicture Then
				ChDir2(ExePathCached)
				If P_OnDrawPicture(picture) = FALSE Then Return FALSE
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Sub OnPictureDrawn(picture As Any Ptr)
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnPictureDrawn As Function Cdecl (As Any Ptr) As Boolean = PluginFn(__index, 16)
			If P_OnPictureDrawn Then
				ChDir2(ExePathCached)
				If P_OnPictureDrawn(picture) = FALSE Then Return
			EndIf
		EndIf
	Next
End Sub

Function OnCheckEventVisibility(event As Any Ptr) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnCheckEventVisibility As Function Cdecl (As Any Ptr) As Boolean = PluginFn(__index, 17)
			If P_OnCheckEventVisibility Then
				ChDir2(ExePathCached)
				If P_OnCheckEventVisibility(event) = FALSE Then Return FALSE
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Function OnDrawEvent(event As Any Ptr, isHero As Boolean) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnDrawEvent As Function Cdecl (As Any Ptr, As Boolean) As Boolean = PluginFn(__index, 18)
			If P_OnDrawEvent Then
				ChDir2(ExePathCached)
				If P_OnDrawEvent(event, isHero) = FALSE Then Return FALSE
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Sub OnEventDrawn(event As Any Ptr, isHero As Boolean)
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnEventDrawn As Function Cdecl (As Any Ptr, As Boolean) As Boolean = PluginFn(__index, 19)
			If P_OnEventDrawn Then
				ChDir2(ExePathCached)
				If P_OnEventDrawn(event, isHero) = FALSE Then Return
			EndIf
		EndIf
	Next
End Sub

Function OnDrawBattler(battler As Any Ptr, isMonster As Boolean, id As Integer) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnDrawBattler As Function Cdecl (As Any Ptr, As Boolean, As Integer) As Boolean = PluginFn(__index, 20)
			If P_OnDrawBattler Then
				ChDir2(ExePathCached)
				If P_OnDrawBattler(battler, isMonster, id) = FALSE Then Return FALSE
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Sub OnBattlerDrawn(battler As Any Ptr, isMonster As Boolean, id As Integer)
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnBattlerDrawn As Function Cdecl (As Any Ptr, As Boolean, As Integer) As Boolean = PluginFn(__index, 21)
			If P_OnBattlerDrawn Then
				ChDir2(ExePathCached)
				If P_OnBattlerDrawn(battler, isMonster, id) = FALSE Then Return
			EndIf
		EndIf
	Next
End Sub

Function OnDrawBattleStatusWindow(x As Integer, selection As Integer, selActive As Boolean, isTargetSelection As Boolean, isVisible As Boolean) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnDrawBattleStatusWindow As Function Cdecl (As Integer, As Integer, As Boolean, As Boolean, As Boolean) As Boolean = PluginFn(__index, 22)
			If P_OnDrawBattleStatusWindow Then
				ChDir2(ExePathCached)
				If P_OnDrawBattleStatusWindow(x, selection, selActive, isTargetSelection, isVisible) = FALSE Then Return FALSE
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Sub OnBattleStatusWindowDrawn(x As Integer, selection As Integer, selActive As Boolean, isTargetSelection As Boolean, isVisible As Boolean)
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnBattleStatusWindowDrawn As Function Cdecl (As Integer, As Integer, As Boolean, As Boolean, As Boolean) As Boolean = PluginFn(__index, 23)
			If P_OnBattleStatusWindowDrawn Then
				ChDir2(ExePathCached)
				If P_OnBattleStatusWindowDrawn(x, selection, selActive, isTargetSelection, isVisible) = FALSE Then Return
			EndIf
		EndIf
	Next
End Sub

Function OnDrawBattleActionWindow(x As Integer Ptr, y As Integer Ptr, selection As Integer, selActive As Boolean, isVisible As Boolean) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnDrawBattleActionWindow As Function Cdecl (As Integer Ptr, As Integer Ptr, As Integer, As Boolean, As Boolean) As Boolean = PluginFn(__index, 24)
			If P_OnDrawBattleActionWindow Then
				ChDir2(ExePathCached)
				If P_OnDrawBattleActionWindow(x, y, selection, selActive, isVisible) = FALSE Then Return FALSE
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Function OnDoBattlerAction(battler As Any Ptr, firstTry As Boolean) As Boolean
	If StopCallbacks Then Return TRUE
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnDoBattlerAction As Function Cdecl (As Any Ptr, As Boolean) As Boolean = PluginFn(__index, 25)
			If P_OnDoBattlerAction Then
				ChDir2(ExePathCached)
				' This also works with plugins with LINK_VERSION < 3 even though their onDoBattlerAction callback has a (RPG::Battler *) signature,
				' i.e. no firstTry parameter, due to CDecl calling convention under which the caller has to remove the parameters from the stack,
				' therefore the callee can safely get superfluous parameters passed without its knowing. 
				If P_OnDoBattlerAction(battler, firstTry) = FALSE Then Return FALSE 
			EndIf
		EndIf
	Next
	Return TRUE
End Function

Sub OnBattlerActionDone(battler As Any Ptr, successful As Boolean)
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnBattlerActionDone As Function Cdecl (As Any Ptr, As Boolean) As Boolean = PluginFn(__index, 26)
			If P_OnBattlerActionDone Then
				ChDir2(ExePathCached)
				If P_OnBattlerActionDone(battler, successful) = FALSE Then Return
			EndIf
		EndIf
	Next
End Sub

Sub OnSystemBackgroundDrawn(rect As RECT Ptr)
	If StopCallbacks Then Return
	For Each(Plugin In Plugins)
		If Plugin Then
			Dim P_OnSystemBackgroundDrawn As Function Cdecl (As RECT Ptr) As Boolean = PluginFn(__index, 27)
			If P_OnSystemBackgroundDrawn Then
				ChDir2(ExePathCached)
				If P_OnSystemBackgroundDrawn(rect) = FALSE Then Return
			EndIf
		EndIf
	Next
End Sub

Sub DynRPG_Init Alias "DynRPG_Init" (Version As Integer, FnArray As Any Ptr Ptr) Export
	ExePathCached = ExePath()
	ChDir(ExePathCached)
	If Version < LINK_VERSION Then
		MessageBox(NULL, !"DynRPG Patch version too old!\r\n" & "Patch link version: " & Version & !"\r\nLoader link version: " & LINK_VERSION, "DynRPG Loader", MB_OK Or MB_ICONERROR)
		End(34)
	EndIf
	Dim mbi As MEMORY_BASIC_INFORMATION
	VirtualQuery(&h401000, @mbi, SizeOf(mbi))
	Dim oldp As DWORD
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, @oldp)
	FnArray[0] = @OnStartup
	FnArray[1] = @OnInitFinished
	FnArray[2] = @OnInitTitleScreen
	FnArray[3] = @OnNewGame
	FnArray[4] = @OnLoadGame
	FnArray[5] = @OnSaveGame
	FnArray[6] = @OnExit
	FnArray[7] = @OnFrame
	FnArray[8] = @OnSetVariable
	FnArray[9] = @OnGetVariable
	FnArray[10] = @OnSetSwitch
	FnArray[11] = @OnGetSwitch
	FnArray[12] = @OnEventCommand
	FnArray[13] = @OnComment
	FnArray[14] = @OnDrawScreen
	FnArray[15] = @OnDrawPicture
	FnArray[16] = @OnPictureDrawn
	FnArray[17] = @OnCheckEventVisibility
	FnArray[18] = @OnDrawEvent
	FnArray[19] = @OnEventDrawn
	FnArray[20] = @OnDrawBattler
	FnArray[21] = @OnBattlerDrawn
	FnArray[22] = @OnDrawBattleStatusWindow
	FnArray[23] = @OnBattleStatusWindowDrawn
	FnArray[24] = @OnDrawBattleActionWindow
	FnArray[25] = @OnDoBattlerAction
	FnArray[26] = @OnBattlerActionDone
	FnArray[27] = @OnSystemBackgroundDrawn
End Sub
