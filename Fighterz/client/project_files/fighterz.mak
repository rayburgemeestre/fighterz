# Microsoft Developer Studio Generated NMAKE File, Based on fighterz.dsp
!IF "$(CFG)" == ""
CFG=fighterz - Win32 Profile
!MESSAGE No configuration specified. Defaulting to fighterz - Win32 Profile.
!ENDIF 

!IF "$(CFG)" != "fighterz - Win32 Release" && "$(CFG)" != "fighterz - Win32 Debug" && "$(CFG)" != "fighterz - Win32 Profile"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fighterz.mak" CFG="fighterz - Win32 Profile"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fighterz - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "fighterz - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "fighterz - Win32 Profile" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "fighterz - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\fighterz.exe" "$(OUTDIR)\fighterz.bsc"


CLEAN :
	-@erase "$(INTDIR)\bullet.obj"
	-@erase "$(INTDIR)\bullet.sbr"
	-@erase "$(INTDIR)\collition.obj"
	-@erase "$(INTDIR)\collition.sbr"
	-@erase "$(INTDIR)\config.obj"
	-@erase "$(INTDIR)\config.sbr"
	-@erase "$(INTDIR)\console.obj"
	-@erase "$(INTDIR)\console.sbr"
	-@erase "$(INTDIR)\genproc.obj"
	-@erase "$(INTDIR)\genproc.sbr"
	-@erase "$(INTDIR)\global.obj"
	-@erase "$(INTDIR)\global.sbr"
	-@erase "$(INTDIR)\icon.res"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\map.obj"
	-@erase "$(INTDIR)\map.sbr"
	-@erase "$(INTDIR)\options.obj"
	-@erase "$(INTDIR)\options.sbr"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\parse.sbr"
	-@erase "$(INTDIR)\ship.obj"
	-@erase "$(INTDIR)\ship.sbr"
	-@erase "$(INTDIR)\socket.obj"
	-@erase "$(INTDIR)\socket.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\fighterz.bsc"
	-@erase "$(OUTDIR)\fighterz.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\fighterz.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\icon.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fighterz.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bullet.sbr" \
	"$(INTDIR)\collition.sbr" \
	"$(INTDIR)\config.sbr" \
	"$(INTDIR)\console.sbr" \
	"$(INTDIR)\genproc.sbr" \
	"$(INTDIR)\global.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\map.sbr" \
	"$(INTDIR)\options.sbr" \
	"$(INTDIR)\parse.sbr" \
	"$(INTDIR)\ship.sbr" \
	"$(INTDIR)\socket.sbr"

"$(OUTDIR)\fighterz.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=wsock32.lib alld.lib dinput.lib ddraw.lib dxguid.lib winmm.lib dsound.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\fighterz.pdb" /machine:I386 /out:"$(OUTDIR)\fighterz.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bullet.obj" \
	"$(INTDIR)\collition.obj" \
	"$(INTDIR)\config.obj" \
	"$(INTDIR)\console.obj" \
	"$(INTDIR)\genproc.obj" \
	"$(INTDIR)\global.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\map.obj" \
	"$(INTDIR)\options.obj" \
	"$(INTDIR)\parse.obj" \
	"$(INTDIR)\ship.obj" \
	"$(INTDIR)\socket.obj" \
	"$(INTDIR)\icon.res"

"$(OUTDIR)\fighterz.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "fighterz - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\fighterz.exe" "$(OUTDIR)\fighterz.bsc"


CLEAN :
	-@erase "$(INTDIR)\bullet.obj"
	-@erase "$(INTDIR)\bullet.sbr"
	-@erase "$(INTDIR)\collition.obj"
	-@erase "$(INTDIR)\collition.sbr"
	-@erase "$(INTDIR)\config.obj"
	-@erase "$(INTDIR)\config.sbr"
	-@erase "$(INTDIR)\console.obj"
	-@erase "$(INTDIR)\console.sbr"
	-@erase "$(INTDIR)\genproc.obj"
	-@erase "$(INTDIR)\genproc.sbr"
	-@erase "$(INTDIR)\global.obj"
	-@erase "$(INTDIR)\global.sbr"
	-@erase "$(INTDIR)\icon.res"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\map.obj"
	-@erase "$(INTDIR)\map.sbr"
	-@erase "$(INTDIR)\options.obj"
	-@erase "$(INTDIR)\options.sbr"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\parse.sbr"
	-@erase "$(INTDIR)\ship.obj"
	-@erase "$(INTDIR)\ship.sbr"
	-@erase "$(INTDIR)\socket.obj"
	-@erase "$(INTDIR)\socket.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\fighterz.bsc"
	-@erase "$(OUTDIR)\fighterz.exe"
	-@erase "$(OUTDIR)\fighterz.ilk"
	-@erase "$(OUTDIR)\fighterz.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\fighterz.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\icon.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fighterz.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bullet.sbr" \
	"$(INTDIR)\collition.sbr" \
	"$(INTDIR)\config.sbr" \
	"$(INTDIR)\console.sbr" \
	"$(INTDIR)\genproc.sbr" \
	"$(INTDIR)\global.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\map.sbr" \
	"$(INTDIR)\options.sbr" \
	"$(INTDIR)\parse.sbr" \
	"$(INTDIR)\ship.sbr" \
	"$(INTDIR)\socket.sbr"

"$(OUTDIR)\fighterz.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=alld.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib dinput.lib ddraw.lib dxguid.lib winmm.lib dsound.lib wsock32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\fighterz.pdb" /debug /machine:I386 /out:"$(OUTDIR)\fighterz.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\bullet.obj" \
	"$(INTDIR)\collition.obj" \
	"$(INTDIR)\config.obj" \
	"$(INTDIR)\console.obj" \
	"$(INTDIR)\genproc.obj" \
	"$(INTDIR)\global.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\map.obj" \
	"$(INTDIR)\options.obj" \
	"$(INTDIR)\parse.obj" \
	"$(INTDIR)\ship.obj" \
	"$(INTDIR)\socket.obj" \
	"$(INTDIR)\icon.res"

"$(OUTDIR)\fighterz.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "fighterz - Win32 Profile"

OUTDIR=.\Profile
INTDIR=.\Profile
# Begin Custom Macros
OutDir=.\Profile
# End Custom Macros

ALL : "$(OUTDIR)\fighterz.exe" "$(OUTDIR)\fighterz.bsc"


CLEAN :
	-@erase "$(INTDIR)\bullet.obj"
	-@erase "$(INTDIR)\bullet.sbr"
	-@erase "$(INTDIR)\collition.obj"
	-@erase "$(INTDIR)\collition.sbr"
	-@erase "$(INTDIR)\config.obj"
	-@erase "$(INTDIR)\config.sbr"
	-@erase "$(INTDIR)\console.obj"
	-@erase "$(INTDIR)\console.sbr"
	-@erase "$(INTDIR)\genproc.obj"
	-@erase "$(INTDIR)\genproc.sbr"
	-@erase "$(INTDIR)\global.obj"
	-@erase "$(INTDIR)\global.sbr"
	-@erase "$(INTDIR)\icon.res"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\map.obj"
	-@erase "$(INTDIR)\map.sbr"
	-@erase "$(INTDIR)\options.obj"
	-@erase "$(INTDIR)\options.sbr"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\parse.sbr"
	-@erase "$(INTDIR)\ship.obj"
	-@erase "$(INTDIR)\ship.sbr"
	-@erase "$(INTDIR)\socket.obj"
	-@erase "$(INTDIR)\socket.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\fighterz.bsc"
	-@erase "$(OUTDIR)\fighterz.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\fighterz.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\icon.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\fighterz.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bullet.sbr" \
	"$(INTDIR)\collition.sbr" \
	"$(INTDIR)\config.sbr" \
	"$(INTDIR)\console.sbr" \
	"$(INTDIR)\genproc.sbr" \
	"$(INTDIR)\global.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\map.sbr" \
	"$(INTDIR)\options.sbr" \
	"$(INTDIR)\parse.sbr" \
	"$(INTDIR)\ship.sbr" \
	"$(INTDIR)\socket.sbr"

"$(OUTDIR)\fighterz.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=alld.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib dinput.lib ddraw.lib dxguid.lib winmm.lib dsound.lib wsock32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"$(OUTDIR)\fighterz.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bullet.obj" \
	"$(INTDIR)\collition.obj" \
	"$(INTDIR)\config.obj" \
	"$(INTDIR)\console.obj" \
	"$(INTDIR)\genproc.obj" \
	"$(INTDIR)\global.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\map.obj" \
	"$(INTDIR)\options.obj" \
	"$(INTDIR)\parse.obj" \
	"$(INTDIR)\ship.obj" \
	"$(INTDIR)\socket.obj" \
	"$(INTDIR)\icon.res"

"$(OUTDIR)\fighterz.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("fighterz.dep")
!INCLUDE "fighterz.dep"
!ELSE 
!MESSAGE Warning: cannot find "fighterz.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "fighterz - Win32 Release" || "$(CFG)" == "fighterz - Win32 Debug" || "$(CFG)" == "fighterz - Win32 Profile"
SOURCE=.\bullet.cpp

"$(INTDIR)\bullet.obj"	"$(INTDIR)\bullet.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\collition.cpp

"$(INTDIR)\collition.obj"	"$(INTDIR)\collition.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\config.cpp

"$(INTDIR)\config.obj"	"$(INTDIR)\config.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\console.cpp

"$(INTDIR)\console.obj"	"$(INTDIR)\console.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\genproc.cpp

"$(INTDIR)\genproc.obj"	"$(INTDIR)\genproc.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\global.cpp

"$(INTDIR)\global.obj"	"$(INTDIR)\global.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\icon.rc

"$(INTDIR)\icon.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=main.cpp

"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\map.cpp

"$(INTDIR)\map.obj"	"$(INTDIR)\map.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\options.cpp

"$(INTDIR)\options.obj"	"$(INTDIR)\options.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\parse.cpp

"$(INTDIR)\parse.obj"	"$(INTDIR)\parse.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ship.cpp

"$(INTDIR)\ship.obj"	"$(INTDIR)\ship.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\socket.cpp

"$(INTDIR)\socket.obj"	"$(INTDIR)\socket.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

