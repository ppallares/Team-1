CPP_FILES = $(wildcard *.cpp)  
OUT_NAMES = $(patsubst %.cpp,%.o,$(CPP_FILES))
FOLDERS = $(wildcard ./General/*.o)\
		$(wildcard ./Physics/*.o)\
		$(wildcard ./AI/*.o)\
		$(wildcard ./Level_Generation/*.o)
		
CC = clang++
COMPILER_FLAGS = -g -Wall -I/usr/include/SDL2 -std=c++11
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer


all: 
	+$(MAKE) -C AI
	+$(MAKE) -C General
	+$(MAKE) -C Physics
	+$(MAKE) -C Level_Generation
	rm -f *.o
	+$(MAKE) main.o
	+@echo How to run:
	+@echo Type  ./main.o arg,
	+@echo args currently supported: none, c, a, l, p
main.o: main.cpp 
	$(CC) $^ $(FOLDERS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@


clean:
	rm *.o
	rm $(FOLDERS)

rebuild:
	+$(MAKE) clean
	+$(MAKE)
