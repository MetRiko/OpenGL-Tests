3RD=../../3rd
INC=-I$(3RD)/glfw/include -I$(3RD)/glm/include -I$(3RD)/glew/include -I../../src
LIB=-L$(3RD)/glfw/lib -L$(3RD)/glew/lib
LIBS=-lglew32 -lglfw3 -lopengl32 -lgdi32 -lglu32
# STD=-std=c++17 -fconcepts -Wpedantic -Wall -Wextra
STD=-std=c++17 -fconcepts -fdiagnostics-color=always

all:
	g++ -c main.cpp -o main.o $(STD) $(INC)
# 	g++ -c src/main.cpp obj/main.o -I3rd/glfw/include
	g++ -o run.exe main.o $(STD) $(LIB) $(LIBS)

run: all
	./run.exe
