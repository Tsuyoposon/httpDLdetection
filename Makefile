#ソース・コンパイル管理
#sourceのMakefileを実行させる

all:
	$(MAKE) -C source/

clean:
	$(MAKE) -C source/ clean
