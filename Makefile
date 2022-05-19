SRC = $(wildcard src/*.cpp)
HDR = $(wildcard include/*.hpp) $(wildcard include/*.h)
OBJ = $(SRC:.cpp=.o)

CC = g++

CFLAGS = -c
LDFLAGS =

OUT = ./t

all: $(OUT)
	$(OUT) -c test.to

%.o: %.cpp $(HDR)
	$(CC) $(CFLAGS) $< -o $@

$(OUT): $(OBJ) $(HDR)
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ) $(OUT)