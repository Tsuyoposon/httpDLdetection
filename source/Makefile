#ソースコンパイル管理
#make で実行ファイル作成
#clean で実行ファイルの削除
CC	      = c++
LIBS	    = -lpgen
CFLAGS    = -Wall -std=c++11
SOURCE    = main.cpp
OBJS      = $(SOURCE:.cc=.o)
PROGRAM   = DLdetection

.PHONY: all
all:	$(PROGRAM)

$(PROGRAM):
		$(CC) -o $(PROGRAM) $(SOURCE) $(CFLAGS) $(LIBS)
		mv $(PROGRAM) ../


.PHONY: clean
clean:
		$(RM) ../$(PROGRAM)
