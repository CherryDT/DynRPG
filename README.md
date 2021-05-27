# DynRPG - The RPG Maker 2003 Plugin SDK

**by David “Cherry” Trapp** - http://cherrytree.at/dynrpg - dynrpg@cherrytree.at

For an explanation of what this is and how to use it, or to **download a release**, check [its **website**](http://cherrytree.at/dynrpg).

This repository houses the full source code of DynRPG. If you are an end-user, get the release linked above instead.

## Directory structure

* `patch`: Contains the source code of the RPG_RT patch (written in assembly).
* `sdk`: Contains the header files and documentation of the DynRPG SDK (written in C++).
* `library`: Contains the source code of the DynRPG library (written in C++).
* `loader`: Contains the source code of the loader DLL (written in FreeBasic).
* `patcher`: Contains the source code of the patcher tool (written in FreeBasic).
* `dyndemo`: Will contain an RM2k3 project as test environment for the patch and loader, will contain the patched RPG_RT and the loader binary.
* `htmlhelp`: Will contain the HTML Help Compiler.
* `docs`: Will contain HTML docs.

## Build instructions

### Patch

#### Prerequisites

* Create an RPG Maker 2003 v1.08 project in a new folder `dyndemo`. You will test the patch and its functions here. The base version of the RPG_RT.exe can be found [here](https://cherryshare.at/f/maVT0s/RPG_RT.exe).
* Install [OllyDbg v1.10](http://ollydbg.de/odbg110.zip) and the [MUltimate Assembler](https://rammichael.com/multimate-assembler) plugin. Also check out [this answer](https://stackoverflow.com/a/13673957/1871033) in case you run into a problem with “Single-step event at ntdll” events. Add `0EEDFADE` to the allowed exceptions list to avoid breaking unnecessarily (for example when pressing F12).

#### Build

1. Load `dyndemo\RPG_RT.exe` into OllyDbg. You can specify command line arguments `TestPlay ShowTitle Window` for easier testing. If you have already loaded it and worked on it before, make sure to rewind it using Ctrl+F2 before saving changes (because further changes may have been made at runtime which you don’t want to include in the patch).
2. Press Ctrl+M to open MUltimate Assembler.
3. If you do this for the first time, right-click the tab bar and use the “Load from file…” option to load `patch\patch.asm`.
4. Click “Assemble” to apply the patch.
5. Make sure you are in the code segment - if not, press Ctrl+G and enter `401000`.
6. Right-click assembly code in the CPU window and select “Copy to executable” => “All modifications”.
7. Right-click the file window contents and select “Save file”.
8. Save to `RPG_RT2.exe`.

This will create a patched RPG_RT binary `dyndemo\RPG_RT2.exe`.

### Loader

#### Prerequisites

* Install [FreeBasic](https://www.freebasic.net/). Known working version is 1.03.0 but newer versions may work as well.
* Install [FBEdit](https://github.com/CherryDT/FbEditMOD/) to open the `.fbp` project files.
* Test project in `dyndemo` must exist (see patch building prerequisites above).

#### Build

1. Open `loader\dynloader.fbp` in FBEdit.
2. Select target `Windows dll` and hit F5 to compile.

This will create a loader DLL`dyndemo\dynloader.dll`.

### Library

#### Prerequisites

* Install MinGW with [GCC 5.3.0](https://sourceforge.net/projects/mingw/files/MinGW/Base/gcc/Version5/gcc-5.3.0-3/). Using a different version may require tweaks to compiler flags and/or modifications of `__asm` statements to work properly!
* Install [CodeBlocks](https://www.codeblocks.org/downloads/) (10.05 is known to work) and make sure it is configured to use the correct GCC version.

#### Build

1. Open `library\DynRPG.cbp` in CodeBlocks.
2. Select target `Release` and Ctrl+F9 to compile.

This will create a static library `sdk\lib\libDynRPG.a`.

**Note:** To build plugins using the DynRPG development environment, all you need to do is adding `(DynRPG repo path)\sdk\include` to your include search paths and `(DynRPG repo path)\sdk\lib` to your library search paths in the plugin project. If you then set the build output as `(DynRPG repo path)\dyndemo\DynPlugins\your_plugin.dll`, you can immediately test the plugin together with the dev version of DynRPG.

### Patcher

#### Prerequisites

* Install [FreeBasic](https://www.freebasic.net/). Known working version is 1.03.0 but newer versions may work as well.
* Install [FBEdit](https://github.com/CherryDT/FbEditMOD/) to open the `.fbp` project files.
* Unpatched file `dyndemo\RPG_RT.exe`, patched file `dyndemo\RPG_RT2.exe` and loader `dyndemo\dynloader.dll` must exist (see instructions to build the patch and the loader above).

#### Build

1. Open a command prompt and go to directory `patcher`.
2. Run `hpdpa dynrpg.txt` to create `dynrpg.hpd`.
3. Open `patcher\dynrpg_patcher.fbp` in FBEdit.
4. Select target `Windows GUI` and hit F5 to compile.

This will create a patcher binary `patcher\dynrpg_patcher.exe`.

### Documentation

#### Prerequisites

* Install [Doxygen](https://www.doxygen.nl/download.html), but get [version 1.7.6.1](https://sourceforge.net/projects/doxygen/files/rel-1.7.6.1/) - there is no guarantee newer versions are compatible.
* Install [HTML Help Compiler](https://docs.microsoft.com/en-us/previous-versions/windows/desktop/htmlhelp/microsoft-html-help-downloads?redirectedfrom=MSDN) into a new directory `htmlhelp`.

#### Build

* Open a command prompt and go to directory `sdk\include\DynRPG`.
* Run `doxygen doxygen.cfg`.

This will create HTML docs in `docs\html` and a CHM file in `sdk\dynrpg.chm`.

