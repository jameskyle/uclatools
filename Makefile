TRGTS=ImgLib  analysis colorize imageConversion
CFLAGS=-O3 -ftree-vectorize -arch ppc -arch i386 -arch ppc64 -arch x86_64

all: $(TRGTS)

$(TRGTS): 
	cd src/$@;CFLAGS='$(CFLAGS)' make all

%.o:
	cd src/$*;CFLAGS='$(CFLAGS)' make $*.o

clean: 
	rm -rf _build/

.PHONY: clean all 
