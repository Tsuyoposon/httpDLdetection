#ソース・コンパイル管理
#sourceのMakefileを実行させる

all:
	$(MAKE) -C source/

2:
	$(MAKE) -C source1/
	$(MAKE) -C source2/
	$(MAKE) -C source3/
	$(MAKE) -C source4/

clean:
	$(MAKE) -C source/ clean
	$(MAKE) -C source1/ clean
	$(MAKE) -C source2/ clean
	$(MAKE) -C source3/ clean
	$(MAKE) -C source4/ clean
