
FILE=Makefile

SUBDIRS =  $(wildcard */)

.PHONY:  $(SUBDIRS)

all:$(SUBDIRS) 

js:$(SUBDIRS) 

clean:$(SUBDIRS) 

$(SUBDIRS):
	$(MAKE) -f $(FILE) -C $@ $(MAKECMDGOALS)
