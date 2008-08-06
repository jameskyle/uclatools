ifeq (,$(filter _%,$(notdir $(CURDIR))))
include ../global.mk
else
#----- End Boilerplate
VPATH = $(SRCDIR) 
SRC_FILES = ImgLib.c VLib.c analyzeUtil.c ImgLibError.c MathLib.c getopt.c
O_FILES   = $(SRC_FILES:%.c=%.o)
PROGRAMS = ShowAnaHdr
all: $(O_FILES) $(PROGRAMS)

%.o: %.c
	$(COMPILE.c) $< $(OUTPUT_OPTION)

%: $(O_FILES) %.c
	+@[ -d bin ] || mkdir -p bin
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o ./bin/$@

.PHONY: all
#----- Begin Boilerplate
endif
