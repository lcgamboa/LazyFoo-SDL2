include ../Makefile.Conf

FILE= $(wildcard *.cpp)
TARGET= $(basename $(FILE))

all: $(TARGET)

%:%.cpp
	$(CC) $(FLAGS) $(FILE) -o $(TARGET) $(LIBS)

js:
	$(CCE) $(FILE)  $(FLAGSE) -o $(TARGET).html

clean:
	rm -rf $(TARGET) *.js *.data *.wasm *.html *.mem
