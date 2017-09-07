CC = g++

MUDUO_INCLUDE = /home/semiconlon/Files/TinyFTP/release-install/include/
MUDUO_LIB = /home/semiconlon/Files/TinyFTP/release-install/lib

FTPSERVER_INCLUDE = /home/semiconlon/Files/TinyFTP

CFLAGS  = -g -O0 -Wall -Wextra  \
	   -Wconversion  -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -I$(MUDUO_INCLUDE) \
		 -I$(FTPSERVER_INCLUDE) \
		 -std=c++11 \

LDFLAGS = -L$(MUDUO_LIB) -lmuduo_net -lmuduo_base -lpthread

TEST_DIR = test


all: TinyFTP unitTest clean

TinyFTP: FTPServer.o TinyFTP.o Arguments.o sysUtil.o cmdDeal.o
	$(CC) -o $@ $^ $(LDFLAGS)

FTPServer.o: FTPServer.cpp FTPServer.h
	$(CC) -c $(CFLAGS) $<

TinyFTP.o: TinyFTP.cpp FTPServer.h
	$(CC) -c $(CFLAGS) $<

cmdDeal.o: cmdDeal.cpp cmdDeal.h
	$(CC) -c $(CFLAGS) $<

sysUtil.o: sysUtil.cpp sysUtil.h
	$(CC) -c $(CFLAGS) $<

Arguments.o: Arguments.cpp Arguments.h
	$(CC) -c $(CFLAGS) $<






unitTest: ArgumentsTest

ArgumentsTest: ArgumentsTest.o Arguments.o
	$(CC) -o $@ $^ $(LDFLAGS)

ArgumentsTest.o: ArgumentsTest.cpp Arguments.h
	$(CC) -c $(CFLAGS) $<

Arguments.o: Arguments.cpp Arguments.h
	$(CC) -c $(CFLAGS) $<


.PHONY: clean

clean:
	rm *.o
