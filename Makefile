TARGET:=crc32


DEBUG?=no

CC:=gcc
LD:=gcc
STRIP:=strip
RM:=rm
CP:=cp
CFLAGS:=-Wall -Wunused -Werror
LDFLAGS:=


# options for debug compilation
ifeq ($(DEBUG), 1)
CFLAGS+=-O0 -g -DDEBUG
else
CFLAGS+=-O3
endif


# options for verbosity compilation
ifneq ($(V), 1)
Q := @
echo-cmd = @echo $(1)
else
Q :=
echo-cmd :=
endif

INSTALLDIR:=/usr/local/bin


SRCS:=main.c
SRCS+=crc32.c

OBJS:=$(SRCS:%.c=%.o)


all: $(TARGET)


crc32: $(OBJS)
	$(call echo-cmd,"LD     $@")
	$(Q)$(LD) $(LDFLAGS) -o $@ $^


main.o: crc32.h


%.o: %.c
	$(call echo-cmd,"CC     $@")
	$(Q)$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean distclean strip install unistall

strip: $(TARGET)
	$(call echo-cmd,"STRIP")
	$(Q)$(STRIP) $(TARGET)

install: $(TARGET) strip
	$(call echo-cmd,"INSTALL")
	$(Q)$(CP) $(TARGET) $(INSTALLDIR)

uninstall:
	$(call echo-cmd,"UNINSTALL")
	$(Q)$(RM) -f $(INSTALLDIR)/$(TARGET)

clean:
	$(call echo-cmd,"CLEAN")
	$(Q)$(RM) -fr *.o *~

distclean: clean
	$(call echo-cmd,"DISTCLEAN")
	$(Q)$(RM) -fr $(TARGET)
