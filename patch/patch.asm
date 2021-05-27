; DynRPG Patch

; FOR COMPATIBILITY: AFTER REMOVING PATCHES, WRITE THE ORIGINAL COMMANDS AS PATCH!!! (And also put them into the HPD)
; 442600..443A70 (442600..442680 < vars for DynRPG lib - 443900..4439A0 < CommonThisEventPatch)
; 45CDF4..45D84C

<0045CDF4>

@JmpHookOnDoBattlerAction:
jmp @HookOnDoBattlerAction ; For RPG::Battler::executeAction(false)

<00442600>

@BattleSpeed:
dd 100.
@TransparentWindowsEverywhere:
dd 0

<0045CE00>

@LINK_VERSION:
dd 3 ; LINK_VERSION

@DynRPGLoaderDLL:
dd 0 ; DynRPGLoaderDLL

@DynRPGVTable:
@OnStartup:
dd 0 ; DynRPGVTable
@OnInitFinished:
dd 0
@OnInitTitleScreen:
dd 0
@OnNewGame:
dd 0
@OnLoadGame:
dd 0
@OnSaveGame:
dd 0
@OnExit:
dd 0
@OnFrame:
dd 0
@OnSetVariable:
dd 0
@OnGetVariable:
dd 0
@OnSetSwitch:
dd 0
@OnGetSwitch:
dd 0
@OnEventCommand:
dd 0
@OnComment:
dd 0
@OnDrawScreen:
dd 0
@OnDrawPicture:
dd 0
@OnPictureDrawn:
dd 0
@OnCheckEventVisibility:
dd 0
@OnDrawEvent:
dd 0
@OnEventDrawn:
dd 0
@OnDrawBattler:
dd 0
@OnBattlerDrawn:
dd 0
@OnDrawBattleStatusWindow:
dd 0
@OnBattleStatusWindowDrawn:
dd 0
@OnDrawBattleActionWindow:
dd 0
@OnDoBattlerAction:
dd 0
@OnBattlerActionDone:
dd 0
@OnSystemBackgroundDrawn:
dd 0

@CurrentEventID:
dd 0
@CurrentEventPage:
dd 0
@CurrentEventScriptLine:
dd 0

@ReadyForExit:
dd 0
@NextLineOffset:
dd 0
@AutoResetEvCmd:
dd 0

@LastEventScriptLine:
dd 0
@SavedEventScriptLine:
dd 0 ; vTable
dd 0 ; command
dd 0 ; treeDepth
dd 0 ; stringParameter
dd 0 ; parameters.size
dd 0 ; parameters.array

@LoadDLL:
pushad ; HookOnStartup proc
push @DLLName
call 406c14 ; LoadLibraryA
test eax, eax
jz @DLLError
push @FnName
push eax
call 406b5c ; GetProcAddress
test eax, eax
jz @DLLError
push @DynRPGVTable
push [@LINK_VERSION]
call eax
call [@OnStartup]
popad
retn

@DLLError:
push 0
push @ErrDLLNotFoundTitle
push @ErrDLLNotFound
push 0
call 4012dc ; MessageBoxA
push 0
call 4012d4 ; ExitProcess
int3

@DLLName:
"dynloader.dll\0"

@FnName:
"DynRPG_Init@8\0"

@ErrDLLNotFound:
"DynRPG Loader (dynloader.dll) could not be loaded!\0"

@ErrDLLNotFoundTitle:
"DynRPG Patch\0"


@MorePictures:
push edi ; MorePictures proc
mov edi, [esp+8]
call [@PictureCallTable+edi*4]
pop edi
inc ebx
cmp ebx, 2001.
retn 4

@PictureCallTable:
dd 4755b8 ; PreparePicture
dd 4c1c20 ; UpdatePicture
dd @HookOnDrawPicture ; DrawPicture
dd 4c1bf8 ; ErasePicture
dd 4c21d4 ; PictureOperation5One
dd 4c2220 ; PictureOperation6One
dd 4c226c ; PictureOperation7One
dd 4c22b8 ; PictureOperation8One


@HookOnStartup:
call @LoadDLL ; HookOnStartup proc
jmp 406910 ; Sysinit::InitExe


@HookOnSetVariable:
pushad ; HookOnSetVariable proc
push ecx
push edx
call [@OnSetVariable]
test al, al
popad
jz 48b3d3 ; For compatibility with CustomSaveLoadPatch
mov esi, edx
mov ebx, eax
cmp esi, 1
jmp 48b3bc ; For compatibility with CustomSaveLoadPatch


@HookOnGetVariable:
pushad ; HookOnGetVariable proc
push edx
call [@OnGetVariable]
popad
cmp edx, 1
jl 48b3a2
jmp 48b39d


@HookOnSetSwitch:
pushad ; HookOnSetSwitch proc
push ecx
push edx
call [@OnSetSwitch]
test al, al
popad
jz 48b362
push ebx
push esi
push edi
mov ebx, ecx
jmp 48b341


@HookOnGetSwitch:
pushad ; HookOnGetSwitch proc
push edx
call [@OnGetSwitch]
popad
cmp edx, 1
jl 48b32e
jmp 48b329


@HookOnDrawScreen:
pushad ; HookOnDrawScreen
mov eax, [4cdd4c] ; DebugParamPtr - is the last to be initialized, thus the testing here
mov eax, [eax]
test eax, eax
jz @NoOUSHook
call [@OnDrawScreen]

@NoOUSHook:
popad
push ebp
mov ebp, esp
push ecx
mov [ebp-4], eax
jmp 46bc33


@HookOnDrawPicture:
mov edx, [eax+10] ; HookOnDrawPicture proc
mov edx, [edx+608]
test edx, edx
jz @HODP_Ret2
push eax
pushad
push eax
call [@OnDrawPicture]
test al, al
popad
jz @HODP_Ret
call 4c1e6c ; DrawPicture

@HODP_Ret:
call [@OnPictureDrawn]

@HODP_Ret2:
retn


@CopyEventScriptLine:
push ebp
mov ebp, esp
mov esi, [arg.1]
mov edi, [arg.2]
cmp byte ptr [arg.3], 0
je @NoCCheck
cmp dword ptr [edi], 4791d0
jne @AbortCESL
cmp dword ptr [edi+4], 0
je @AbortCESL
cmp dword ptr [esi+4], 0
je @AbortCESL

@NoCCheck:
mov eax, [esi+4]
mov [edi+4], eax
mov eax, [esi+8]
mov [edi+8], eax
lea eax, [edi+0c]
mov edx, [esi+0c]
call 404540 ; LStrAsg 
mov edx, [esi+10]
push edx
cmp byte ptr [arg.3], 0
je @AlwaysResize
cmp edx, [edi+10]
jle @DontResize

@AlwaysResize:
cmp edx, [edi+10]
pushfd
mov [edi+10], edx
popfd
jle @DontResize
lea eax, [edi+14]
shl edx, 2
call 4027ac ; ResizeArray

@DontResize:
pop ecx
mov esi, [esi+14]
mov edi, [edi+14]
test edi, edi
jz @AbortCESL
test esi, esi
jz @AbortCESL
rep movsd

@AbortCESL:
mov esp, ebp
pop ebp
retn 0c


@HookOnEventCommand:
pushad ; HookOnEventCommand proc
mov [@CurrentEventScriptLine], edx
push [ebx+1c]
pop [@CurrentEventID]
mov dword ptr [@CurrentEventPage], 0
mov eax, [4cdd74]
mov eax, [eax]
mov eax, [eax+18]
mov edx, [ebx+1c]
call 475740 ; GetFromList
test eax, eax
jz @SEIDVCont
call 4ab4f0 ; GetActiveEventPage
mov [@CurrentEventPage], eax

@SEIDVCont:
popad
call 479710 ; GetEventScriptLine
and dword ptr [eax+4], 7fffffff
pushad
push eax
mov dword ptr [@NextLineOffset], -1
mov dword ptr [@AutoResetEvCmd], 1 ; SET TO ZERO TO DISABLE AUTO RESET
mov [@LastEventScriptLine], eax
pushad
push 0
push @SavedEventScriptLine
push eax
call @CopyEventScriptLine
popad
push @NextLineOffset
push [@CurrentEventScriptLine]
push [@CurrentEventPage]
push [@CurrentEventID]
push ebx
push eax
call [@OnEventCommand]
test al, al
pop eax
jnz @SEIDVHookOK
or dword ptr [eax+4], 80000000

@SEIDVHookOK:
popad
retn


@HookOnEventCommandExecuted:
mov edx, [@NextLineOffset] ; HookOnEventCommandExecuted proc
cmp edx, -1
je @NoOffsetChange
cmp ebx, 0ffff ; If this code path is reached without any command executed, ebx is no valid pointer but a counter instead
jg @NoOffsetChange
mov [ebx+18], edx
cmp edx, [@NextLineOffset]
jne @NoOffsetChange
mov dword ptr [esi+14], 1 ; do a Wait 0.0 if we loop back to where we were otherwise the game may hang

@NoOffsetChange:
mov dl, [@AutoResetEvCmd]
test dl, dl
jz @NoAutoReset
cmp edi, [@LastEventScriptLine]
jne @NoAutoReset
cmp dword ptr [@SavedEventScriptLine+4], 0
je @NoAutoReset
cmp dword ptr [@LastEventScriptLine], 0
je @NoAutoReset
pushad
push 1
push edi
push @SavedEventScriptLine
call @CopyEventScriptLine
popad

@NoAutoReset:
mov dword ptr [@LastEventScriptLine], 0
pop edx
pop edi
pop esi
pop ebx
retn


@HookOnComment:
push ebx ; HookOnCommentProc
mov ebx, edx
push 0
push 0
lea eax, [esp]
mov edx, [ecx+0c]
call 404540 ; LStrAsg 

@L00000002:
mov edx, [ebx+18]
mov eax, [ebx+14]
call 479710 ; GetEventScriptLine
mov [esp+4], eax
mov ecx, eax
cmp dword ptr [ecx+4], 22410.
jnz @L00000003
push ecx
lea eax, [esp+4]
mov edx, @NewLine
call 4047ac ; LStrCat
pop ecx
lea eax, [esp]
mov edx, [ecx+0c]
call 4047ac ; LStrCat
inc dword ptr [ebx+18]
jmp @L00000002

@L00000003:
lea eax, [esp]
lea edx, [esp+4]
push @NextLineOffset
push [@CurrentEventScriptLine]
push [@CurrentEventPage]
push [@CurrentEventID]
push ebx
push [edx]
push [eax]
call [@OnComment]
lea eax, [esp]
call 4044ec ; LStrClr
pop eax
pop eax
pop ebx
retn

dd 0ffffffff
dd 2.
@NewLine:
"\r\n\0"


@HookOnInitFinished:
call 48ede4 ; HookOnInitFinished proc ; TLcfDebugScene__Constructor
pushad
call [@OnInitFinished]
popad
retn


@HookOnNewGame:
call 4a5578 ; HookOnNewGame proc
pushad
mov eax, [4cdf3c] ; LcfgPicture
mov eax, [eax]
push 1001.  ; Erase pictures > 1000 on new game!
push 2000.
call @ErasePictures
call [@OnNewGame]
popad
retn


@HookOnLoadGame:
push edx ; HookOnLoadGame proc
call 4a541c ; LoadGame
call [@OnLoadGame]
retn


@HookOnSaveGame:
push edx ; HookOnSaveGame proc
call 4a5524 ; SaveGame
pop edx
pushad
push edx
call [@OnSaveGame]
popad
retn


@HookOnExit:
cmp dword ptr [ebx], WM_CLOSE
jne 439efc
cmp dword ptr [@ReadyForExit], 1
je @HOE_Ret
pushad ; HookOnExit proc
mov dword ptr [@ReadyForExit], 1
call [@OnExit]
popad
jmp 439efc

@HOE_Ret:
retn


@HookOnDrawEvent:
push eax ; HookOnDrawEvent proc
pushad
push 0
push eax
call [@OnDrawEvent]
test al, al
popad
jz @HODE_Ret
cmp dword ptr [eax+9c], 0
je @HODE_Ret
call 4c5b98 ; TLcfgCharacter_Draw

@HODE_Ret:
pop eax
push 0
push eax
call [@OnEventDrawn]
retn


@HookOnDrawEvent_Party:
push eax ; HookOnDrawEvent_Party proc
pushad
push 1
push eax
call [@OnDrawEvent]
test al, al
popad
jz @HODE2_Ret
cmp byte ptr [eax+99], 0
jnz @HODE2_Ret
cmp byte ptr [eax+49], 0
jnz @HODE2_Ret
call 4c5b98 ; TLcfgCharacter_Draw

@HODE2_Ret:
pop eax
push 1
push eax
call [@OnEventDrawn]
retn


@HookOnDrawBattler_Enemy:
push ebp ; HookOnDrawBattler_Enemy proc
mov ebp, esp
sub esp, 8
mov [local.1], edx
pushad
push edx
push 1
call 4be860 ; GetEnemy
push eax
mov [local.2], eax
call [@OnDrawBattler]
test al, al
popad
jz @HODB_Ret
call 4be860 ; GetEnemy
call 4bd900 ; DrawEnemy

@HODB_Ret:
push [local.1]
push 1
push [local.2]
call [@OnBattlerDrawn]
mov esp, ebp
pop ebp
retn


@HookOnDrawBattler_Hero:
push ebp ; HookOnDrawBattler_Hero proc
mov ebp, esp
sub esp, 8
mov [local.1], edx
pushad
push edx
push 0
call 4b4870 ; GetActor
push eax
mov [local.2], eax
call [@OnDrawBattler]
test al, al
popad
jz @HODB2_Ret
call 4b4870 ; GetActor
call 4bca30 ; DrawActorBattler

@HODB2_Ret:
push [local.1]
push 0
push [local.2]
call [@OnBattlerDrawn]
mov esp, ebp
pop ebp
retn


@HookOnInitTitleScreen:
cmp byte ptr [eax+0d], 0 ; HookOnInitTitleScreen proc
jz 4909a0
cmp byte ptr [eax+0c], 0
jnz 4909a0
pushad
call [@OnInitTitleScreen]
popad
jmp 4909a0


@HookOnDoBattlerAction:
pushad ; HookOnDoBattlerAction proc
mov eax, [edx+10]
movzx eax, byte ptr [eax+7] ; [TLcfgUnitActItem+7] = did last execution attempts fail?
xor eax, 1 ; Convert it to "is this the first try?"
push eax
push edx
call [@OnDoBattlerAction]
popad
push edx
call 49a04c ; ExecuteBattlerAction
pop edx
pushad
movzx eax, al
push eax
push edx
mov edx, [edx+10]
xor al, 1
or [edx+7], al ; If execution failed, set [TLcfgUnitActItem+7]
;movzx ; WHAT WAS HERE?!
call [@OnBattlerActionDone]
popad
retn


@HookOnFrame_Map:
push 0 ; HookOnFrame_Map proc
call 4a37bc ; TLcfFieldScene_Draw
call [@OnFrame]
retn


@HookOnFrame_Menu:
push 1 ; HookOnFrame_Menu proc
call 4a3314 ; TLcfMenuScene_Draw
call [@OnFrame]
retn


@HookOnFrame_Battle:
push 2 ; HookOnFrame_Battle proc
call 498ca4 ; TLcfBattleScene_Draw
call [@OnFrame]
retn


@HookOnFrame_Shop:
push 3 ; HookOnFrame_Shop proc
call 494a1c ; TLcfShopScene_Draw
call [@OnFrame]
retn


@HookOnFrame_Name:
push 4 ; HookOnFrame_Name proc
call 492bb4 ; TLcfNameScene_Draw
call [@OnFrame]
retn


@HookOnFrame_SaveFile:
push 5 ; HookOnFrame_SaveFile proc
call 4916cc ; TLcfSaveFileScene_Draw
call [@OnFrame]
retn


@HookOnFrame_Title:
push 6 ; HookOnFrame_Title proc
call 490b30 ; TLcfTitleScene_Draw
call [@OnFrame]
retn


@HookOnFrame_GameOver:
push 7 ; HookOnFrame_GameOver proc
call 48f6c0 ; TLcfGameOverScene_Draw
call [@OnFrame]
retn


@HookOnFrame_Debug:
push 8 ; HookOnFrame_Debug proc
call 48f424 ; TLcfDebugScene_Draw
call [@OnFrame]
retn


@HookOnCheckEventVisibility:
pushad ; HookOnCheckEventVisibility
push eax
call [@OnCheckEventVisibility]
test al, al
popad
jz @OCEV_Vis
push ebx
push esi
mov ebx, eax
jmp 4c42f2

@OCEV_Vis:
mov al, 1
retn


@HookOnDrawBattleStatusWindow:
push eax
pushad
mov edx, [eax+8]
mov edx, [edx+608]
test edx, edx
setnz dl
movzx edx, dl
push edx
mov edx, [4cdd38]
mov edx, [edx]
cmp eax, [edx+24]
setne dl
movzx edx, dl
push edx
movzx edx, byte ptr [eax+50]
push edx
push [eax+44]
push [eax+14]
call [@OnDrawBattleStatusWindow]
test eax, eax
popad
jz @DontDrawBSW
call 496718 ; TLcfBattleStatusWindow_Draw

@DontDrawBSW:
pop eax
pushad
mov edx, [eax+8]
mov edx, [edx+608]
test edx, edx
setnz dl
movzx edx, dl
push edx
mov edx, [4cdd38]
mov edx, [edx]
cmp eax, [edx+24]
setne dl
movzx edx, dl
push edx
movzx edx, byte ptr [eax+50]
push edx
push [eax+44]
push [eax+14]
call [@OnBattleStatusWindowDrawn]
popad
retn


@HookOnDrawBattleActionWindow:
mov edx, [eax+8] ; HookOnDrawBattleActionWindow proc
mov edx, [edx+608]
test edx, edx
jz @NoBAWHook
push eax
pushad
mov edx, [eax+8]
mov edx, [edx+608]
test edx, edx
setnz dl
movzx edx, dl
push edx
movzx edx, byte ptr [eax+50]
push edx
push [eax+44]
lea edx, [eax+18]
push edx
lea edx, [eax+14]
push edx
call [@OnDrawBattleStatusWindow]
test eax, eax
popad
jz @DontDrawBAW
mov edx, [eax]
call [edx+34] ; TLcfWindow_Draw

@DontDrawBAW:
pop eax

@NoBAWHook:
retn


@HookOnSystemBackgroundDrawn:
push edx ; HookOnSystemBackgroundDrawn proc
call @OriginalDrawSystemBackground
call [@OnSystemBackgroundDrawn]
retn


@OriginalDrawSystemBackground:
push ebx ; OriginalDrawSystemBackground proc
push esi
push edi
push ebp
add esp, -10
jmp 4681c3


@SetCommonEventID1:
call 47991c ; SetCommonEventID1 proc ; CallEvent
mov eax, ebx
neg eax
retn


@SetCommonEventID2:
call 47991c ; SetCommonEventID2 proc ; CallEvent
mov eax, edi
neg eax
mov [ebx+1c], eax
retn


@SetCommonEventID3:
call 47991c ; SetCommonEventID3 proc ; CallEvent
mov edx, 1
mov eax, esi
call 47967c ; GetEvParam
neg eax
mov [ebx+1c], eax
retn


@SetCommonEventID4:
call 47991c ; SetCommonEventID4 proc ; CallEvent
xor edx, edx
mov eax, edi
call 47967c ; GetEvParam
neg eax
mov [ebx+1c], eax
retn


@ChoicePlayBuzzerSound:
cmp dword ptr [esi+0a0], 0 ; ChoicePlayBuzzerSound proc
pushfd
jnz @NoBuzzer
mov eax, [4cdc7c]
mov eax, [eax]
call 48bae4 ; GetBuzzerSound
mov edx, eax
mov eax, [4cdc7c]
mov eax, [eax]
call 48b538 ; CallHarmonyPlaySound

@NoBuzzer:
popfd
retn


@GetVar:
push edx ; GetVar proc
push ecx
mov eax, [4cdc7c]
mov eax, [eax]
mov edx, [esp+0c]
call 48b398 ; GetVar
pop ecx
pop edx
retn 4


@BetterInputNumber:
mov [edx+0b0], eax ; BetterInputNumber proc
push eax
call @GetVar
mov [edx+0a8], eax
retn


@CheckF11:
push 7a ; CheckF11 proc ; VK_F11
call 407054 ; GetAsyncKeyState
movsx eax, ax
test ah, 80
jz 407054 ; GetAsyncKeyState
mov eax, [4cdc7c]
mov eax, [eax]
mov byte ptr [eax+4], 5
mov eax, [4cdfcc]
mov eax, [eax]
mov byte ptr [eax+50], 1
mov byte ptr [eax+51], 0
mov eax, [4cdc1c]
mov eax, [eax]
push 40240000
push 0
call 48ce04 ; FadeOut
mov eax, [4cdc1c]
mov eax, [eax]
mov byte ptr [eax+0c], 0
xor ax, ax
retn 4


@UpdateEventsAfterShop:
call 46bc2c ; UpdateEventsAfterShop proc ; UpdateScreen
mov eax, [4cdd74]
mov eax, [eax]
mov eax, [eax+18]
call 4ab8b4 ; UpdateEvents
retn



@DoSmoothEventSpeed:
mov al, [esi+43] ; DoSmoothEventSpeed proc
test al, al
jz @NormalEvSpeed
xor edx, edx
cmp al, 0ff
je @EvSpeedRet
movzx edx, al
jmp @EvSpeedRet

@NormalEvSpeed:
mov eax, [esi+38]
mov edx, [4cda90+eax*4]

@EvSpeedRet:
mov eax, esi
mov ecx, [eax]
call [ecx+34]
retn


@GetEvScreenXWrapper:
push eax ; GetEvScreenXWrapper proc
push @ScreenXJumpBack
push ebx
push esi
mov esi, [4cdd74]
jmp 4c46f0

@ScreenXJumpBack:
pop edx ; ScreenXJumpBack proc
movsx edx, byte ptr [edx+41]
add eax, edx
retn


@GetEvScreenYWrapper:
push eax ; GetEvScreenYWrapper proc
push @ScreenYJumpBack
push ebx
push esi
mov esi, [4cdd74]
jmp 4c47cc

@ScreenYJumpBack:
pop edx ; ScreenYJumpBack proc
movsx edx, byte ptr [edx+42]
add eax, edx
retn


@CheckRestartOnError:
mov ecx, [4cdb94] ; CheckRestartOnError proc
mov ecx, [ecx]
test ecx, ecx
jz @AbortGame
cmp dword ptr [ecx], 0
jz @AbortGame
mov ecx, [ecx+10]
test ecx, ecx
jz @AbortGame
jmp 46ae95

@AbortGame:
mov eax, [4cde78]
mov eax, [eax]
call 455808 ; Forms::TApplication::Terminate
jmp 46ae95


@PlayCancelOrder:
cmp byte ptr [esi+80], 1 ; PlayCancelOrder proc
jne 404510
and byte ptr [esi+80], 1
push eax
push edx
push ecx
mov eax, [4cdc7c]
mov eax, [eax]
push eax
call 48ba04 ; GetCancelSound
mov edx, eax
pop eax
call 48b538 ; CallHarmonyPlaySound
pop ecx
pop edx
pop eax
jmp 404510


@GaugeSkillWindowFix:
push eax ; GaugeSkillWindowFix proc
mov eax, [4cdd60]
mov eax, [eax]
cmp byte ptr [eax+8], 2 ; in gauge layout only
pop eax
jz @GSKRet
mov edx, [eax]
call [edx+28] 

@GSKRet:
retn


@PlayEnemyAttackSound:
; PlayEnemyAttackSound proc
push eax
mov eax, [eax+10]
cmp byte ptr [eax+4], 4 ; On AK_NONE, don't flash and don't play sound
je @PEAS_NoAction
cmp word ptr [eax+4], 700 ; Same for BA_NONE
je @PEAS_NoAction
push edx
push ecx
mov eax, [4cdc7c]
mov eax, [eax]
push eax
call 48bc84 ; GetAttackingSound
mov edx, eax
pop eax
call 48b538 ; CallHarmonyPlaySound
pop ecx
pop edx
pop eax
jmp 4c1178 ; FlashBattler

@PEAS_NoAction:
pop eax
retn 0c

<00442680>

@DontExitTooQuickly:
@DETQ_Loop:
push 1 ; DontExitTooQuickly proc
call 40d6bc ; Sleep
cmp dword ptr [@ReadyForExit], 0
je @DETQ_Loop
push 3000.
call 40d6bc ; Sleep
call 41f528
retn


@ApplyBattleSpeedSetting:
push eax ; ApplyBattleSpeedSetting proc
mov eax, [@BattleSpeed]
push edx
push ecx
xor edx, edx
imul eax, [esp+10]
mov ecx, 100.
idiv ecx
mov [esp+10], eax
pop ecx
pop edx
pop eax
retn


@ApplyBattleSpeedSetting1:
idiv ecx
push eax ; ApplyBattleSpeedSetting1 proc
call @ApplyBattleSpeedSetting
pop eax
add [ebx+44], eax
retn


@ApplyBattleSpeedSetting2:
call 4a6014 ; ApplyBattleSpeedSetting2 proc ; GetPartyMember
push ebp
call @ApplyBattleSpeedSetting
pop ebp
add [eax+44], ebp
retn


@ErasePictures:
push ebp ; ErasePictures proc
mov ebp, esp
push eax
push ebx
push esi
mov esi, eax
mov ebx, [arg.2]
inc dword ptr [arg.1]

@ErasePicLoop:
mov edx, ebx
mov eax, esi
call 475740 ; GetFromList
test eax, eax
jz @EraseNull
call 4c1bf8 ; ErasePicture

@EraseNull:
inc ebx
cmp ebx, [arg.1]
jnz @ErasePicLoop
pop esi
pop ebx
pop eax
mov esp, ebp
pop ebp
retn 8


@SpecialErasePictures:
push 1. ; @SpecialErasePictures proc
push 1000.
call @ErasePictures
retn


@GetPartyMemberBattleBugfix:
call 4c33d0 ; GetPartyMemberBattleBugfix proc ; DrawWeather
mov eax, [ebp-4]
mov eax, [eax+24]
mov edx, [eax+44]
push eax
mov eax, [4cdb74]
mov eax, [eax]
call 4a6014 ; GetPartyMember
test eax, eax
pop eax
jnz @GPMBB_OK
mov dword ptr [eax+44], 0

@GPMBB_OK:
retn


@TransparentWindowInTitle:
mov edx, [4cdc7c] ; TransparentWindowInTitle proc
cmp al, 2
jne @TrNextCmp
push eax
mov eax, [4cdd60]
mov eax, [eax]
cmp byte ptr [eax+8], 0 ; in traditional battle layout, disable transparency
pop eax
je 4c650a
;cmp dword ptr [@TransparentWindowsEverywhere], 0
;je 4c650a
jmp 4c650e

@TrNextCmp:
cmp al, 5
je 4c650a
cmp al, 3
je 4c650a
cmp dword ptr [@TransparentWindowsEverywhere], 0
je 4c650a
jmp 4c650e


@CheckEventScriptLineDestruction:
cmp eax, [@LastEventScriptLine] ; CheckEventScriptLineDestruction proc
jne 47963c
mov dword ptr [@LastEventScriptLine], 0
jmp 47963c

@BattleActionBugfix:
push eax ; BattleActionBugfix proc
mov eax, [ebp]
mov eax, [eax+4c]
mov eax, [eax+10]
mov byte ptr [eax+4], 4 ; action->kind = RPG::AK_NONE
pop eax
jmp 47f7e0 ; GetBattleCommand2

@SmallWindowBattleMessageBugfix:
mov eax, [4cdd60] ; SmallWindowBattleMessageBugfix proc
mov eax, [eax]
cmp dword ptr [eax+10], 1
jnz 4c643b
mov eax, [4cdef4] ; MessagePtr
mov eax, [eax]
cmp ebx, eax ; Is this the message window?
je 4c643b
jmp 4c6426


@SmallWindowChoiceAmountBugfix:
mov eax, [ebx+20] ; SmallWindowChoiceAmountBugfix proc
sub eax, dword ptr [ebx+34] ; Subtract twice the top cursor offset instead of a fixed 0x10
sub eax, dword ptr [ebx+34]
retn


@DivisionByZeroBugfix:
test edi, edi ; DivisionByZeroBugfix proc
jnz @DBZBRet
xor eax, eax

@DBZBRet:
retn

; !== END
; =================================================================================
@RepairMistake1:
mov eax, [eax+0ac] ; RepairMistake1 proc
call 4047a4
jmp 49338f

@RepairMistake2:
push ebx ; RepairMistake2 proc
mov ebx, eax
mov eax, [4cdb24]
jmp 48cd48

@RepairMistake3:
mov ecx, 30.
mov edx, 20.
retn

@RepairMistake4:
mov ecx, 40.
mov edx, 10.
retn

@RepairMistake5:
push 44
push 0
lea ecx, [edi+0c]
jmp 4950de


<00443900>

@CommonThisEventPatchHandler:
cmp edx, 10005.
je @CTEPIsThis

@CTEPIsThisButOkay:
mov eax, edx
add eax, -10001.
jmp 4ac277

@CTEPIsThis:
cmp dword ptr [ecx+1c], 0
jg @CTEPIsThisButOkay

mov eax, [eax+4]
mov eax, [eax+8]
mov ecx, [eax+8]
mov eax, [eax+4]

@CTEPLoop:
dec ecx
mov edx, [eax+ecx*4]
mov edx, [edx+1c]
cmp edx, 0
jg 4ac2ba

test ecx, ecx
jnz @CTEPLoop

xor edx, edx
jmp 4ac2ba


@CommonThisEventPatchHandler2:
; push scripter
; push scriptdata
call 47967c ; GetEvParam
mov edx, eax
cmp edx, 10005.
jnz @CTEPReturn

@CommonThisEventPatchHandler3:
mov ecx, [esp+4]
mov edx, [ecx+1c]
cmp edx, 0
jg @CTEPReturn

mov eax, [esp+8]
mov eax, [eax+4]
mov eax, [eax+8]
mov ecx, [eax+8]
mov eax, [eax+4]

@CTEPLoop2:
dec ecx
mov edx, [eax+ecx*4]
mov edx, [edx+1c]
cmp edx, 0
jg @CTEPReturn

test ecx, ecx
jnz @CTEPLoop2

xor edx, edx

@CTEPReturn:
mov eax, edx
retn 8


; =================================================================================



<0045D840>

int3 ; just for security
int3
int3
int3
int3
int3
int3
int3
int3
int3
int3
int3

<00443A60>

int3 ; just for security
int3
int3
int3
int3
int3
int3
int3
int3
int3
int3
int3

<004C9C31>

call @HookOnStartup

<0048B3B5> ; For compatibility with CustomSaveLoadPatch

jmp @HookOnSetVariable

;<0048B398>

;jmp @HookOnGetVariable ; TOO SLOW!

<0048B33C>

jmp @HookOnSetSwitch

;<0048B324>

;jmp @HookOnGetSwitch ; TOO SLOW!

<0046BC2C>

jmp @HookOnDrawScreen

;<004C243E>

;call @HookOnDrawPicture

<004B1DEC>

call @HookOnEventCommand

<004B2A8B>

call @HookOnComment

<0048FD85>

call @HookOnInitFinished

<00490A62>

call @HookOnNewGame

<004915F0>

call @HookOnLoadGame

<00491532>

call @HookOnSaveGame

;<0048FDD0>
<0044F3B8>

call @HookOnExit

<004AB570>

jmp @HookOnDrawEvent

<004AA8B4>

jmp @HookOnDrawEvent_Party

<004990AD>

call @HookOnDrawBattler_Enemy
jmp 4990d7

<00498964>

call @HookOnDoBattlerAction

<004990CD>

call @HookOnDrawBattler_Hero
jmp 4990d7

<0049038A>

call @HookOnInitTitleScreen

<004A34A8>

dd @HookOnFrame_Map

<0049DF40>

dd @HookOnFrame_Menu

<00494ED8>

dd @HookOnFrame_Battle

<00492ED0>

dd @HookOnFrame_Shop

<004919A8>

dd @HookOnFrame_Name

<00490CF8>

dd @HookOnFrame_SaveFile

<0049054C>

dd @HookOnFrame_Title

<0048F514>

dd @HookOnFrame_GameOver

<0048E5EC>

dd @HookOnFrame_Debug

<004B2B81>

jmp @HookOnEventCommandExecuted

<004C42EC>

jmp @HookOnCheckEventVisibility

<00494D00>

dd @HookOnDrawBattleStatusWindow

<004994FA>

call @HookOnDrawBattleActionWindow

<004681BC>

jmp @HookOnSystemBackgroundDrawn

<004B3096>

jle 4b30af ; Avoid problems with negative IDs for CEs

<004B1952>

jle 4b19aa ; Avoid problems with negative IDs for CEs

<004B2DA6>

call @SetCommonEventID1 ; set common event ID in +1C field
nop
nop

<004B0ECC>

call @SetCommonEventID2

<004B0D5A>

call @SetCommonEventID3

<004B0F71>

call @SetCommonEventID4

<004C834B>

call @ChoicePlayBuzzerSound ; play buzzer sound when esc is pressed at choice when default = ignore (0)
nop
nop

<004ACA31>

call @BetterInputNumber ; preserve initial value of variable
nop

<004A3717>

call @CheckF11 ; in testplay, use F11 for saving the game

<00493887>

call @UpdateEventsAfterShop ; bugfix

<00493384>

jmp @RepairMistake1 ; I made a mistake there, now I am repairing it (1)

<004C46E8>

jmp @GetEvScreenXWrapper

<004C47C4>

jmp @GetEvScreenYWrapper

<004C5B4A>

call @DoSmoothEventSpeed
jmp 4c5b8c

<004C416C>

call 474d68 ; at [TLcfgCharacter+40], save dword instead of byte (so +41, +42 and +43 will be saved too).

<004C3EA9>

xor eax, eax ; initialize [TLcfgCharacter+40] as dword => erase +41, +42 and +43 too
mov [ebx+40], eax
nop

<0046AE89>

jmp @CheckRestartOnError ; don't close on error; just restart game

<004A00F0>

call @PlayCancelOrder ; play cancel sound when exiting order menu

<0049FDCA>

mov byte ptr [esi+80], 3 ; hint for cancel sound fix

<004B1EFA>

nop ; unlock pictures
nop
nop
nop
nop
nop

<004950D7>

jmp @RepairMistake5

<00498345>

call @GaugeSkillWindowFix ; don't hide status window when skill window is opened in gauge layout

<0049C4B2>

nop ; fix reflect bug
mov eax, [4cddc8]
mov eax, [eax]
call 4b4870 ; GetActor

<0049592D>

call @ApplyBattleSpeedSetting1

<00495C87>

call @ApplyBattleSpeedSetting2
nop
nop
nop

<0049A2D0>

call @PlayEnemyAttackSound ; play sound

<0049A2C0>

push 20 ; more instense flashing of enemies
push 30

<0049B9F7>

mov ecx, 50. ; skill info window longer visible for monster skills only
mov edx, 15.

<0049A79C>

;mov ecx, 90. ; skill info window longer visible --- UNDO CHANGES because of bug
;mov edx, 15.
call @RepairMistake4
nop
nop
nop
nop
nop

<0049A7FC>

;mov ecx, 90.
;mov edx, 15.
call @RepairMistake4
nop
nop
nop
nop
nop

<0049A87C>

;mov ecx, 90.
;mov edx, 15.
call @RepairMistake4
nop
nop
nop
nop
nop

<0049A8CA>

;mov ecx, 90.
;mov edx, 15.
call @RepairMistake4
nop
nop
nop
nop
nop

<0049A91C>

;mov ecx, 90.
;mov edx, 15.
call @RepairMistake4
nop
nop
nop
nop
nop

<0049A970>

;mov ecx, 90.
;mov edx, 15.
call @RepairMistake4
nop
nop
nop
nop
nop

<0049CCC2>

;mov ecx, 90.
;mov edx, 15.
call @RepairMistake4
nop
nop
nop
nop
nop

<004BF29C>

mov dword ptr [eax+4], 4 ; clear also [UnitActItem+6] and +7
nop

<004A3D5C>

nop ; I *think* this is an obsolete loop (about event visibility)
nop
; Explanation:
; The RPG Maker seems to be doing something like this:
; for(int i = -1; i < 16; i++) { // WE CAN REMOVE THIS
;   foreach(event in sameLevelHeroEvents) {
;       if(event.y >= i && event.y < i + 16) { // AND THIS
;         event.draw();
;       }
;   }
; }
; We should be able to simplify it because sameLevelHeroEvents is already sorted by z-order!

<004A3D73>

nop
nop

<004A3D85>

nop
nop

<004B4083>

jmp 4b4089 ; skip event visibility check for ships

<004C237D>

push 0
call @MorePictures
nop
nop

<004C241E>

push 1
call @MorePictures
nop
nop

<004C243E>

push 2
call @MorePictures
nop
nop

<004C245E>

push 3
call @MorePictures
nop
nop

<004C2483>

push 4
call @MorePictures
nop
nop

<004C24AB>

push 5
call @MorePictures
nop
nop

<004C24D3>

push 6
call @MorePictures
nop
nop

<004C24FB>

push 7
call @MorePictures
nop
nop

<004A4562>

call @SpecialErasePictures

<004522CC>

jmp 4522ed ; Ignore "Attempt to focus on invalid window" message

<00499482>

call @GetPartyMemberBattleBugfix

<00497136>

nop ; allow transparent windows with classic battle layout
nop
nop
nop

<004C6500>

jmp @TransparentWindowInTitle ; make window in title screen transparent

<004791CC>

dd @CheckEventScriptLineDestruction ; prevent errors when an event script line is destroyed before HookOnEventCommandExecuted ran

<0048CD40>

jmp @RepairMistake2

<00490FF9>

mov eax, 4956fc ; Change %3d to %4d so that HP can have 4 digits in the save menu, properly aligned

<0049625A>

call @BattleActionBugfix ; Bugfix for "Link to event" commands, see http://www.multimediaxis.de/threads/121175-RPG2000-und-2003-Sammelthread-f%C3%BCr-Probleme?p=2982219#post2982219

<004C6419>

jmp @SmallWindowBattleMessageBugfix ; Bugfix for misaligned cursor in choices in battle using "Small window"

<004C6445>

call @SmallWindowChoiceAmountBugfix ; Bugfix for small windows displaying only 3 lines instead of 4, etc.
nop

<00466B2C>

call @DivisionByZeroBugfix ; Bugfix for division by zero error at picture (or generic: image) display using DrawAuroraSheet_Transform

<004AC270>

jmp @CommonThisEventPatchHandler

<004AEF3A>

push eax 
push esi
mov eax, ebx
call @CommonThisEventPatchHandler2
jmp 4aef56

<004AF0A9>

push eax ; for second call
push eax
push edi
mov eax, esi
call @CommonThisEventPatchHandler2
jmp 4af0c5

<004AF0CE>

push edi
call @CommonThisEventPatchHandler2
jmp 4af0eb

<004AFA97>

push eax
push ebp
mov eax, ebx
call @CommonThisEventPatchHandler2
mov esi, eax
jmp 4afabd

<004B0D77>

push [ebp-10] ; scripter, from outer esi
push [ebp-4]
call @CommonThisEventPatchHandler2
jmp 4b0d97

<004B0C95>

push eax
push edx
call @CommonThisEventPatchHandler3
cmp edx, 0 ; result is in eax AND edx
jle 4b0cc1
jmp 4b0cae