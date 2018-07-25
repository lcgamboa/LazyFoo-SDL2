
FILE=Makefile

SUBDIRS =  $(sort $(wildcard */))

.PHONY:  $(SUBDIRS)

all:$(SUBDIRS) 

js:$(SUBDIRS) 

clean:$(SUBDIRS) 

$(SUBDIRS):
	$(MAKE) -f $(FILE) -C $@ $(MAKECMDGOALS)

test:
	ip addr show
	python -m SimpleHTTPServer
