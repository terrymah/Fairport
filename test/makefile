include makefile.inc

#
# Fairport unit test
#

ifeq ($(OS),windows)
NAME=fairport.exe
else # assume Unix
NAME=fairport
endif

all: $(NAME)

$(NAME): unittest.cpp
	$(CC) unittest.cpp $(OPTIONS) $(OUTPUT)$(NAME)

clean:
	$(DELCMD) fairport* *.o* *.pdb *.ilk *.jpg *.i *.Manifest *.xml *.gcov
