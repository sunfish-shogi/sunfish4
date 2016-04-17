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
TESTRESULT_SSE:=$(PROJ_ROOT)/out/test_result_sse.xml
TESTRESULT_NOSSE:=$(PROJ_ROOT)/out/test_result_nosse.xml
COVOUT_SSE:=$(PROJ_ROOT)/out/coverage_sse.txt
COVOUT_NOSSE:=$(PROJ_ROOT)/out/coverage_nosse.txt

KIFU_PROF1:=$(PROJ_ROOT)/kifu/prof1
KIFU_PROF5:=$(PROJ_ROOT)/kifu/prof5
KIFU_PROBLEM:=$(PROJ_ROOT)/kifu/problem

GEN_COV:=$(PROJ_ROOT)/tools/gen_cov_report.py
GROUP_PROF:=$(PROJ_ROOT)/tools/group_prof.pl

HAS_SSE2:=$(shell $(CPP) -E -dM -xc /dev/null | grep __SSE2__ | sed 's/^.* //')
HAS_COV:=$(shell which $(COV))

.PHONY: all
.PHONY: expt expt-prof
.PHONY: test test-sse test-nosse
.PHONY: bm
.PHONY: usi usi-debug
.PHONY: tools
.PHONY: dev
.PHONY: clean

help:
	@echo 'usage:'
	@echo '  make all'
	@echo '  make expt'
	@echo '  make solve'
	@echo '  make err'
	@echo '  make prof'
	@echo '  make prof1'
	@echo '  make test'
	@echo '  make bm'
	@echo '  make ln'
	@echo '  make csa'
	@echo '  make usi'
	@echo '  make usi-debug'
	@echo '  make tools'
	@echo '  make dev'
	@echo '  make clean'

all: test expt bm ln csa usi tools dev

expt:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/expt
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_EXPT) $(SUNFISH_EXPT)

solve:
	$(MAKE) expt
	./$(SUNFISH_EXPT) --solve $(KIFU_PROBLEM) --time 5 --depth 18

expt-err:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release -D ENABLE_ERR_RATE=ON $(PROJ_ROOT)/src/expt
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_EXPT) $(SUNFISH_EXPT)

err:
	$(MAKE) expt-err
	./$(SUNFISH_EXPT) --solve $(KIFU_PROBLEM) --time 5 --depth 18

expt-prof:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release -D PROFILE=ON $(PROJ_ROOT)/src/expt
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_EXPT) $(SUNFISH_EXPT)

prof:
	$(MAKE) expt-prof
	./$(SUNFISH_EXPT) --solve $(KIFU_PROF5) --time 5 --depth 18
	$(SHELL) -c '$(PROF) ./$(SUNFISH_EXPT) > $(PROFOUT)'
	@echo
	$(SHELL) -c '$(GROUP_PROF) < $(PROFOUT)'
	@echo
	@echo "Please see the details in $(PROFOUT)."

prof1:
	$(MAKE) expt-prof
	./$(SUNFISH_EXPT) --solve $(KIFU_PROF1) --time 5 --depth 18
	$(SHELL) -c '$(PROF) ./$(SUNFISH_EXPT) > $(PROFOUT)'
	@echo
	$(SHELL) -c '$(GROUP_PROF) < $(PROFOUT)'
	@echo
	@echo "Please see the details in $(PROFOUT)."

test:
ifeq ($(HAS_SSE2),1)
	$(MAKE) test-sse
	$(MAKE) test-nosse
else
	$(MAKE) test-nosse
endif

test-sse:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug -D USE_SSE2=1 $(PROJ_ROOT)/src/test
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_TEST) $(SUNFISH_TEST)
	$(FIND) $(BUILD_DIR)/$@ -name '*.gcda' | xargs $(RM)
	$(SHELL) -c './$(SUNFISH_TEST) --out $(TESTRESULT_SSE)'
ifneq ($(HAS_COV),)
	cd $(BUILD_DIR)/$@ && $(SHELL) -c '$(GEN_COV) -s $(PROJ_ROOT)/src -e test > $(COVOUT_SSE)'
endif

test-nosse:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug -D USE_SSE2=0 $(PROJ_ROOT)/src/test
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_TEST) $(SUNFISH_TEST)
	$(FIND) $(BUILD_DIR)/$@ -name '*.gcda' | xargs $(RM)
	$(SHELL) -c './$(SUNFISH_TEST) --out $(TESTRESULT_NOSSE)'
ifneq ($(HAS_COV),)
	cd $(BUILD_DIR)/$@ && $(SHELL) -c '$(GEN_COV) -s $(PROJ_ROOT)/src -e test > $(COVOUT_NOSSE)'
endif

bm:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/benchmark
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_BM) $(SUNFISH_BM)

ln:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/learn
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_LN) $(SUNFISH_LN)

csa:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/csa
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_CSA) $(SUNFISH_CSA)

usi:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/usi
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_USI) $(SUNFISH_USI)

usi-debug:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=RelWithDebInfo $(PROJ_ROOT)/src/usi
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_USI) $(SUNFISH_USI)

tools:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release $(PROJ_ROOT)/src/tools
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_TOOLS) $(SUNFISH_TOOLS)

dev:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=RelWithDebInfo $(PROJ_ROOT)/src/dev
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_DEV) $(SUNFISH_DEV)

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(SUNFISH_EXPT) $(SUNFISH_TEST) $(SUNFISH_BM) $(SUNFISH_LN) $(SUNFISH_USI) $(SUNFISH_DEV)
