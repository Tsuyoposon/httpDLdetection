# httpDLdetection  


## はじめに
フリーソフトである[Libpgen](https://github.com/slankdev/libpgen)をダウンロード

- git clone https://github.com/slankdev/libpgen.git
- cd libpgen
- git checkout -b 1.1DL refs/tags/v1.1
  - タグで管理してあるので、バージョン1.1をダウンロード
  - git branch をして 1.1DL が作成されているか確認
- sudo make install

これでLibpgenライブラリがインストールされる
<br/>
<br/>
## 実行ファイル管理
C++で書かれているので実行ファイルを作成してから実行  
- make  
でDLdetectionが作成される

- make clean  
で実行ファイルを消す
<br/>

## ソフト仕様
パケットデータファイルのデータから、指定のデータがダウンロードされているか検知する  
- .pcapng形式のデータ2つのデータを使って検知する
  - 読み取り用パケットデータ　：検知をしたいパケットフローデータ
  - 検知シグネチャデータ　　　：検知したいデータが入っているパケットデータ


### 操作コマンド
- ./DLdetection (読み取り用データ) （検知シグネチャデータ）
  - 検知シグネチャデータを元に検知を行う
  - result.pcapng が作成され、検知したパケットの一覧が抽出される
  - 実行結果がコマンドラインに表示される


- ./DLdetection -m (検知したいファイルデータが入っているフォルダ)
  - -mで、検知シグネチャデータ作成モードで実行する
  - 指定したフォルダに入っているファイルを元に作成
  - signature.pcapng として作成される
