#Include "windows.bi"
#Include "fbgfx.bi"
#Include "string.bi"
#Include "file.bi"
#Include "useful/file_obj_standalone.bi"

#Macro Each(__iter__, __arr__)
        __index As Integer = LBound(__arr__) To UBound(__arr__)
        #Define __iter__ (__arr__(__index)) 
#EndMacro
#Define In ,

#Define ChDir2(_d_)

#Define MAX_PLUGIN_COUNT 50

Dim Shared Plugins(MAX_PLUGIN_COUNT - 1) As HMODULE
Dim Shared PluginNames(MAX_PLUGIN_COUNT - 1) As String
Dim Shared PluginLinkVersions(MAX_PLUGIN_COUNT - 1) As Integer
Dim Shared PluginFn(MAX_PLUGIN_COUNT - 1, 27) As Any Ptr
Dim Shared StopCallbacks As Boolean

Dim Shared KeyNamesBuffer As ZString * 32766 
Dim Shared ValueBuffer As ZString * 32766

Data 0, 0, 0, 0, 0, 0, 1, 1, 1
Data 0, 0, 0, 0, 0, 1, 0, 0, 1
Data 0, 1, 0, 0, 1, 0, 0, 0, 1
Data 1, 0, 1, 1, 0, 0, 0, 1, 0
Data 1, 0, 1, 0, 0, 0, 1, 0, 0
Data 0, 1, 0, 0, 0, 1, 0, 0, 0
Data 0, 1, 0, 0, 1, 1, 0, 0, 0
Data 1, 0, 1, 1, 0, 0, 1, 0, 0
Data 1, 1, 0, 0, 1, 1, 0, 0, 0

Declare Sub OnExit()

Dim Shared hWndMain As HWND

Dim Shared PluginData As String

Dim Shared ExePathCached As String

Dim Shared CallbackStack As Integer = 0

Enum EParamType
	PARAM_NUMBER
	PARAM_STRING
	PARAM_TOKEN
End Enum

Type CommentParameter
	ptype As EParamType
	number As Double
	text As ZString * 200
End Type

Type TCommentData
	command As ZString * 200
	parametersCount As Integer
	parameters(99) As CommentParameter
End Type

Dim Shared CommentData As TCommentData

Dim Shared LStrClr As Any Ptr = &h4044ec
Dim Shared GetFromList As Any Ptr = &h475740
Dim Shared GetVarP As Any Ptr = &h48b398

Function GetVar(id As Integer) As Integer
	Asm
		mov eax, [&h4cdc7c]
		mov eax, [eax]
		mov edx, [id]
		call [GetVarP]
		mov [Function], eax
	End Asm
End Function

Function GetHeroName(id As Integer) As String
	Dim s As ZString Ptr
	Asm
		mov eax, [&h4cddc8]
		mov eax, [eax]
		mov edx, [id]
		call [GetFromList]
		test eax, eax
		jz Cont
		mov ecx, [eax]
		lea edx, [s]
		call [ecx+24]
		Cont:
	End Asm
	
	If s = 0 Then Return ""
	
	Function = *s
	
	Asm
		lea eax, [s]
		call [LStrClr]
	End Asm
End Function

