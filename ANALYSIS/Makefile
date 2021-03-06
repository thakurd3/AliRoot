
include Makefile.arch

PACKAGE = ANALYSISalice

default-target: lib$(PACKAGE).so

ALICEINC      = -I.

# add include paths from other par files
ifneq ($(ESD_INCLUDE),)
   ALICEINC += -I../$(ESD_INCLUDE)
endif

ifneq ($(AOD_INCLUDE),)
   ALICEINC += -I../$(AOD_INCLUDE)
endif

ifneq ($(STEERBase_INCLUDE),)
   ALICEINC += -I../$(STEERBase_INCLUDE)
endif

ifneq ($(ANALYSIS_INCLUDE),)
   ALICEINC += -I../$(ANALYSIS_INCLUDE)
endif

ifneq ($(OADB_INCLUDE),)
   ALICEINC += -I../$(OADB_INCLUDE)
endif

ifneq ($(ANALYSISalice_INCLUDE),)
   ALICEINC += -I../$(ANALYSISalice_INCLUDE)
endif

# only if no par file was loaded before
#ifeq ($(ALICEINC),-I.)
  ifneq ($(ALICE_ROOT),)
    ALICEINC += -I$(ALICE_ROOT)/include -I$(ALICE_ROOT)/TENDER/Tender -I$(ALICE_ROOT)/TOF -I$(ALICE_ROOT)/TRD/Cal -I$(ALICE_ROOT)/VZERO -I$(ALICE_ROOT)/TPC -I$(ALICE_ROOT)/T0 -I$(ALICE_ROOT)/EMCAL
  endif
#endif

ifeq ($(PLATFORM),macosx)
ROOTLIBDIR     := $(shell $(RC) --libdir)
ifeq ($(PACKAGE),ANALYSIS)
   PACKLDFLAGS = $(STEERBase_LIBS) $(ESD_LIBS) $(AOD_LIBS) -L$(ROOTLIBDIR) -lXMLParser
endif
ifeq ($(PACKAGE),ANALYSISalice) 
   PACKLDFLAGS = $(STEERBase_LIBS) $(ESD_LIBS) $(AOD_LIBS) $(ANALYSIS_LIBS) $(OADB_LIBS) -L$(ROOTLIBDIR) -lXMLIO -lEG -lTreePlayer
endif
endif

CHECKXML      := $(shell root-config --has-xml)

ifeq ($(CHECKXML),yes)
  CXXFLAGS += -DWITHXML
  CINTFLAGS += -DWITHXML
endif

CXXFLAGS += $(ALICEINC) -g

SRCS     = $(wildcard *.cxx)
HDRS     = $(SRCS:.cxx=.h)
DHDR     = $(PACKAGE)LinkDef.h

DHDR_ANALYSIS := $(DHDR)
HDRS_ANALYSIS := $(HDRS)
SRCS_ANALYSIS := $(SRCS) G__$(PACKAGE).cxx
OBJS_ANALYSIS := $(SRCS_ANALYSIS:.cxx=.o)

PARFILE       = $(PACKAGE).par


lib$(PACKAGE).so: $(OBJS_ANALYSIS)
	@echo "Linking" $@ ...
	@/bin/rm -f $@
ifeq ($(PLATFORM),macosx)
	$(LD) $(SOFLAGS)$@ $(LDFLAGS) $^ $(OutPutOpt) $@ $(PACKLDFLAGS) $(EXPLLINKLIBS)
else
	$(LD) $(SOFLAGS) $(LDFLAGS) $^ $(OutPutOpt) $@ $(PACKLDFLAGS) $(EXPLLINKLIBS)
endif
	@chmod a+x $@
	@echo "done"

%.o:    %.cxx %.h
	$(CXX) $(CXXFLAGS) $(PACKCXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS_ANALYSIS) *.so G__$(PACKAGE).* $(PARFILE)

G__$(PACKAGE).cxx G__$(PACKAGE).h: $(HDRS) $(DHDR)
	@echo "Generating dictionary ..."
	rootcint -f $@ -c $(CINTFLAGS) $(ALICEINC) $^

### CREATE PAR FILE

$(PARFILE): $(patsubst %,$(PACKAGE)/%,$(filter-out G__%, $(HDRS_ANALYSIS) $(SRCS_ANALYSIS) $(DHDR_ANALYSIS) Makefile Makefile.arch PROOF-INF))
	@echo "Creating archive" $@ ...
	@tar cfzh $@ $(PACKAGE)
	@rm -rf $(PACKAGE)
	@echo "done"

$(PACKAGE)/Makefile: Makefile #.$(PACKAGE)
	@echo Copying $< to $@ with transformations
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@sed 's/include \$$(ROOTSYS)\/test\/Makefile.arch/include Makefile.arch/' < $^ > $@

$(PACKAGE)/Makefile.arch: $(ROOTSYS)/test/Makefile.arch
	@echo Copying $< to $@
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@cp -a $^ $@

$(PACKAGE)/PROOF-INF: PROOF-INF.$(PACKAGE)
	@echo Copying $< to $@
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@cp -a -r $^ $@

$(PACKAGE)/%: %
	@echo Copying $< to $@
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@cp -a $< $@

test-%.par: %.par
	@echo "INFO: The file $< is now tested, in case of an error check in par-tmp."
	@mkdir -p par-tmp
	@cd par-tmp; tar xfz ../$<;	cd $(subst .par,,$<); PROOF-INF/BUILD.sh
	@rm -rf par-tmp
	@echo "INFO: Testing succeeded (already cleaned up)"
