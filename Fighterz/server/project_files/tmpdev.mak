# Microsoft Developer Studio Generated NMAKE File, Based on tmpdev.dsp
!IF "$(CFG)" == ""
CFG=tmpdev - Win32 Debug
!MESSAGE No configuration specified. Defaulting to tmpdev - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "tmpdev - Win32 Release" && "$(CFG)" != "tmpdev - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tmpdev.mak" CFG="tmpdev - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tmpdev - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "tmpdev - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "tmpdev - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\tmpdev.exe"


CLEAN :
	-@erase "$(INTDIR)\bullet.obj"
	-@erase "$(INTDIR)\collition.obj"
	-@erase "$(INTDIR)\console.obj"
	-@erase "$(INTDIR)\findpath.obj"
	-@erase "$(INTDIR)\findpath_pq.obj"
	-@erase "$(INTDIR)\genproc.obj"
	-@erase "$(INTDIR)\map.obj"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\server.obj"
	-@erase "$(INTDIR)\ship.obj"
	-@erase "$(INTDIR)\socket.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\tmpdev.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\tmpdev.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tmpdev.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=alld.lib wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\tmpdev.pdb" /machine:I386 /out:"$(OUTDIR)\tmpdev.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bullet.obj" \
	"$(INTDIR)\collition.obj" \
	"$(INTDIR)\console.obj" \
	"$(INTDIR)\findpath.obj" \
	"$(INTDIR)\findpath_pq.obj" \
	"$(INTDIR)\genproc.obj" \
	"$(INTDIR)\map.obj" \
	"$(INTDIR)\parse.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\server.obj" \
	"$(INTDIR)\ship.obj" \
	"$(INTDIR)\socket.obj"

"$(OUTDIR)\tmpdev.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\server.exe" "$(OUTDIR)\tmpdev.bsc"


CLEAN :
	-@erase "$(INTDIR)\bullet.obj"
	-@erase "$(INTDIR)\bullet.sbr"
	-@erase "$(INTDIR)\collition.obj"
	-@erase "$(INTDIR)\collition.sbr"
	-@erase "$(INTDIR)\console.obj"
	-@erase "$(INTDIR)\console.sbr"
	-@erase "$(INTDIR)\findpath.obj"
	-@erase "$(INTDIR)\findpath.sbr"
	-@erase "$(INTDIR)\findpath_pq.obj"
	-@erase "$(INTDIR)\findpath_pq.sbr"
	-@erase "$(INTDIR)\genproc.obj"
	-@erase "$(INTDIR)\genproc.sbr"
	-@erase "$(INTDIR)\map.obj"
	-@erase "$(INTDIR)\map.sbr"
	-@erase "$(INTDIR)\parse.obj"
	-@erase "$(INTDIR)\parse.sbr"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\player.sbr"
	-@erase "$(INTDIR)\server.obj"
	-@erase "$(INTDIR)\server.sbr"
	-@erase "$(INTDIR)\ship.obj"
	-@erase "$(INTDIR)\ship.sbr"
	-@erase "$(INTDIR)\socket.obj"
	-@erase "$(INTDIR)\socket.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\server.exe"
	-@erase "$(OUTDIR)\server.ilk"
	-@erase "$(OUTDIR)\server.pdb"
	-@erase "$(OUTDIR)\tmpdev.bsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\tmpdev.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\tmpdev.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bullet.sbr" \
	"$(INTDIR)\collition.sbr" \
	"$(INTDIR)\console.sbr" \
	"$(INTDIR)\findpath.sbr" \
	"$(INTDIR)\findpath_pq.sbr" \
	"$(INTDIR)\genproc.sbr" \
	"$(INTDIR)\map.sbr" \
	"$(INTDIR)\parse.sbr" \
	"$(INTDIR)\player.sbr" \
	"$(INTDIR)\server.sbr" \
	"$(INTDIR)\ship.sbr" \
	"$(INTDIR)\socket.sbr"

"$(OUTDIR)\tmpdev.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=alld.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib ole32.lib dinput.lib ddraw.lib dxguid.lib winmm.lib dsound.lib wsock32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\server.pdb" /debug /machine:I386 /out:"$(OUTDIR)\server.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\bullet.obj" \
	"$(INTDIR)\collition.obj" \
	"$(INTDIR)\console.obj" \
	"$(INTDIR)\findpath.obj" \
	"$(INTDIR)\findpath_pq.obj" \
	"$(INTDIR)\genproc.obj" \
	"$(INTDIR)\map.obj" \
	"$(INTDIR)\parse.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\server.obj" \
	"$(INTDIR)\ship.obj" \
	"$(INTDIR)\socket.obj"

"$(OUTDIR)\server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("tmpdev.dep")
!INCLUDE "tmpdev.dep"
!ELSE 
!MESSAGE Warning: cannot find "tmpdev.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "tmpdev - Win32 Release" || "$(CFG)" == "tmpdev - Win32 Debug"
SOURCE=.\bullet.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\bullet.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\bullet.obj"	"$(INTDIR)\bullet.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\collition.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\collition.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\collition.obj"	"$(INTDIR)\collition.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\console.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\console.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\console.obj"	"$(INTDIR)\console.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\findpath.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\findpath.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\findpath.obj"	"$(INTDIR)\findpath.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\findpath_pq.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\findpath_pq.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\findpath_pq.obj"	"$(INTDIR)\findpath_pq.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\genproc.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\genproc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\genproc.obj"	"$(INTDIR)\genproc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\map.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\map.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\map.obj"	"$(INTDIR)\map.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\parse.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\parse.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\parse.obj"	"$(INTDIR)\parse.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\player.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\player.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\player.obj"	"$(INTDIR)\player.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\server.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\server.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\server.obj"	"$(INTDIR)\server.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\ship.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\ship.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\ship.obj"	"$(INTDIR)\ship.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\socket.cpp

!IF  "$(CFG)" == "tmpdev - Win32 Release"


"$(INTDIR)\socket.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "tmpdev - Win32 Debug"


"$(INTDIR)\socket.obj"	"$(INTDIR)\socket.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

