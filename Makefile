

ALLH = LexicalParser
ALL = LexicalParser mainTest
PUBLIC = LexicalParser SemanticParser
TARGET = mainTest
GEN = gen
INCLUDE = include /home/13774/x2-devel/include
SRC = src
TEST = test
LD = ld
CC = g++


CCFLAGS := $(patsubst %,-I %,$(INCLUDE)) -std=c++11 -D CODE64
#==========================================
.ONESHELL:
.SECONDEXPANSION:
.PHONY : all clean run

run:$(patsubst %,$(GEN)/%.out,$(TARGET))
	$<

all: $(patsubst %,$(GEN)/%.out,$(TARGET))
	#check out for all
	echo $^
	
$(patsubst %,$(GEN)/%.out,$(TARGET)):$(patsubst %,$(GEN)/%.o,$(PUBLIC)) $$(patsubst $(GEN)/%.out,$(GEN)/%.o,$$@)
	#$(LD) $^ -o $@
	$(CC) $^ -o $@

$(patsubst %,$(GEN)/%.o,$(PUBLIC)):$$(patsubst $$(GEN)/%.o,$$(SRC)/%.cpp,$$@)
	$(CC) $(CCFLAGS) -c $< -o $@

$(patsubst %,$(GEN)/%.o,$(TARGET)):$$(patsubst $$(GEN)/%.o,$$(TEST)/%.cpp,$$@)
	$(CC) $(CCFLAGS) -c $< -o $@

%.cpp:$(patsubst %,$(INCLUDE)/%.h,$(ALLH))
	@echo check h files for cpp

Makefile:
	@echo nothing to do with Makefile.

clean:
	-rm -rf $(GEN)/*

