export tbb_os=windows

ifneq (,$(stdver))
    ifeq (,$(findstring ++, $(stdver)))
        $(warning "Warning: unexpected stdver=$(stdver) is used.")
    endif

    CXX_STD_FLAGS = -std=$(stdver) -D_TBB_CPP0X
endif

CXX_ONLY_FLAGS += $(CXX_STD_FLAGS)

export SHELL = cmd

test_dir:=.

export arch=ia32
export runtime=gcc_tdm-gcc-mingw32-dw2

export compiler=gcc
debugger=gdb

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
LINK_TBB.LIB = $(if $(crosstest),$(TBB.LIB),$(DEFINE_KEY)__TBB_IMPLICITLY_LINKED)
TBB.MANIFEST =
MALLOC.DEF = $(MALLOC_ROOT)/$(def_prefix)-tbbmalloc-export.def
MALLOC.DLL = tbbmalloc$(DEBUG_SUFFIX).$(DLL)
MALLOC.LIB = tbbmalloc$(DEBUG_SUFFIX).$(LIBEXT)
MALLOC.RES = tbbmalloc.res
MALLOC.MANIFEST =
LINK_MALLOC.LIB = $(MALLOC.LIB)
MALLOCPROXY.DLL = tbbmalloc_proxy$(DEBUG_SUFFIX).$(DLL)
MALLOCPROXY.LIB = tbbmalloc_proxy$(DEBUG_SUFFIX).$(LIBEXT)
LINK_MALLOCPROXY.LIB = $(MALLOCPROXY.LIB)
PROXY.LIB = tbbproxy$(DEBUG_SUFFIX).$(LIBEXT)
RML.DEF = $(RML_SERVER_ROOT)/$(def_prefix)-rml-export.def
RML.DLL = irml$(DEBUG_SUFFIX).$(DLL)
RML.LIB = irml$(DEBUG_SUFFIX).$(LIBEXT)
RML.RES = irml.res
OPENCL.LIB = OpenCL.$(LIBEXT)

# Support for running debug tests to release library and vice versa
flip_cfg=$(subst _flipcfg,_release,$(subst _release,_debug,$(subst _debug,_flipcfg,$(1))))
cross_cfg = $(if $(crosstest),$(call flip_cfg,$(1)),$(1))
# Setting default configuration to release
cfg?=release

ifdef BUILDING_PHASE
 ifndef target
  target:=$(tbb_os)
 endif

 SLASH= $(strip \)
 OBJ = o
 LIBEXT = dll # MinGW allows linking with DLLs directly
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
 CPLUS_FLAGS = -g -Os
 CPLUS_FLAGS += -DUSE_WINTHREAD
 CPLUS_FLAGS += -D_WIN32_WINNT=$(_WIN32_WINNT)
 CPLUS_FLAGS += -DMINGW_HAS_SECURE_API=1 -D__MSVCRT_VERSION__=0x0700 -msse -mthreads
 CONLY = gcc
 debugger = gdb
 C_FLAGS = $(CPLUS_FLAGS)
 CPLUS_FLAGS += -m32 -march=i686 $(RTM_KEY)
 LIB_LINK_FLAGS += -m32
 export UNIXMODE = 1
 ASM=
 ASM_FLAGS=
 TBB_ASM.OBJ=
 ASSEMBLY_SOURCE=$(arch)-gas
 M_CPLUS_FLAGS = $(CPLUS_FLAGS) -fno-rtti -fno-exceptions
endif

ifneq ($(BUILDING_PHASE),1)
 # definitions for top-level Makefiles
 origin_build_dir:=$(origin tbb_build_dir)
 tbb_build_dir?=$(tbb_root)$(SLASH)build
 export tbb_build_prefix?=$(tbb_os)_$(arch)_$(compiler)_$(runtime)$(CPF_SUFFIX)
 work_dir=$(tbb_build_dir)$(SLASH)$(tbb_build_prefix)
endif

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
