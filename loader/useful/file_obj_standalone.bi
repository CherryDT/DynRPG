/'

Copyright (C) 2006-2017, David "Cherry" Trapp
All rights reserved.

1. Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the conditions laid out in the
   following paragraphs are met.
2. Redistributions of source code (if available) must retain the above
   copyright notice, this list of conditions and the following disclaimer.
3. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
  3.1. In case this software is redistributed in binary form inside of a
       executable or library file belonging to Third Parties and/or
       including modifications done by Third Parties (or done by the Author
       in Third Parties' names), any license restrictions imposed by the
       Third Parties which disallow modification and/or redistribution remain
       valid and have to be adhered, overriding the rights described in
       paragraph 1 of this license. This does not give the Third Parties
       the right to claim ownership of those parts of the software which
       were developed by the Author, nor does it affect the remaining
       parts of this license.
4. All advertising materials by Third Parties mentioning features or use
   of this software must display the following acknowledgement (unless
   agreed otherwise through written notice):
   This product includes software developed by David "Cherry" Trapp.

DISCLAIMER:
THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

'/

Namespace FileObj
	Enum FileMode
		MODE_NONE
		MODE_OUTPUT
		MODE_INPUT
		MODE_APPEND
		MODE_RANDOM
		MODE_BINARY
	End Enum
	
	Type File
		handle As Integer
		filename As String
		mode As FileMode
		valid As Boolean
		fieldlen As Integer
		Declare Constructor (filename As String, mode As FileMode, fieldlen As Integer = 1)
		Declare Destructor ()
		Declare Function WriteLine(text As String, newline As Boolean = TRUE) As Integer
		Declare Function ReadLine() As String
		Declare Function EndReached() As Boolean
		Declare Function WriteByte(value As UByte, position_ As LongInt = -1) As Integer
		Declare Function ReadByte(position_ As LongInt = -1) As UByte
		Declare Function WriteWord(value As UShort, position_ As LongInt = -1) As Integer
		Declare Function ReadWord(position_ As LongInt = -1) As UShort
		Declare Function WriteInt(value As Integer, position_ As LongInt = -1) As Integer
		Declare Function ReadInt(position_ As LongInt = -1) As Integer
		Declare Function WriteString(value As String, position_ As LongInt = -1) As Integer
		Declare Function ReadString(length_ As UInteger, position_ As LongInt = -1) As String
		Declare Function WriteStringAuto(value As String, position_ As LongInt = -1) As Integer
		Declare Function ReadStringAuto(position_ As LongInt = -1) As String
		Declare Property position() As LongInt
		Declare Property position(newpos As LongInt)
		Declare Function Length() As LongInt
		Declare Function Save() As Integer
		Declare Function CloseF() As Integer
	End Type
	
	Type Stream 
		stream As String
		position__ As LongInt
		filename As String
		Declare Constructor (filename As String, loadfromfile As Boolean = FALSE)
		Declare Constructor ()
		Declare Function ReadLine() As String
		Declare Function EndReached() As Boolean
		Declare Property position() As LongInt
		Declare Property position(newpos As LongInt)
		Declare Function Length() As LongInt
		Declare Function Save(filename As String) As Integer
		Declare Function Load(filename As String) As Integer
	End Type
	
	Constructor File(filename As String, mode As FileMode, fieldlen As Integer = 1)
		This.valid = FALSE
		This.handle = FreeFile
		If This.handle = 0 Then Return
		Select Case mode
			Case MODE_OUTPUT
				Open filename For Output As #(This.handle)
			Case MODE_INPUT
				Open filename For Input As #(This.handle)
			Case MODE_APPEND
				Open filename For Append As #(This.handle)
			Case MODE_RANDOM
				Open filename For Random As #(This.handle) Len = fieldlen
			Case MODE_BINARY
				Open filename For Binary As #(This.handle)
			Case Else
				Return
		End Select
		If Err = NULL Then This.valid = TRUE Else Return
		This.filename = filename
		This.mode = mode
		This.fieldlen = fieldlen
	End Constructor
	
	Destructor File()
		This.CloseF()
	End Destructor
	
	Function File.WriteLine(text As String, newline As Boolean = TRUE) As Integer
		If This.valid = FALSE Then Return -1
		If newline Then Print #(This.handle), text Else Print #(This.handle), text;
		Return Err 
	End Function
	
	Function File.ReadLine() As String
		If This.valid = FALSE Then Return ""
		Dim text As String
		Line Input #(This.handle), text
		Return text
	End Function
	
	Function File.EndReached() As Boolean
		If This.valid = FALSE Then Return Cast(Boolean, -1)
		Return Eof(This.handle)
	End Function
	
	Function File.WriteByte(value As UByte, position_ As LongInt = -1) As Integer
		If This.valid = FALSE Then Return -1
		If position_ = -1 Then Put #(This.handle), , value Else Put #(This.handle), position_, value
		Return Err
	End Function
	
	Function File.ReadByte(position_ As LongInt = -1) As UByte
		If This.valid = FALSE Then Return 0
		Dim value As UByte
		If position_ = -1 Then Get #(This.handle), , value Else Get #(This.handle), position_, value
		Return value  
	End Function
	
	Function File.WriteWord(value As UShort, position_ As LongInt = -1) As Integer
		If This.valid = FALSE Then Return -1
		If position_ = -1 Then Put #(This.handle), , value Else Put #(This.handle), position_, value
		Return Err
	End Function
	
	Function File.ReadWord(position_ As LongInt = -1) As UShort
		If This.valid = FALSE Then Return 0
		Dim value As UShort
		If position_ = -1 Then Get #(This.handle), , value Else Get #(This.handle), position_, value
		Return value  
	End Function
	
	Function File.WriteInt(value As Integer, position_ As LongInt = -1) As Integer
		If This.valid = FALSE Then Return -1
		If position_ = -1 Then Put #(This.handle), , value Else Put #(This.handle), position_, value
		Return Err
	End Function
	
	Function File.ReadInt(position_ As LongInt = -1) As Integer
		If This.valid = FALSE Then Return 0
		Dim value As Integer
		If position_ = -1 Then Get #(This.handle), , value Else Get #(This.handle), position_, value
		Return value 
	End Function
	
	Function File.WriteString(value As String, position_ As LongInt = -1) As Integer
		If This.valid = FALSE Then Return -1
		For i As Integer = 0 To Len(value) - 1
			This.WriteByte(value[i], position_ + IIf(position_ = -1, 0, i))
		Next
		Return Err
	End Function
	
	Function File.ReadString(length_ As UInteger, position_ As LongInt = -1) As String
		If This.valid = FALSE Then Return ""
		Dim value As String = String(length_, Chr(0))
		For i As Integer = 0 To length_ - 1
			value[i] = This.ReadByte(position_ + IIf(position_ = -1, 0, i))
		Next
		Return value 
	End Function
	
	Function File.WriteStringAuto(value As String, position_ As LongInt = -1) As Integer
		Var ret = This.WriteInt(Len(value), position_)
		If ret Then Return ret
		If Len(value) <> 0 Then Return This.WriteString(value)
		Return 0
	End Function
	
	Function File.ReadStringAuto(position_ As LongInt = -1) As String
		Var length_ = This.ReadInt(position_)
		If length_ > 0 Then Return This.ReadString(length_)
		Return ""
	End Function
	
	Property File.position() As LongInt
		If This.valid = FALSE Then Return -1
		Return Seek(This.handle)
	End Property
	
	Property File.position(newpos As LongInt)
		If This.valid = FALSE Then Return
		Seek #(This.handle), newpos
	End Property
	
	Function File.Length() As LongInt
		If This.valid = FALSE Then Return -1
		Return Lof(This.handle)
	End Function
	
	Function File.Save() As Integer
		If This.valid = FALSE Then Return -1
		Dim oldpos As LongInt = This.position
		Close #(This.handle)
		This.valid = FALSE
		Select Case mode
			Case MODE_OUTPUT, MODE_APPEND
				Open This.filename For Append As #(This.handle)
			Case MODE_INPUT
				Open This.filename For Input As #(This.handle)
			Case MODE_RANDOM
				Open This.filename For Random As #(This.handle) Len = This.fieldlen
			Case MODE_BINARY
				Open This.filename For Binary As #(This.handle)
			Case Else
				Return -1
		End Select
		If Err = NULL Then This.valid = TRUE Else Return Err
		This.position = oldpos
	End Function
	
	Function File.CloseF() As Integer
		If This.valid = FALSE Then Return -1
		Close #(This.handle)
		This.valid = FALSE
		Return Err
	End Function
	
	Constructor Stream(source As String, loadfromfile As Boolean = FALSE)
		This.stream = ""
		This.position__ = 1
		If loadfromfile = FALSE Then
			This.stream = source
			Return
		EndIf
		If FileExists(source) Then
			This.Load(source)
		EndIf
	End Constructor
	
	Constructor Stream()
		This.stream = ""
		This.position__ = 1
	End Constructor
	
	Function Stream.ReadLine() As String
		Var e = InStr(This.position, This.stream, !"\13\10")
		If e = 0 Then e = This.Length + 1
		Var ret = Mid(This.stream, This.position, e - This.position)
		This.position = e + 2
		Return ret
	End Function
	
	Function Stream.EndReached() As Boolean
		Return (This.position > This.Length) And TRUE
	End Function
	
	Property Stream.position() As LongInt
		Return This.position__
	End Property
	
	Property Stream.position(newpos As LongInt)
		This.position__ = min(max(1ll, newpos), This.Length + 1)
	End Property
	
	Function Stream.Length() As LongInt
		Return Len(This.stream)
	End Function
	
	Function Stream.Save(filename As String) As Integer
		Var f = File(filename, MODE_BINARY)
		If f.valid = FALSE Then Return -1
		f.WriteString(This.stream)
		This.filename = filename
	End Function
	
	Function Stream.Load(filename As String) As Integer
		Var f = File(filename, MODE_BINARY)
		If f.valid = FALSE Then Return -1
		This.stream = f.ReadString(f.Length())
		This.position__ = 1
		This.filename = filename
	End Function
End Namespace

