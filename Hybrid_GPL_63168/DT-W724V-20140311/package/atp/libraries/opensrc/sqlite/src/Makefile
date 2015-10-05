################### program name 
PROGRAM_NAME = sqlite

################### depend objects
OBJS = sqlite3.o

######################### compile options
CFLAGS = -D_FILE_OFFSET_BITS=64 -fPIC

################### depend lib
LIBS = -L$(LIB_ATP_DIR)

################### Yourself lib(if no, fill nothing)
EXTRA_LIB =

test: shell.o
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(DBG_FLAGS) -o sql shell.o $(LIBS) -lsqlite -ldl -lpthread
	install -m 755 sql $(INSTALL_DIR)/bin

include $(TOPDIR)/package/atp/rule.mak
