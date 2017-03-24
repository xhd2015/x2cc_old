

ALLH = LexicalParser
ALL = LexicalParser testParseNumber
PUBLIC = LexicalParser
TARGET = testParseNumber
GEN = gen
LD = ld
CC = g++

#==========================================
.ONESHELL:
.SECONDEXPANSION:
.PHONY : all clean

all: $(patsubst %,$(GEN)/%.out,$(TARGET))
	#check out for all
	echo $^
	
$(patsubst %,$(GEN)/%.out,$(TARGET)):$(patsubst %,$(GEN)/%.o,$(PUBLIC)) $$(patsubst $(GEN)/%.out,$(GEN)/%.o,$$@)
	echo $^
	$(LD) $^ -o $@

$(GEN)/%.o:%.cpp
	$(CC) $< -o $@

%.cpp:$(patsubst %,%.h,$(ALLH))
	#check h files for cpp

clean:
	-rm -rf $(GEN)/*

