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
OS=windows
else # assume Unix
DELCMD=rm
OS=unix
endif
