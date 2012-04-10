SDK:=$(shell ls /Developer/SDKs | sort -r | head -n 1)
CFLAGS=-Wall -Os -g -isysroot /Developer/SDKs/$(SDK) -arch i386 -arch ppc -I../unimotion
LFLAGS = -Wl,-syslibroot,/Developer/SDKs/$(SDK) -arch i386 -arch ppc -framework Carbon -framework OpenGL -framework GLUT
TARGET= motion

CFLAGS2=-Wall -Os -g -isysroot /Developer/SDKs/$(SDK) -arch i386 -arch ppc -fconstant-cfstrings
LFLAGS2=-Wl,-syslibroot,/Developer/SDKs/$(SDK) -arch i386 -arch ppc -framework IOKit -framework CoreFoundation
TARGET2=libUniMotion.dylib

all: $(TARGET2) $(TARGET) 

clean:
	rm -f *.o $(TARGET) $(TARGET2)

$(TARGET): main.o
	$(CC) $(LFLAGS) -o $@ $^ -L.  -lUniMotion

# Program to display the use of the SMS.
main.o: main.c 

# API for the SMS: SUDDEN MOTION SENSOR
$(TARGET2): unimotion.o
	$(CC) $(LFLAGS2) -headerpad_max_install_names -dynamiclib -install_name @executable_path/$(TARGET2) -o $@ $^

unimotion.o: unimotion.c unimotion.h