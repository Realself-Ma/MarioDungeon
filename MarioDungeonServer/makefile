CC=g++
CCFLAGS=-Wall -std=c++11
INCDIR=-I /home/mtf/muduo
LIBDIR=-L /home/mtf/build/release-cpp11/lib
LIB=-pthread -lmysqlclient -lmuduo_net -lmuduo_base
main:*.cpp
	$(CC) $(CCFLAGS) *.cpp $(INCDIR) $(LIBDIR) -o main $(LIB)
.PHONY:clean
clean:
	rm -rf *.o main
