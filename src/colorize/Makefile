ifeq (,$(filter _%,$(notdir $(CURDIR))))
include ../global.mk
else
#----- End Boilerplate
IMGLIB=../src/ImgLib
VPATH = $(SRCDIR) $(IMGLIB)

SRC_FILES = ImgLib.c VLib.c analyzeUtil.c ImgLibError.c MathLib.c getopt.c  overlayMac.c
O_FILES = $(SRC_FILES:%.c=%.o)
PROGRAMS = overlayMac

all: $(O_FILES) $(PROGRAMS)

%.o: %.c
	$(COMPILE.c) -I$(IMGLIB) $< $(OUTPUT_OPTION)

$(PROGRAMS): $(O_FILES) 
	+@[ -d bin ] || mkdir -p bin
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o ./bin/$@

#----- Begin Boilerplate
endif
