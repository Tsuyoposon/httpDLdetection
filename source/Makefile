#ソースコンパイル管理
#make で実行ファイル作成
#clean で実行ファイルの削除
CC	      = gcc
LIBS	    = -lpgen
CFLAGS    = -Wall
SOURCE    = main.cc readpcap.cc
OBJS      = $(SOURCE:.cc=.o)
PROGRAM   = DLdetection

.PHONY: all
all:	$(PROGRAM)

$(PROGRAM):	$(OBJS)
		$(CC) $(OBJS) $(LIBS) -o $(PROGRAM)
		mv $(PROGRAM) ../

.c.o:
		$(CC) $(CFLAGS) -c $(LIBS)

.PHONY: clean
clean:
		$(RM) ../$(PROGRAM)
		$(RM) $(OBJS)
