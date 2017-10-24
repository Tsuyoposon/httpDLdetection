#ソース・コンパイル管理
#sourceのMakefileを実行させる

all:
	$(MAKE) -C source/
	$(MAKE) -C source2/
	$(MAKE) -C source3/

clean:
	$(MAKE) -C source/ clean
	$(MAKE) -C source2/ clean
	$(MAKE) -C source3/ clean
