CC=gcc
CFLAGS= -std=gnu99 -g `pkg-config --cflags gtk+-3.0` -Wl,--export-dynamic -pthread -lm
LIBS = `pkg-config --libs gtk+-3.0 gmodule-export-2.0` -lm
CHECK=-Wall -Wextra

all: main.o ThreadWork.o Thread.o LoteryScheduler.o Timer.o UI_Callbacks.o Thread_Callbacks.o FileLoader.o ProgressbarList.o SharedState.o Viewer.o Viewer.callbacks.o 
	$(CC) $(CHECK) $(CFLAGS)  -o threads_lottery main.o ThreadWork.o Thread.o LoteryScheduler.o Timer.o UI_Callbacks.o Thread_Callbacks.o FileLoader.o ProgressbarList.o SharedState.o Viewer.o Viewer.callbacks.o  $(LIBS)

clean:
	rm *.o threads_lottery
