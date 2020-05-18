#
# Makefile fuer QUEST (Queueing SDL Tool)
#
#
# $Author: hirche $
# $Date: 1998/02/19 16:15:20 $
#
# (C) 1995 Universitaet GH Essen
#
# Dieses Makefile stellt folgende Operationen zur Verfuegung:
#
# -) make [all]     : Compiler erzeugen
# -) make install   : Neues Release des Parsers fuer Benutzer zugaenglich machen
# -) make release   :   "      "     "     "      "     "          "        "
# -) make checkout  : alle Projektdateien auschecken
# -) make checkout-lock: alle Projektdateien zum Aendern auschecken
# -) make depend    : alle Abhaengigkeiten zwischen Projektdateien feststellen
# -) make clean     : Objektdateien (*.o) und temporaere Dateien entfernen
# -) make veryclean : make clean + rcsclean + alle Postscriptdateien entfernen
# -) make git-pull  : pull sources from github
# -) make git-push  : push sources to github
# -) make git-add   : add changed sources to git staging
# -) make git-commit: commit staged sources to git HEAD
# -) make postscript: Postscriptdateien aus alle *.h Dateien erzeugen
# -) make print     : alle *.h Dateien ausdrucken
# -) make backup    : Backup von allen *.cpp *.h Dateien und dem Makefile
# -) make find-error: Compilierungsfehler anzeigen
# -) make tags      : Source-Browser-Datei erzeugen
#
# Der Aufbau dieses Makefiles gliedert sich wie folgt:
#
#  0. Globale Umschalter (z.Z zwischen GNU- und SUN-Tools)
#  1. Makefiledirektiven
#  2. Pfade und Dateinamen
#  3. Programme, Kommandos, Kommandooptionen
#  4. Compilierungs- und Linkeroptionen
#  5. Quelldateien des Projekts
#  6. Objektdateien des Projekts
#  7. Makefileregeln (Operationen, s.o.)
#

###############################
# Figure out host system type #
###############################

## Don't rely on $HOSTTYPE being set correctly!
## Try to figure it out ourselves:

## Prevent make from interpreting special characters:
SCRIPT := \
	case `uname -s` in\
    	Linux)	case `uname -m` in\
					i?86) echo ix86-linux;;\
                                        x86_64) echo x86_64-linux;;\
					arm*) echo arm-linux;;\
					sparc) echo sparc-linux;;\
					*) echo unknown-linux;;\
				esac;;\
		SunOS)	echo `uname -p`-SunOS;;\
		*) echo unknown-unknown;;\
	esac

## Run script and store result in variable:
QUEST_HOSTTYPE := $(shell $(SCRIPT))


##########################
# 0. Globale Umschalter: #
##########################

ifeq ($(QUEST_HOSTTYPE), sparc-SunOS)

#TOOLS = SUN
TOOLS = GNU

else

TOOLS = GNU

endif

#PROFILING = yes
PROFILING = no

##########################
# 1. Makefiledirektiven: #
##########################

.SILENT:
                         # alle Make Operationen ohne Ausgaben

.PHONY:	clean all release
                         # Welche Operationen sollen gespraechig sein?

.SUFFIXES: .c .cpp .h .o .y .l
                         # verwendete Dateiendungen 

############################
# 2. Pfade und Dateinamen: #
############################

INCDIR = ../../include
                         # Verzeichnis der DS-Includedateien fuer die
                         # Benutzer der DS (Generator, ...)

LIBDIR = ../../lib/$(QUEST_HOSTTYPE)
                         # Verzeichnis der DS-Library fuer die
                         # Benutzer der DS (Generator, ...)
BINDIR = ../../bin/$(QUEST_HOSTTYPE)
                         # Verzeichnis in dem die Binaerdateien gespeichert
                         # werden sollen
OBJBASEDIR = obj
                         # Verzeichnis unter dem die Objektdateien aller Hosts
                         # gespeichert werden sollen
OBJDIR = $(OBJBASEDIR)/$(QUEST_HOSTTYPE)
                         # Verzeichnis in dem die Objektdateien gespeichert
                         # werden sollen
PSDIR = ps
                         # Directory in dem die Postscriptversionen der
                         # Dateien gespeichert werden sollen
LOGFILE = $(USER).$(QUEST_HOSTTYPE).make.log
                         # Protokollierungsdatei fuer den make-Vorgang
DEPFILE = .depend.$(QUEST_HOSTTYPE)
                         # Dependency-Datei (automatisch generiert)
OUTPUT = $(OBJDIR)/quest
                         # Name des erzeugten Programms
LOUTPUT = $(OBJDIR)/libQUEST.a
                         # Name der erzeugten Library
BACKUP = quest
                         # Name des Backupfiles (ohne Endungen!)

##############################################
# 3. Programme, Kommandos, Kommandooptionen: #
##############################################

LEX = flex
                         # Kommando zum Starten des lexikalischen Analysators
LEXFLAGS = -i
                         # Flags fuer Lexer-Generator
YACC = byacc
                         # Kommando zum Starten des Parser-Generators
YACCFLAGS = -d # -v -t
                         # Flags fuer Parser-Generator
TAR = tar cf
                         # Befehl zum Erzeugen eines Sourcecode-Archievs
UNTAR = tar xf
                         # Befehl zum Expandieren eines Sourcecode-Archievs
TAR_SUFFIX = tar
                         # Endung welche das tar-Kommando verwendet
COMPRESS = gzip -9
                         # Befehl zum Komprimieren von Dateien
UNCOMPRESS = gzip -d
                         # Befehl zum Dekomprimieren von Dateien
COMPRESS_SUFFIX = gz
                         # Endung welche das Komprimierungsprogramm verwendet
EDITOR = nano
                         # Name des verwendeten Editors

GIT = git                # git

TAGS = ctags -t
                         # Programm zum Erzeugen von Sourcebrowsing-Infos
                         # (wo ist eine bestimmte Funktion definiert?, etc)
MAKE_PS = a2ps
                         # Kommando zum Erzeugen von Postscript aus ASCII
MAKE_PS_FLAGS = -o
                         # Option fuer MAKE_PS
PS_SUFFIX = ps

PRINT = lp -d ps1
                         # Druckkommando

ifeq ($(TOOLS), GNU)     # GNU-Version ?

CC = gcc
                         # Name des C-Compilers
C++ = g++
                         # Name des C++-Compilers

AR = ar
                         # Name des Archivers (zum Erzeugen von Libraries)
ARFLAGS = r
                         # Flags fuer den Archiver
else                     # SUN-Version

CC = cc
                         # Name des C-Compilers
C++ = CC
                         # Name des C++-Compilers
AR = $(C++)
                         # Name des Archivers (zum Erzeugen von Libraries)
ARFLAGS = -xar -o
                         # Flags fuer den Archiver
endif

RCSCLEAN = rcsclean
                         # Befehl zum "Aufraeumen" des Projekts (Loeschen
                         # der nicht mit Lock Option "ausgecheckten"
                         # Dateien
RM = /bin/rm -f
                         # Befehl zum Loeschen von Dateien
MV = /bin/mv
                         # Befehl zum Verschieben von Dateien
CP = /bin/cp
                         # Befehl zum Kopieren von Dateien
CHMOD = /bin/chmod
                         # Befehl zum Aendern der Dateiattribute
TOUCH = touch
                         # Befehl zum Aktualisieren des Dateidatums
MKDIR = mkdir -p
                         # Befehl zum Erzeugen von Directories
ifeq ($(TOOLS), GNU)

MAKE_DEPEND = $(C++) -M

else

MAKE_DEPEND = $(C++) -xM

endif
                         # Befehl zum Erzeugen der Abhaengigkeiten
STRIP = strip
                         # Befehl zum Strippen von Binaries
SED = sed
                         # Name des Stream-Editors sed
COFLAGS += -M -q
                         # Flags for checkout program.

#########################################
# 4. Compilierungs- und Linkeroptionen: #
#########################################

ifeq ($(TOOLS), GNU)     # GNU-Version ?

DEFINES = -D_PRODUCTION_RELEASE
                         # fuer Auslieferungsversion von QUEST
# DEFINES += -DDMALLOC -DDEBUG -DDMALLOC_FUNC_CHECK
                         # Defines fuer dmalloc
INCLUDES = -I$(INCDIR)
                         # Include-Verzeichnisse fuer die Compiler
CDEBUG = -O3 -pipe
                         # Debug-/Optimierungsoptionen
CFLAGS = $(CDEBUG) -Wall -pedantic-errors -Wstrict-prototypes
                         # Compiler-Flags
TFLAGS = -fno-implicit-templates
                         # Template-Flags

ifeq ($(PROFILING), yes) # Profiling einschalten ?

PFLAGS = -pg

endif

LIBS = -L$(LIBDIR) -lQP -lCG -lDS # -ldmalloc
                         # Libraries die zum Projekt gelinkt werden sollen

else                     # Sun-Version !

DEFINES = 
#-D_PRODUCTION_RELEASE
#-DDEBUG -DDMALLOC_FUNC_CHECK
                         # Defines fuer die Compiler
INCLUDES = -I$(INCDIR)
                         # Include-Verzeichnisse fuer die Compiler
CDEBUG = -O3 -sb
                         # Debug-/Optimierungsoptionen
CFLAGS = $(CDEBUG) 
                         # Compiler-Flags
TFLAGS = 
                         # Template-Flags

ifeq ($(PROFILING), yes) # Profiling einschalten ?

PFLAGS = -xpg

endif

LIBS = -L$(LIBDIR) -lQP -lCG -lDS
                         # Libraries die zum Projekt gelinkt werden sollen

endif

#################################
# 5. Quelldateien des Projekts: #
#################################

HEADERS =\
	Quest.h
	
SRCS =\
	Main.cpp\
	Quest.cpp
#        dmalloc.cpp

##################################
# 6. Objektdateien des Projekts: #
##################################

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

######################
# 7. Makefileregeln: #
######################

all: clean-rubbish $(OBJDIR) $(LOUTPUT) $(OUTPUT)

$(OUTPUT): $(OBJS) $(LIBDIR)/libDS.a $(LIBDIR)/libCG.a $(LIBDIR)/libQP.a
	@echo Linking $(OUTPUT) ...
	$(C++) $(CDEBUG) $(PFLAGS) -o $(OUTPUT) \
		$(OBJS) \
		$(LIBS) \
    2>> $(LOGFILE)

$(LOUTPUT): $(OBJDIR)/Quest.o
	echo Constructing $(LOUTPUT) ...
	$(AR) $(ARFLAGS) $(LOUTPUT) $(OBJDIR)/Quest.o \
		2>> $(LOGFILE)

$(OBJS): $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	@echo Compiling $< ...
	$(C++) -c $(CFLAGS) $(TFLAGS) $(PFLAGS) $(DEFINES) $(INCLUDES) $< -o $@ 2>> $(LOGFILE)

$(OBJBASEDIR):
	@if [ ! \( -d $(OBJBASEDIR) \) ]; then \
		echo Creating $(OBJBASEDIR) ...; \
		$(MKDIR) $(OBJBASEDIR); fi

$(OBJDIR): $(OBJBASEDIR)
	@if [ ! \( -d $(OBJDIR) \) ]; then \
		echo Creating $(OBJDIR) ...; \
		$(MKDIR) $(OBJDIR); fi

$(LIBDIR): 
	@if [ ! \( -d $(LIBDIR) \) ]; then \
		echo Creating $(LIBDIR) ...; \
		$(MKDIR) $(LIBDIR); fi

$(BINDIR): 
	@if [ ! \( -d $(BINDIR) \) ]; then \
		echo Creating $(BINDIR) ...; \
		$(MKDIR) $(BINDIR); fi

$(DEPFILE):
	$(TOUCH) $(DEPFILE)

install-lib: $(LOUTPUT) $(LIBDIR)
	@echo Deleting old library from $(LIBDIR) ...
	-$(RM) $(LIBDIR)/$(LOUTPUT)
	@echo Installing new library in $(LIBDIR) ...
	$(CP)  $(LOUTPUT) $(LIBDIR)

install-includes: $(HEADERS)
	@echo Deleting old include files from $(INCDIR) ...
	for X in $(HEADERS); do \
		$(RM) $(INCDIR)/$${X}; done
	@echo Installing new include files in $(INCDIR) ...
	for X in $(HEADERS); do \
		$(CP)  $${X} $(INCDIR);\
		$(CHMOD) 660 $(INCDIR)/$${X}; done

install: install-lib install-includes $(OUTPUT) $(BINDIR)
	@echo Installing $(OUTPUT) in $(BINDIR) ...
	$(CP) $(OUTPUT) $(BINDIR)
	@echo Stripping $(BINDIR)/$(notdir $(OUTPUT)) ...
	$(STRIP) $(BINDIR)/$(notdir $(OUTPUT)) 

release: install

git-pull:
	@echo Pulling sources form github...
	$(GIT) pull

git-add:
	@echo Staging changed local sources...
	$(GIT) add -A

git-commit:
	@echo Committing changed local sources...
	$(GIT) commit

git-push:
	@echo Pushing sources to github...
	$(GIT) push

backup:		$(SRCS) $(HEADERS)
	-$(RM) $(BACKUP).$(TAR_SUFFIX) $(BACKUP).$(TAR_SUFFIX).$(COMPRESS_SUFFIX)
	$(TAR) $(BACKUP).$(TAR_SUFFIX) $(SRCS) $(HEADERS) Makefile
	$(COMPRESS) $(BACKUP).$(TAR_SUFFIX)
	-$(MV) $(BACKUP).$(TAR_SUFFIX).$(COMPRESS_SUFFIX) $(BACKUP).taz

fe find-error:
	$(EDITOR) -p $(LOGFILE)

tags: $(SRCS)
	-$(TAGS) $(SRCS)

clean-rubbish:
	-$(RM) core* *.output *.bak $(LOGFILE)

clean-objects:
	-$(RM) $(OBJDIR)/*.o $(OUTPUT) $(TEST_OUTPUT)

clean-rcs:
	-@$(RCSCLEAN) 2> /dev/null

clean: clean-rubbish clean-objects

veryclean: clean clean-rcs 
	-$(RM) $(PSDIR)/*.$(PS_SUFFIX) *.$(PS_SUFFIX) *.$(TAR_SUFFIX) *.$(COMPRESS_SUFFIX) *.taz *tags 2> /dev/null

checkout:
	-@$(CO) -q $(HEADERS)\
						 $(SRCS)

checkout-lock:
	-@$(CO) -l $(HEADERS)\
						 $(SRCS)

depend: $(HEADERS) $(SRCS) $(TMPSRCS)
	@echo Building dependency file $(DEPFILE) ...
	$(MAKE_DEPEND) $(SRCS) $(TMPSRCS) $(INCLUDES) > $(DEPFILE)
	$(SED) 's/^\(.*\)\.o/\$$(OBJDIR)\/\1\.o/g' $(DEPFILE) > $(DEPFILE).sed
	$(MV) $(DEPFILE).sed $(DEPFILE)

-include $(DEPFILE)

