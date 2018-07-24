include ../Makefile.Conf

FILE= $(wildcard *.cpp)
TARGET= $(basename $(FILE))

all: $(TARGET)

%:%.cpp
	$(CC) $(FLAGS) $(FILE) -o $(TARGET) $(LIBS) -lGL -lGLU

clean:
	rm -rf $(TARGET) 
