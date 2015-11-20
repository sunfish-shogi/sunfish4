CMAKE:=cmake
CMAKE_CACHE:=CMakeCache.txt
MKDIR:=mkdir
LN:=ln
TEST:=test

SUNFISH:=sunfish
SUNFISH_TEST:=sunfish_test
SUNFISH_DEV:=sunfish_dev
EVAL_BIN:=eval.bin
BUILD_DIR:=build
PROF:=gprof
PROFOUT:=profile.txt
GEN_COV:=tools/gen_cov_report.py
COV:=gcov

HAS_SSE2:=$(shell $(CPP) -E -dM -xc /dev/null | grep __SSE2__ | sed 's/^.* //')
HAS_COV:=$(shell which $(COV))

.PHONY: release debug learn clean

help:
	@echo 'usage:'
	@echo '  make release'
	@echo '  make debug'
	@echo '  make test'
	@echo '  make clean'

release:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release ../../src/app
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH) $(SUNFISH)

debug:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug ../../src/app
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH) $(SUNFISH)

test:
ifeq ($(HAS_SSE2),1)
	$(MAKE) test-sse
	$(MAKE) test-nosse
else
	$(MAKE) test-nosse
endif

test-sse:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug -D USE_SSE2=1 ../../src/test
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_TEST) $(SUNFISH_TEST)
	$(SHELL) -c './$(SUNFISH_TEST) --out test_result_sse.xml'
ifneq ($(HAS_COV),)
	cd $(BUILD_DIR)/$@ && $(SHELL) -c '../../$(GEN_COV) -s ../../src -e test > ../../coverage_sse.txt'
endif

test-nosse:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug -D USE_SSE2=0 ../../src/test
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_TEST) $(SUNFISH_TEST)
	$(SHELL) -c './$(SUNFISH_TEST) --out test_result_nosse.xml'
ifneq ($(HAS_COV),)
	cd $(BUILD_DIR)/$@ && $(SHELL) -c '../../$(GEN_COV) -s ../../src -e test > ../../coverage_nosse.txt'
endif

dev:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug ../../src/dev
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_DEV) $(SUNFISH_DEV)

gen-zobrist:
	$(MAKE) dev
	$(SHELL) -c './$(SUNFISH_DEV) --gen-zobrist src/core/position/Zobrist.cpp'

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(SUNFISH) $(SUNFISH_TEST) $(SUNFISH_DEV)
