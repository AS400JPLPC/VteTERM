# définition des cibles particulières
.PHONY: all mrprope build clean
# désactivation des règles implicites
.SUFFIXES:
# nom de l'executable :
EXEC = vterm


# Adresse des sources, des objets et des includes :
SRCDIR = $(CURDIR)/src/
OBJDIR = $(CURDIR)/obj/
BINDIR = $(CURDIR)/bin/


INCLUDES =

INCLIB =

#/usr/lib/x86_64-linux-gnu/libdl.so

OBJ = $(OBJDIR)$(EXEC).o
EXECUTABLE = $(BINDIR)$(EXEC)



# choix du compilateur :
CXX = g++

# options compilations : voir vte-dev ---->  -Wextra               -Wno-unused-parameter   use juste for debug only for you 

CPPFLAGS=    -std=c++17 -Wall  -fexceptions -pedantic-errors  -Wno-parentheses -Waddress -Wsign-compare -fpermissive  \
			`pkg-config --cflags gtk+-3.0`  `pkg-config --cflags vte-2.91` -fexpensive-optimizations -O3 -Os 

# -no-pie because it's the master program of the project so the caller 
LDFLAGS=  `pkg-config gtk+-3.0 --libs`  `pkg-config vte-2.91 --libs`  -lX11 -no-pie

	
# -------------------------------------------------------------------
#  compilation
# -------------------------------------------------------------------
# compilation obj :  ex  #@echo "$(OBJCPP)"
#
# debug -v   prod [ on enleve les symboles -s & compression ...]


# regle edition de liens  
all: $(OBJ)
	
	$(CXX)  $(OBJ) -o $(EXECUTABLE)   $(LDFLAGS) -s $(INCLIB)  
 
# regle de compilation des sources objet
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX)  $(CPPFLAGS) $(INCLUDES)  -o $@ -c $< 



# pour effacer tous les objets :
clean:
	rm -rf  $(OBJDIR)*.o

# pour effacer tous les objet et les executables :
mrprope: clean
	rm -rf $(EXECUTABLE)

# efface objet(s) et affiche la taille de l'objet résultant
build: clean
	du -sh $(EXECUTABLE)
