GCC_OPTIONS=-Wall -Werror -pedantic -Wno-long-long -g -DFAIRPORT_VALIDATION_LEVEL_FULL -I ".."

#
# Detect compiler
#
ifdef VCINSTALLDIR # VC, unless told to use g++
    ifdef USE_GPP
        CC = g++
        OPTIONS=$(GCC_OPTIONS)
        OUTPUT=-o
    else # else not USE_GPP
        CC = cl
        OPTIONS=/DEBUG /DFAIRPORT_VALIDATION_LEVEL_FULL /Od /MDd /EHsc /Zi /nologo /W4 /WX /I ".."
        OUTPUT=/Fe
    endif # USE_GPP
else # assume g++, unless told to use clang++
    ifdef USE_CLANG
        CC = clang++
    else
        CC = g++
    endif
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
    UNAME := $(shell uname)
    DELCMD=rm
    OS=unix
    #OSX
    ifeq ($(UNAME),Darwin)
        OPTIONS+= -liconv -gstabs
    endif
endif
