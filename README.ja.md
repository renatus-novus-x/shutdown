# shutdown

Sharp X68000 / Human68k用の、電源オフと再起動を行う最小ユーティリティです。

## 使用方法

```text
shutdown -h now    システムの電源を切る
shutdown -r now    システムを再起動する
shutdown -?        usageを表示する
shutdown --help    usageを表示する
```

引数なしで`shutdown`を実行した場合はusageを表示するだけで、電源オフや再起動は
行いません。生成したディスクイメージも、Human68k起動後に`AUTOEXEC.BAT`から
引数なしで実行し、usageを表示します。

終了処理にはX68000の`TRAP #10`を使用します。ファイル操作を完了してから電源オフ
または再起動を実行してください。

## ビルド

elf2x68kを導入し、`m68k-xelf-gcc`に`PATH`が通ったWSLまたはLinux環境でビルドします。

必要なホストツールをインストールします。

```sh
sudo apt install python3 curl unar
```

すべての成果物を作成します。

```sh
make -C src
```

MakefileはHuman68k 3.02とバージョンを固定した`xdftool.py`をダウンロードし、次の
ファイルを作成します。

```text
src/shutdown.x     Human68k実行ファイル
src/shutdown.xdf   起動可能なHuman68kディスクイメージ
dist/shutdown.zip  Human68k許諾条件を含む配布用アーカイブ
```

生成したディスクイメージの内容を確認する場合は次を実行します。

```sh
make -C src check-xdf
```

生成ファイルは`make -C src clean`で削除できます。ダウンロードした補助ファイルも
削除する場合は`make -C src distclean`を使用します。
