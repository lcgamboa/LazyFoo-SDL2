
FILE=Makefile

SUBDIRS =  $(wildcard */)

.PHONY:  $(SUBDIRS)

all:$(SUBDIRS) 

clean:$(SUBDIRS) 

$(SUBDIRS):
	$(MAKE) -f $(FILE) -C $@ $(MAKECMDGOALS)
