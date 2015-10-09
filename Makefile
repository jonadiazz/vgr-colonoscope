all: pathing

pathing: pathing.cpp
	g++ -o pathing pathing.cpp `pkg-config opencv --cflags --libs`

clean:
	rm -f pathing