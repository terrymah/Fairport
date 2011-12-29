#
# Fairport unit test
#

GCC_OPTIONS=-Wall -Werror -g -DFAIRPORT_VALIDATION_LEVEL_FULL -I ".."

#
# Detect compiler
#
ifdef VCINSTALLDIR # VC
ifdef USE_GPP
CC = g++
OPTIONS=$(GCC_OPTIONS)
OUTPUT=-o
else # else not USE_GPP
CC = cl
OPTIONS=/DEBUG /DFAIRPORT_VALIDATION_LEVEL_FULL /Od /MDd /EHsc /Zi /nologo /W4 /WX /I ".."
OUTPUT=/Fe
endif # USE_GPP
else # assume g++
CC = g++
OPTIONS=$(GCC_OPTIONS)
OUTPUT=-o
endif

#
# Add some other options
#
ifdef CPP11
OPTIONS+= -std=c++0x
endif

ifdef ANALYZE
OPTIONS+= /analyze
endif

#
# Detect OS
#
ifdef APPDATA # Windows
DELCMD=del
NAME=fairport.exe
else # assume Unix
DELCMD=rm
NAME=fairport
endif

fairport.exe: unittest.cpp
	$(CC) unittest.cpp $(OPTIONS) $(OUTPUT)$(NAME)

clean:
	$(DELCMD) fairport* *.o* *.pdb *.ilk *.jpg *.i *.Manifest *.xml *.gcov
