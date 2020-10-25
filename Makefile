# nom de l'executable :
PROJET = vteTerm


# Adresse des sources, des objets et des includes :
SRCDIR = $(CURDIR)/src/
OBJDIR = $(CURDIR)/obj/
BINDIR = $(CURDIR)/bin/


INCLUDES =\
-I`pkg-config --cflags gtk+-3.0` \
-I`pkg-config --cflags vte-2.91` \
-I$(SRCDIR)

INCLIB =\
`pkg-config vte-2.91 --libs` \
`pkg-config gtk+-3.0 --libs`


#/usr/lib/x86_64-linux-gnu/libdl.so

OBJ = $(OBJDIR)$(PROJET).o
RESULT = $(BINDIR)$(PROJET)



# choix du compilateur :
CXX = g++

# options compilations : voir vte-dev  -Wextra -Wno-unused-parameter   use juste for debug only for you  -Os -s
# -------------------------------------------------------------------
#  production
# -------------------------------------------------------------------
ifeq ($(PROD), true)
 CPPFLAGS=	-std=c++17	-Wall	-fexceptions	-pedantic-errors	-Wno-parentheses	-Waddress	\
 			-Wsign-compare -fpermissive 	-fstack-clash-protection	-fstack-protector-all \
 			`pkg-config --cflags gtk+-3.0`
#  -no-pie because it's the master program of the project so the caller  `pkg-config gmodule-2.0 --libs`
LDFLAGS =	-lX11 -no-pie  $(INCLIB)

OPTIMIZE = -fexpensive-optimizations -O3 -s 

# -------------------------------------------------------------------
#  debug	-fno-omit-frame-pointer
# -------------------------------------------------------------------

else
CPPFLAGS=	-std=c++17 -fexceptions	-pedantic-errors	-Wno-parentheses	-Waddress \
			-Wsign-compare -fpermissive \
			`pkg-config --cflags gtk+-3.0` 
 
LDFLAGS =	-lX11   -no-pie  `pkg-config gtk+-3.0 --libs` 

OPTIMIZE = 
endif
# -------------------------------------------------------------------
#  compilation
# -------------------------------------------------------------------
# compilation obj :  ex  #@echo "$(OBJCPP)"
#
# debug -v   prod [ on enleve les symboles -s & compression ...]


# regle edition de liens
all: $(OBJ)
	$(CXX)  $(OBJ) -o $(RESULT) $(OPTIMIZE) $(LDFLAGS) $(INCLIB)

ifeq ($(PROD), true)
	du -sh	$(RESULT)
	cp -f	$(RESULT) /home/soleil/NimScreen/
	cp -f	$(RESULT) /home/soleil/T_LIB/
	rm	-rf	$(OBJDIR)$(PROJET).o
	rm	-rf	$(RESULT)
else
	du -sh	$(RESULT)
	cp -f	$(RESULT) /home/soleil/NimScreen/
	cp -f	$(RESULT) /home/soleil/T_LIB/
	rm	-rf	$(RESULT)
endif

# regle de compilation des sources objet
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CXX) $(CPPFLAGS) $(INCLUDES)  -o $@ -c $<



# pour effacer tous les objets :
clean:
	rm -rf  $(OBJDIR)*.o
	rm -rf  $(RESULT)
