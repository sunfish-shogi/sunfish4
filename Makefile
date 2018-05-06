CMAKE:=cmake
MKDIR:=mkdir
LN:=ln
FIND:=find

COV:=gcov
PROF:=gprof

PROJ_ROOT:=$(shell pwd)

SUNFISH_EXPT:=sunfish_expt
SUNFISH_TEST:=sunfish_test
SUNFISH_BM:=sunfish_bm
SUNFISH_LN:=sunfish_ln
SUNFISH_CSA:=sunfish_csa
SUNFISH_USI:=sunfish_usi
SUNFISH_TOOLS:=sunfish_tools
SUNFISH_DEV:=sunfish_dev

BUILD_DIR:=$(PROJ_ROOT)/out/build
PROFOUT:=$(PROJ_ROOT)/out/profile.txt
TESTRESULT:=$(PROJ_ROOT)/out/test_result.xml
COVOUT:=$(PROJ_ROOT)/out/coverage.txt

KIFU_PROF1:=$(PROJ_ROOT)/kifu/prof1
KIFU_PROF5:=$(PROJ_ROOT)/kifu/prof5
KIFU_PROBLEM:=$(PROJ_ROOT)/kifu/problem

GEN_COV:=$(PROJ_ROOT)/tools/gen_cov_report.py
GROUP_PROF:=$(PROJ_ROOT)/tools/group_prof.pl

HAS_COV:=$(shell which $(COV))

help:
	@echo 'usage:'
	@echo '  make all'
	@echo '  make expt'
	@echo '  make solve'
	@echo '  make prof'
	@echo '  make prof1'
	@echo '  make test'
	@echo '  make bm'
	@echo '  make ln'
	@echo '  make ln-mat'
	@echo '  make ln-mat-only'
	@echo '  make csa'
	@echo '  make csa-debug'
	@echo '  make usi'
	@echo '  make usi-debug'
	@echo '  make tools'
	@echo '  make dev'
	@echo '  make clean'

.PHONY: all
all: test expt bm ln csa usi tools dev

.PHONY: expt
expt:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/expt
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_EXPT) $(SUNFISH_EXPT)

.PHONY: solve
solve:
	$(MAKE) expt
	./$(SUNFISH_EXPT) --solve $(KIFU_PROBLEM) --time 1 --depth 18

.PHONY: expt-prof
expt-prof:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release -D PROFILE=ON $(PROJ_ROOT)/src/expt
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_EXPT) $(SUNFISH_EXPT)

.PHONY: prof
prof:
	$(MAKE) expt-prof
	./$(SUNFISH_EXPT) --solve $(KIFU_PROF5) -ni --time 5 --depth 18
	$(SHELL) -c '$(PROF) ./$(SUNFISH_EXPT) > $(PROFOUT)'
	@echo
	$(SHELL) -c '$(GROUP_PROF) < $(PROFOUT)'
	@echo
	@echo "Please see the details in $(PROFOUT)."

.PHONY: prof1
prof1:
	$(MAKE) expt-prof
	./$(SUNFISH_EXPT) --solve $(KIFU_PROF1) -ni --time 5 --depth 18
	$(SHELL) -c '$(PROF) ./$(SUNFISH_EXPT) > $(PROFOUT)'
	@echo
	$(SHELL) -c '$(GROUP_PROF) < $(PROFOUT)'
	@echo
	@echo "Please see the details in $(PROFOUT)."

.PHONY: test
test:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug $(PROJ_ROOT)/src/test
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_TEST) $(SUNFISH_TEST)
	$(FIND) $(BUILD_DIR)/$@ -name '*.gcda' | xargs $(RM)
	$(SHELL) -c './$(SUNFISH_TEST) --out $(TESTRESULT)'
ifneq ($(HAS_COV),)
	cd $(BUILD_DIR)/$@ && $(SHELL) -c '$(GEN_COV) -s $(PROJ_ROOT)/src -e test > $(COVOUT)'
endif

.PHONY: bm
bm:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/benchmark
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_BM) $(SUNFISH_BM)

.PHONY: ln
ln:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release -D LEARNING=ON $(PROJ_ROOT)/src/learn
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_LN) $(SUNFISH_LN)

.PHONY: ln-mat
ln-mat:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release -D LEARNING=ON -D MATERIAL_LEARNING=ON $(PROJ_ROOT)/src/learn
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_LN) $(SUNFISH_LN)

.PHONY: ln-mat-only
ln-mat-only:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release -D LEARNING=ON -D MATERIAL_LEARNING_ONLY=ON $(PROJ_ROOT)/src/learn
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_LN) $(SUNFISH_LN)

.PHONY: csa
csa:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/csa
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_CSA) $(SUNFISH_CSA)

.PHONY: csa-debug
csa-debug:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=RelWithDebInfo $(PROJ_ROOT)/src/csa
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_CSA) $(SUNFISH_CSA)

.PHONY: usi
usi:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/usi
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_USI) $(SUNFISH_USI)

.PHONY: usi-debug
usi-debug:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=RelWithDebInfo $(PROJ_ROOT)/src/usi
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_USI) $(SUNFISH_USI)

.PHONY: tools
tools:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/tools
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_TOOLS) $(SUNFISH_TOOLS)

.PHONY: dev
dev:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=RelWithDebInfo $(PROJ_ROOT)/src/dev
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_DEV) $(SUNFISH_DEV)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(SUNFISH_EXPT) \
		$(SUNFISH_TEST) \
		$(SUNFISH_BM) \
		$(SUNFISH_LN) \
		$(SUNFISH_CSA) \
		$(SUNFISH_USI) \
		$(SUNFISH_TOOLS) \
		$(SUNFISH_DEV)
