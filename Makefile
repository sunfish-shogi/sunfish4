CMAKE:=cmake
CMAKE_CACHE:=CMakeCache.txt
MKDIR:=mkdir
LN:=ln
TEST:=test

SUNFISH:=sunfish
SUNFISH_TEST:=sunfish_test
EVAL_BIN:=eval.bin
BUILD_DIR:=build
PROF:=gprof
PROFOUT:=profile.txt

.PHONY: release debug learn clean

help:
	@echo 'usage:'
	@echo '  make release'
	@echo '  make debug'
	@echo '  make test'
	@echo '  make clean'

release:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && \
	$(CMAKE) -D CMAKE_BUILD_TYPE=Release ../../src/app && \
	$(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH) $(SUNFISH)

debug:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && \
	$(CMAKE) -D CMAKE_BUILD_TYPE=Debug ../../src/app && \
	$(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH) $(SUNFISH)

test:
	$(MKDIR) -p $(BUILD_DIR)/$@ 2> /dev/null
	cd $(BUILD_DIR)/$@ && \
	$(CMAKE) -D CMAKE_BUILD_TYPE=Debug ../../src/test && \
	$(MAKE)
	$(LN) -s -f $(BUILD_DIR)/$@/$(SUNFISH_TEST) $(SUNFISH_TEST)
	$(SHELL) -c './$(SUNFISH_TEST)'

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(SUNFISH) $(SUNFISH_TEST)
