##Solaris 2 defines here.
#TOOLS=/opt/local/ImgTools/Objs
#READ_IMAGE=$(TOOLS)/read_image.o
#READ_OTL=$(TOOLS)/read_otl.o
#CC=gcc
C_OPTIMIZE_FLAGS=-O2 -funroll-loops -fomit-frame-pointer -ffast-math -fshared-data 
#INCLUDES=-I/homes/perfusion/home/tldavis/include -I/usr/openwin/include 
#LD_OPTIONS=-L/usr/openwin/lib -R/usr/openwin/lib

# SUNOS4 defines here.
TOOLS=../Reader/
READ_IMAGE=read_image.o
READ_OTL=read_otl.o
CC=gcc
#INCLUDES=-I$(HOME)/include -I/usr/openwin3/include 
CFLAGS=$(C_OPTIMIZE_FLAGS) $(INCLUDES) -DBSD -DNEED_GETOPT_H

xds.test: xds.c read_im.o read_acr.o read_apd2.o symtab.o $(READ_IMAGE)  $(READ_OTL)
	$(CC) $(CFLAGS)  -g -DDEBUG -o xds.test read_im.o read_acr.o \
	read_apd2.o symtab.o \
	 $(READ_IMAGE) $(READ_OTL) -lX11 -L/usr/X11R6/lib xds.c

xds: xds.c read_im.o read_acr.o read_apd2.o symtab.o $(READ_IMAGE) $(READ_OTL)
	$(CC) $(CFLAGS) -O2 -o xds xds.c read_im.o read_acr.o \
	read_apd2.o symtab.o $(READ_IMAGE) $(READ_OTL) -lX11 -L/usr/X11R6/lib
	strip xds

repack: repack.c read_im.o read_acr.o read_apd2.o symtab.o $(READ_OTL) $(READ_IMAGE)
	$(CC) $(CFLAGS) -cg87 -o repack repack.c read_im.o read_acr.o read_apd2.o symtab.o $(READ_OTL) $(READ_IMAGE) 

ss: ss.c
	$(CC) $(CFLAGS) -O2 -cg87  -o ss ss.c $(READ_OTL) $(READ_IMAGE) 
ss.test: ss.c 
	$(CC) $(CFLAGS) -g -o ss.test ss.c $(READ_OTL) $(READ_IMAGE) 

mosaic: mosaic.c
	$(CC) $(CFLAGS) -g -o mosaic mosaic.c $(READ_IMAGE) 

clean:
	rm -rf *.o *.dSYM
