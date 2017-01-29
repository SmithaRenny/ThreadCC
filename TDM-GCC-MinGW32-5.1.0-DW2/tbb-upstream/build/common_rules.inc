ifneq (,$(findstring s,$(MAKEFLAGS)))
  override largs+=-q
endif
ifneq (,$(repeat))
  override largs+=-r $(repeat)
endif

INCLUDES += $(INCLUDE_KEY)$(tbb_root)/src $(INCLUDE_KEY)$(tbb_root)/src/rml/include $(INCLUDE_KEY)$(tbb_root)/include

CPLUS_FLAGS += $(WARNING_KEY) $(CXXFLAGS)
ifeq ($(tbb_cpf),1)
CPLUS_FLAGS += $(DEFINE_KEY)__TBB_CPF_BUILD=1
endif
LINK_FLAGS += $(LDFLAGS)
LIB_LINK_FLAGS += $(LDFLAGS)

LIB_LINK_CMD ?= $(CPLUS) $(PIC_KEY)
ifeq ($(origin LIB_OUTPUT_KEY), undefined)
    LIB_OUTPUT_KEY = $(OUTPUT_KEY)
endif
ifeq ($(origin LIB_LINK_LIBS), undefined)
    LIB_LINK_LIBS = $(LIBDL) $(LIBS)
endif

CONLY ?= $(CPLUS)

# The most generic rules
#$(1) - is the target pattern
define make-cxx-obj
$1: %.cpp
	$$(CPLUS) $$(OUTPUTOBJ_KEY)$$@ $$(COMPILE_ONLY) $$(CPLUS_FLAGS) $$(CXX_ONLY_FLAGS) $$(CXX_WARN_SUPPRESS) $$(INCLUDES) $$<
endef

TEST_AFFIXES_OBJS=$(addsuffix .$(OBJ),$(addprefix %_,$(TEST_SUFFIXES)) $(addsuffix _%,$(TEST_PREFIXES)))

# Make will not process the same recipe for each test pattern (since the dependency on the same %.cpp)
# thus the separated recipes should be provided
$(foreach t,%.$(OBJ) $(TEST_AFFIXES_OBJS),$(eval $(call make-cxx-obj,$(t))))

.PRECIOUS: %.$(OBJ) %.$(TEST_EXT) %.res $(TEST_AFFIXES_OBJS)

# Rules for generating a test DLL
%_dll.$(OBJ): %.cpp
	$(CPLUS) $(COMPILE_ONLY) $(OUTPUTOBJ_KEY)$@ $(CPLUS_FLAGS) $(PIC_KEY) $(DEFINE_KEY)_USRDLL $(INCLUDES) $<

#$(1) - is the binary name
#$(2) - is the input obj files and libraries
define make-test-binary
	$(CPLUS) $(OUTPUT_KEY)$(strip $1) $(CPLUS_FLAGS) $(2) $(LIBS) $(LINK_FLAGS)
endef

# LINK_FILES the list of options to link test specific files (libraries and object files)
LINK_FILES+=$(TEST_LIBS)
# Rule for generating executable test
%.$(TEST_EXT): %.$(OBJ) $(TEST_LIBS) $(TEST_PREREQUISITE) $(if $(use_proxy),$(PROXY.LIB))
	$(call make-test-binary,$@,$< $(LINK_FILES) $(PIE_FLAG))

# Rules for generating a test DLL
%_dll.$(DLL): LINK_FLAGS += $(PIC_KEY) $(DYLIB_KEY)
%_dll.$(DLL): TEST_LIBS := $(subst %_dll.$(DLL),,$(TEST_LIBS))
%_dll.$(DLL): %_dll.$(OBJ)
	$(call make-test-binary,$@,$< $(LINK_FILES))
.PRECIOUS: %_dll.$(OBJ) %_dll.$(DLL)

%.$(OBJ): %.c
	$(CONLY) $(COMPILE_ONLY) $(OUTPUTOBJ_KEY)$@ $(C_FLAGS) $(INCLUDES) $<

%.$(OBJ): %.asm
	$(ASM) $(ASM_FLAGS) $<

%.$(OBJ): %.s
	cpp <$< | grep -v '^#' >$*.tmp
	$(ASM) $(ASM_FLAGS) -o $@ $*.tmp

# Rule for generating .E file if needed for visual inspection
# Note that ICL treats an argument after PREPROC_ONLY as a file to open,
# so all uses of PREPROC_ONLY should be immediately followed by a file name
%.E: %.cpp
	$(CPLUS) $(CPLUS_FLAGS) $(CXX_ONLY_FLAGS) $(INCLUDES) $(PREPROC_ONLY) $< >$@

# TODO Rule for generating .asm file if needed for visual inspection
%.asm: %.cpp
	$(CPLUS) /c /FAs /Fa $(CPLUS_FLAGS) $(CXX_ONLY_FLAGS) $(INCLUDES) $<

# TODO Rule for generating .s file if needed for visual inspection
%.s: %.cpp
	$(CPLUS) -S $(CPLUS_FLAGS) $(CXX_ONLY_FLAGS) $(INCLUDES) $<

# Customizations
$(KNOWN_WARNINGS): %.$(OBJ): %.cpp
	$(CPLUS) $(COMPILE_ONLY) $(subst $(WARNING_KEY),,$(CPLUS_FLAGS)) $(CXX_ONLY_FLAGS) $(CXX_WARN_SUPPRESS) $(INCLUDES) $<

tbb_misc.$(OBJ): INCLUDES+=$(INCLUDE_KEY).

tbb_misc.E: tbb_misc.cpp
	$(CPLUS) $(CPLUS_FLAGS) $(CXX_ONLY_FLAGS) $(INCLUDE_KEY). $(INCLUDES) $(PREPROC_ONLY) $< >$@

%.res: %.rc $(TBB.MANIFEST)
	rc /Fo$@ $(INCLUDES) $(filter /D%,$(CPLUS_FLAGS)) $<

