#ソース・コンパイル管理
#sourceのMakefileを実行させる

all:
	$(MAKE) -C source/
	$(MAKE) -C source2/

clean:
	$(MAKE) -C source/ clean
