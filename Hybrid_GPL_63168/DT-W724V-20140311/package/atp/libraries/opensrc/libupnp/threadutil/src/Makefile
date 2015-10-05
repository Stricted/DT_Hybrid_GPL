################### depend objects
OBJS = FreeList.o LinkedList.o ThreadPool.o TimerThread.o

######################### compile options
#
# Must define _GNU_SOURCE to use pthread_rwlock_xxxx
# See features.h of uclibc
#
CFLAGS = -D_GNU_SOURCE -Wl,-allow-shlib-undefined

################### Yourself lib(if no, fill nothing)
EXTRA_LIB =

include $(TOPDIR)/package/atp/rule.mak
