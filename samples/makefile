include ../test/makefile.inc

#
# Fairport sample apps
#

ifeq ($(OS), windows)
NAMEPROP=nprop.exe
PSTATTACHMENT=pstattach.exe
PSTDIR=pst_dir.exe
STOREINFO=pstinfo.exe
PSTVALUES=pstval.exe
else
NAMEPROP=nprop
PSTATTACHMENT=pstattach
PSTDIR=pst_dir
STOREINFO=pstinfo
PSTVALUES=pstval
endif

#all: $(NAMEPROP) $(PSTATTACHMENT) $(PSTDIR) $(STOREINFO) $(PSTVALUES)
all: $(NAMEPROP) $(PSTATTACHMENT) $(PSTDIR) $(STOREINFO)

$(NAMEPROP): nameprop/main.cpp
	$(CC) nameprop/main.cpp $(OPTIONS) $(OUTPUT)$(NAMEPROP)

$(PSTATTACHMENT): pstattachment/main.cpp
	$(CC) pstattachment/main.cpp $(OPTIONS) $(OUTPUT)$(PSTATTACHMENT)

$(PSTDIR): pstdir/main.cpp
	$(CC) pstdir/main.cpp $(OPTIONS) $(OUTPUT)$(PSTDIR)

$(STOREINFO): storeinfo/main.cpp
	$(CC) storeinfo/main.cpp $(OPTIONS) $(OUTPUT)$(STOREINFO)

$(PSTVALUES): pstvalues/main.cpp pstvalues/values.h
	$(CC) pstvalues/main.cpp $(OPTIONS) $(OUTPUT)$(PSTVALUES)

pstvalues/values.h: pstvalues/values.pl
	cd pstvalues; perl values.pl ../../fairport/disk/disk.h ../../fairport/util/primitives.h > values.h

clean: 
	$(DELCMD) $(NAMEPROP) $(PSTATTACHMENT) $(PSTDIR) $(STOREINFO) $(PSTVALUES)
