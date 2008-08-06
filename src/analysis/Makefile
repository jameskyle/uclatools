ifeq (,$(filter _%,$(notdir $(CURDIR))))
include ../global.mk
else
#----- End Boilerplate
IMGLIB=../src/ImgLib
VPATH = $(SRCDIR) $(IMGLIB)

SRC_FILES = ImgLib.c VLib.c analyzeUtil.c ImgLibError.c MathLib.c getopt.c Analyze2Genesis.c 
O_FILES = $(SRC_FILES:%.c=%.o)
PROGRAMS = Analyze2Genesis Averager CC_gr dcm2img equalize EditAnalyzeHeader 
PROGRAMS += EditNSlices GEFunc2Analyze gen2bshort Generic2bshort Genesis2Analyze
PROGRAMS += ImageInfo Mathalyze mult2stack npix Outlier P2P ReOrderImages 
PROGRAMS += SwapBFloat SignaRenum SynthSeries T2Map TimeVolumeSwap variance

all: $(O_FILES) $(PROGRAMS)

%.o: %.c
	$(COMPILE.c) -I$(IMGLIB) $< $(OUTPUT_OPTION)

$(PROGRAMS): $(O_FILES) 
	+@[ -d bin ] || mkdir -p bin
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o ./bin/$@

#----- Begin Boilerplate
endif
