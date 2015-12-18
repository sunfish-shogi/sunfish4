CMAKE:=cmake
MKDIR:=mkdir
LN:=ln
FIND:=find

COV:=gcov
PROF:=gprof

SUNFISH_EXPT:=sunfish_expt
SUNFISH_TEST:=sunfish_test
SUNFISH_BM:=sunfish_bm
SUNFISH_USI:=sunfish_usi
SUNFISH_DEV:=sunfish_dev
BUILD_DIR:=build
KIFU_PROF1:=kifu/prof1
PROFOUT:=profile.txt
GEN_COV:=tools/gen_cov_report.py

HAS_SSE2:=$(shell $(CPP) -E -dM -xc /dev/null | grep __SSE2__ | sed 's/^.* //')
HAS_COV:=$(shell which $(COV))

.PHONY: expt expt-prof
.PHONY: test test-sse
.PHONY: test-nosse
.PHONY: bm
.PHONY: usi usi-debug
.PHONY: dev
.PHONY: clean

help:
	@echo 'usage:'
	@echo '  make expt'
	@echo '  make prof'
	@echo '  make test'
	@echo '  make bm'
	@echo '  make usi'
	@echo '  make usi-debug'
	@echo '  make dev'
	@echo '  make clean'

expt:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release ../../src/expt
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_EXPT) $(SUNFISH_EXPT)

expt-prof:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Release -D PROFILE=ON ../../src/expt
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_EXPT) $(SUNFISH_EXPT)

prof:
	$(MAKE) expt-prof
	./$(SUNFISH_EXPT) --solve $(KIFU_PROF1)
	$(SHELL) -c '$(PROF) ./$(SUNFISH_EXPT) > $(PROFOUT)'
	@echo "See $(PROFOUT)."

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

usi-debug:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug ../../src/usi
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_USI) $(SUNFISH_USI)

dev:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && $(CMAKE) -D CMAKE_BUILD_TYPE=Debug ../../src/dev
	cd $(BUILD_DIR)/$@ && $(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_DEV) $(SUNFISH_DEV)

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(SUNFISH_EXPT) $(SUNFISH_TEST) $(SUNFISH_BM) $(SUNFISH_USI) $(SUNFISH_DEV)
