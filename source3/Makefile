#ソースコンパイル管理
#make で実行ファイル作成
#clean で実行ファイルの削除
CC	      = c++
#LIBS	    = -lpge
CFLAGS    = -Wall
SOURCE    = make_datfile.cpp
#OBJS      = $(SOURCE:.cc=.o)
PROGRAM   = make_data

.PHONY: all
all:	$(PROGRAM)

$(PROGRAM):
		$(CC) -o $(PROGRAM) $(SOURCE) $(CFLAGS)
		mv $(PROGRAM) ../

.PHONY: clean
clean:
		$(RM) ../$(PROGRAM)
		$(RM) $(OBJS)
