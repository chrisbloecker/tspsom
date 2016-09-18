# Objektdateien
OBJS             = $(SRCS:.c=.o)

# Compiler
CC               = gcc

# Precompiler flags
CPPFLAGS_LINUX   =
CPPFLAGS_LINUX64 =
CPPFLAGS_MACOSX  = -DMACOSX
CPPFLAGS_COMMON  = -I./ -I /usr/include/cairo
CPPFLAGS         = $(CPPFLAGS_COMMON) $(CPPFLAGS_$(OS))

# Compiler flags
CFLAGS_LINUX   =
CFLAGS_LINUX64 =
CFLAGS_MACOSX  =
CFLAGS_COMMON  = -Wall -Wextra -Wno-unused-parameter -Werror -Wno-comment -std=gnu99 -pedantic -pg
CFLAGS         = $(CFLAGS_COMMON) $(CFLAGS_$(OS)) -O3 -pipe

# Linker
LD               = gcc

# Linker flags
LDFLAGS_LINUX    = 
LDFLAGS_LINUX64  = 
LDFLAGS_MACOSOX  =
LDFLAGS_COMMON   = -pg 
LDFLAGS          = $(LDFLAGS_COMMON) $(LDFLAGS_$(OS))

# Linker libraries

LDLIBS_LINUX     = 
LDLIBS_LINUX64   = 
LDLIBS_MACOSX    = 
LDLIBS_COMMON    = -lm -lpthread -lcairo -lrt
LDLIBS           = $(LDLIBS_COMMON) $(LDLIBS_$(OS))

# Debugging-Informationen aktivieren
DEBUG = no
INFO  = yes

# Wenn Debugging-Informationen aktiviert werden sollen, entsprechende
# Praeprozessorflags setzen
ifeq ($(DEBUG),yes)
CPPFLAGS_COMMON+=-g -DDEBUG
endif

ifeq ($(INFO),yes)
CPPFLAGS_COMMON+= -DINFO
endif

# zusaetzliche Abhaengigkeiten einbinden
-include Makefile.depend

# Quelldateien
SRCS   = main.c \
         vector.c \
         sampleMap.c \
         mapReader.c \
         neuralNet.c \
         drawer.c 

# ausfuehrbares Ziel
TARGET = tspsom


.SUFFIXES: .o .c
.PHONY: all clean distclean depend $(TARGET)

# TARGETS
all: depend $(TARGET)

doc:
	doxygen ../common/Doxyfile

# Linken des ausfuehrbaren Programms
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(TARGET)

# Kompilieren der Objektdateien
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $*.o $*.c

# einfaches Aufraeumen
clean:
	rm -f $(TARGET)
	rm -f $(OBJS)

# alles loeschen, was erstellt wurde
distclean: clean
	rm -f *~
	rm -f Makefile.depend
	rm -rf doc

# Abhaengigkeiten automatisch ermitteln
depend:
	@rm -f Makefile.depend
	@echo -n "building Makefile.depend ... "
	@$(foreach SRC, $(SRCS), ( $(CC) $(CPPFLAGS) $(SRC) -MM -g0 ) 1>> Makefile.depend;)
	@echo "done"
