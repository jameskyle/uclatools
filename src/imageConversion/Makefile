ifeq (,$(filter _%,$(notdir $(CURDIR))))
include ../global.mk
else
#----- End Boilerplate
IMGLIB=../src/ImgLib
VPATH = $(SRCDIR) $(IMGLIB)

CFLAGS=-O3 -ftree-vectorize -arch ppc -arch i386 -arch ppc64 -arch x86_64
SRC_FILES = ImgLib.c analyzeUtil.c ImgLibError.c VLib.c MathLib.c getopt.c imconvert.c
O_FILES   = $(SRC_FILES:%.c=%.o)
PROGRAMS = imconvert Ana3to4d

all: $(O_FILES) $(PROGRAMS)

%.o: %.c
	$(COMPILE.c) -I$(IMGLIB) $< $(OUTPUT_OPTION)

$(PROGRAMS): ${O_FILES} 
	+@[ -d bin ] || mkdir -p bin
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -I$(IMGLIB) -o ./bin/$@
#----- Begin Boilerplate
endif
