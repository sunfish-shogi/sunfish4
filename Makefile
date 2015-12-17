CMAKE:=cmake
MKDIR:=mkdir
LN:=ln
FIND:=find

SUNFISH:=sunfish
SUNFISH_TEST:=sunfish_test
SUNFISH_BM:=sunfish_bm
SUNFISH_USI:=sunfish_usi
SUNFISH_DEV:=sunfish_dev
BUILD_DIR:=build
GEN_COV:=tools/gen_cov_report.py
COV:=gcov

HAS_SSE2:=$(shell $(CPP) -E -dM -xc /dev/null | grep __SSE2__ | sed 's/^.* //')
HAS_COV:=$(shell which $(COV))

.PHONY: release debug test test-sse test-nosse bm usi dev clean

help:
	@echo 'usage:'
	@echo '  make release'
	@echo '  make debug'
	@echo '  make test'
	@echo '  make bm'
	@echo '  make usi'
	@echo '  make dev'
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
	$(FIND) $(BUILD_DIR)/$@ -name '*.gcda' | xargs $(RM)
	$(SHELL) -c './$(SUNFISH_TEST) --out test_result_sse.xml'
ifneq ($(HAS_COV),)
	cd $(BUILD_DIR)/$@ && $(SHELL) -c '../../$(GEN_COV) -s ../../src -e test > ../../coverage_sse.txt'
endif

test-nosse:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug -D USE_SSE2=0 ../../src/test
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_TEST) $(SUNFISH_TEST)
	$(FIND) $(BUILD_DIR)/$@ -name '*.gcda' | xargs $(RM)
	$(SHELL) -c './$(SUNFISH_TEST) --out test_result_nosse.xml'
ifneq ($(HAS_COV),)
	cd $(BUILD_DIR)/$@ && $(SHELL) -c '../../$(GEN_COV) -s ../../src -e test > ../../coverage_nosse.txt'
endif

bm:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release ../../src/benchmark
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_BM) $(SUNFISH_BM)

usi:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release ../../src/usi
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_USI) $(SUNFISH_USI)

dev:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug ../../src/dev
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_DEV) $(SUNFISH_DEV)

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(SUNFISH) $(SUNFISH_TEST) $(SUNFISH_BM) $(SUNFISH_USI) $(SUNFISH_DEV)
