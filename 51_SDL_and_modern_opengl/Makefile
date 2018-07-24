include ../Makefile.Conf

FILE= $(wildcard *.cpp)
TARGET= $(basename $(FILE))

all: $(TARGET)

%:%.cpp
	$(CC) $(FLAGS) $(FILE) -o $(TARGET) $(LIBS) -lGL -lGLU -lGLEW

clean:
	rm -rf $(TARGET) 
