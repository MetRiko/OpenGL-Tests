INC=-I3rd/glfw/include -I3rd/glm/include -I3rd/glew/include -Isrc
LIB=-L3rd/glfw/lib -L3rd/glew/lib
LIBS=-lglew32 -lglfw3 -lopengl32 -lgdi32 -lglu32
# STD=-std=c++17 -fconcepts -Wpedantic -Wall -Wextra
STD=-std=c++17 -fconcepts 

all:
	g++ -c src/main.cpp -o obj/main.o $(STD) $(INC)
# 	g++ -c src/main.cpp obj/main.o -I3rd/glfw/include
	g++ -o run.exe obj/main.o $(STD) $(LIB) $(LIBS)

run: all
	./run.exe
