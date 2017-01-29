
tbb_root?=.

export tbb_os=windows

ifeq ($(tbb_cpf),1)
  export CPF_SUFFIX ?=_preview
endif

ifneq (,$(stdver))
    ifeq (,$(findstring ++, $(stdver)))
        $(warning "Warning: unexpected stdver=$(stdver) is used.")
    endif

    CXX_STD_FLAGS = -std=$(stdver) -D_TBB_CPP0X
endif

CXX_ONLY_FLAGS += $(CXX_STD_FLAGS)
export SHELL = cmd
test_dir:=.
export arch:=ia32
export runtime:=tdm-gcc-mingw32-dw2
native_compiler:=gcc
export compiler:=gcc
debugger:=gdb

CMD=cmd /C
CWD=$(shell cmd /C echo %CD%)
RM=cmd /C del /Q /F
RD=cmd /C rmdir
MD=cmd /c mkdir
SLASH=\\
NUL = nul
AR=lib
AR_OUTPUT_KEY=/out:
AR_FLAGS=/nologo /nodefaultlib
OBJ = obj
DLL = dll
LIBEXT = lib
ASMEXT = asm
def_prefix = win32
_WIN32_WINNT=0x0502

TBB.LST = $(tbb_root)/src/tbb/$(def_prefix)-tbb-export.lst
TBB.DEF = $(TBB.LST:.lst=.def)
TBB.DLL = tbb$(CPF_SUFFIX)$(DEBUG_SUFFIX).$(DLL)
TBB.LIB = tbb$(CPF_SUFFIX)$(DEBUG_SUFFIX).$(LIBEXT)
TBB.RES = tbb_resource.res
LINK_TBB.LIB = $(if $(crosstest),$(TBB.LIB),$(DEFINE_KEY)__TBB_IMPLICITLY_LINKED)
TBB.MANIFEST =
ifneq ($(filter vc8 vc9,$(runtime)),)
    TBB.MANIFEST = tbbmanifest.exe.manifest
endif

MALLOC.DEF = $(MALLOC_ROOT)/$(def_prefix)-tbbmalloc-export.def
MALLOC.DLL = tbbmalloc$(DEBUG_SUFFIX).$(DLL)
MALLOC.LIB = tbbmalloc$(DEBUG_SUFFIX).$(LIBEXT)
MALLOC.RES = tbbmalloc.res
MALLOC.MANIFEST =
ifneq ($(filter vc8 vc9,$(runtime)),)
MALLOC.MANIFEST = tbbmanifest.exe.manifest
endif
LINK_MALLOC.LIB = $(MALLOC.LIB)

MALLOCPROXY.DLL = tbbmalloc_proxy$(DEBUG_SUFFIX).$(DLL)
MALLOCPROXY.LIB = tbbmalloc_proxy$(DEBUG_SUFFIX).$(LIBEXT)
LINK_MALLOCPROXY.LIB = $(MALLOCPROXY.LIB)

PROXY.LIB = tbbproxy$(DEBUG_SUFFIX).$(LIBEXT)

RML.DEF = $(RML_SERVER_ROOT)/$(def_prefix)-rml-export.def
RML.DLL = irml$(DEBUG_SUFFIX).$(DLL)
RML.LIB = irml$(DEBUG_SUFFIX).$(LIBEXT)
RML.RES = irml.res
ifneq ($(filter vc8 vc9,$(runtime)),)
RML.MANIFEST = tbbmanifest.exe.manifest
endif

MAKE_TBBVARS  = cmd /C "$(subst /,\,$(tbb_root))\build\generate_tbbvars.bat"

OPENCL.LIB = OpenCL.$(LIBEXT)

# Support for running debug tests to release library and vice versa
#flip_cfg=$(subst _flipcfg,_release,$(subst _release,_debug,$(subst _debug,_flipcfg,$(1))))
#cross_cfg = $(if $(crosstest),$(call flip_cfg,$(1)),$(1))
# Setting default configuration to release
cfg?=release

SLASH= $(strip \)
OBJ = o
LIBEXT = dll
TBB.RES =
MALLOC.RES =
RML.RES =
TBB.MANIFEST =
MALLOC.MANIFEST =
RML.MANIFEST =
TBB.LST = $(tbb_root)/src/tbb/lin32-tbb-export.lst
MALLOC.DEF = $(MALLOC_ROOT)/$(def_prefix)-gcc-tbbmalloc-export.def
RML.DEF = $(RML_SERVER_ROOT)/lin-rml-export.def
LINK_TBB.LIB = $(TBB.LIB)
PROXY.LIB =
CPLUS = g++
COMPILE_ONLY = -c -MMD
PREPROC_ONLY = -E -x c++
INCLUDE_KEY = -I
DEFINE_KEY = -D
OUTPUT_KEY = -o #
OUTPUTOBJ_KEY = -o #
PIC_KEY =
WARNING_AS_ERROR_KEY = -Werror
WARNING_KEY = -Wall
TEST_WARNING_KEY = -Wextra -Wshadow -Wcast-qual -Woverloaded-virtual -Wnon-virtual-dtor -Wno-uninitialized
WARNING_SUPPRESS = -Wno-parentheses -Wno-uninitialized -Wno-non-virtual-dtor
DYLIB_KEY = -shared
LIBDL =
EXPORT_KEY = -Wl,--version-script,
LIBS = -lpsapi
LINK_FLAGS = -Wl,--enable-auto-import
LIB_LINK_FLAGS = $(DYLIB_KEY)
RTM_KEY = -mrtm
# consider Os
CPLUS_FLAGS = -g -Os
CPLUS_FLAGS += -DUSE_WINTHREAD
CPLUS_FLAGS += -D_WIN32_WINNT=$(_WIN32_WINNT)
CPLUS_FLAGS += -DMINGW_HAS_SECURE_API=1 -D__MSVCRT_VERSION__=0x0700 -msse -mthreads
CONLY = gcc
debugger = gdb
C_FLAGS = $(CPLUS_FLAGS)
CPLUS_FLAGS += -m32 -march=i686 $(RTM_KEY)
LIB_LINK_FLAGS += -m32 -s
export UNIXMODE = 1
ASM=
ASM_FLAGS=
TBB_ASM.OBJ=
ASSEMBLY_SOURCE=$(arch)-gas
M_CPLUS_FLAGS = $(CPLUS_FLAGS) -fno-rtti -fno-exceptions

ifneq ($(BUILDING_PHASE),1)
 # definitions for top-level Makefiles
 origin_build_dir:=$(origin tbb_build_dir)
 tbb_build_dir?=$(tbb_root)$(SLASH)build
 export tbb_build_prefix?=$(tbb_os)_$(arch)_$(compiler)_$(runtime)$(CPF_SUFFIX)
 work_dir=$(tbb_build_dir)$(SLASH)$(tbb_build_prefix)
endif  # BUILDING_PHASE != 1

# YOU MUST GUARANTEE THAT ONLY ONE COPY OF
# THE TBB RUN-TIME IS LINKED INTO AN APPLICATION!  LINKING IN MULTIPLE COPIES
# OF THE TBB RUN-TIME, DIRECTLY OR INDIRECTLY, MAY CAUSE PROGRAM FAILURE!
LIB_LINK_CMD = ar rcs
LIB_LINK_FLAGS =
LIB_LINK_LIBS =
LIB_OUTPUT_KEY =
DYLIB_KEY =
override CXXFLAGS += -D__TBB_DYNAMIC_LOAD_ENABLED=0 -D__TBB_SOURCE_DIRECTLY_INCLUDED=1
ITT_NOTIFY =
DLL = a
LIBEXT = a
LIBPREF = lib
LIBDL =
TBB.DLL = $(LIBPREF)tbb$(DEBUG_SUFFIX).$(LIBEXT)
LINK_TBB.LIB = $(TBB.DLL)
TBB.LST =
TBB.DEF =
TBB_NO_VERSION.DLL =
MALLOC.DLL = $(LIBPREF)tbbmalloc$(DEBUG_SUFFIX).$(LIBEXT)
LINK_MALLOC.LIB = $(MALLOC.DLL)
MALLOC.DEF =
MALLOC_NO_VERSION.DLL =
MALLOCPROXY.DLL =
MALLOCPROXY.DEF =

ifndef BUILDING_PHASE
  work_dir:=$(work_dir)
  # assign new value for tbb_root if path is not absolute (the filter keeps only /* paths)
  ifeq ($(filter /% $(SLASH)%, $(subst :, ,$(tbb_root)) ),)
   full_tbb_root:=$(CURDIR)/$(tbb_root)
   ifeq ($(origin_build_dir),undefined)
   #relative path are needed here as a workaround to support whitespaces in path
    override tbb_root:=../..
   else
    override tbb_root:=$(full_tbb_root)
   endif
  export tbb_root
  endif
 endif # !BUILDING_PHASE

.DELETE_ON_ERROR:    # Make will delete target if error occurred when building it.

MAKEOVERRIDES =

.PHONY: default all tbb tbbmalloc tbbproxy test examples

.NOTPARALLEL: tbb tbbmalloc

default: tbb tbbmalloc

tbb: mkdir
	$(MAKE) -C "$(work_dir)_release"  -r -f $(tbb_root)/build/Makefile.tbb cfg=release

tbbmalloc: mkdir
	$(MAKE) -C "$(work_dir)_release"  -r -f $(tbb_root)/build/Makefile.tbbmalloc cfg=release malloc

.PHONY: clean clean_examples mkdir info

mkdir:
	$(shell $(MD) "$(work_dir)_release" >$(NUL) 2>$(NUL))
	@echo Created $(work_dir)_release and ..._debug directories
