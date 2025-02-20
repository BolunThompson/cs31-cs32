CC = g++
INCLUDES = -I/usr/X11/include/GL
LIBS = -L/usr/X11/lib -lglut -lGL -lGLU
STD = -std=c++17

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

.PHONY: default all clean

PRODUCT = WonkyKong

all: $(PRODUCT)

%.o: %.cpp $(HEADERS)
	$(CC) -c $(STD) $(SAMP) $(CCFLAGS) $(INCLUDES) $< -o $@

$(PRODUCT): $(OBJECTS) 
	$(CC) $(SAMP) $(OBJECTS) $(LIBS) -o $@

clean:
	rm -f *.o
	rm -f $(PRODUCT)

sample: clean
	@ $(MAKE) SAMP='-arch arm64 -arch x86_64 -mmacosx-version-min=11.3'
