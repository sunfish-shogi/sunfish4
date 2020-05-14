Sunfish
=======

[![Circle CI](https://circleci.com/gh/sunfish-shogi/sunfish4.svg?style=svg)](https://circleci.com/gh/sunfish-shogi/sunfish4)

[English](README.en.md)

Sunfish は将棋の対局プログラムで、 Sunfish4 はその 4 作目です。

過去のバージョン
----------------

- [Sunfish](https://github.com/sunfish-shogi/sunfish)
  + 2008 ~ 2012
  + [WCSC(World Computer Shogi Championship)2011](http://www2.computer-shogi.org/wcsc21/) 及び [WCSC2012](http://www2.computer-shogi.org/wcsc22/) へ出場しました。
  + WCSC2012 新人賞
- [Sunfish2](https://github.com/sunfish-shogi/sunfish2)
  + 2012 ~ 2013
  + [WCSC2013](http://www2.computer-shogi.org/wcsc23/) へ出場しました。
- [Sunfish3](https://github.com/sunfish-shogi/sunfish3)
  + 2014 ~ 2015
  + [WCSC2015](http://www2.computer-shogi.org/wcsc25/) へ出場しました。

リリース一覧
------------

https://github.com/sunfish-shogi/sunfish4/releases

使い方
------

[Releases](https://github.com/sunfish-shogi/sunfish4/releases) から最新版 (Latest release と書かれたバージョン) の `sunfish4_v～_win64.zip` をダウンロードして下さい。

ファイルを解凍後、 `sunfish_usi.exe` を USI 対応アプリ ([Shogidokoro](http://shogidokoro.starfree.jp/), [Shogi GUI](http://shogigui.siganus.com/), [ShogiBrowserQ](https://www.sbrowser-q.com/) など) に登録してください。

ビルド方法 (開発者向け)
-----------------------

### 必要なもの

- CMake
- C++ Compiler(GCC4.9/Clang/MSVC2013)

### GNU Make の場合

```
# TARGET には "usi", "csa", "test" などを指定する。
make TARGET
```

### Xcode の場合

```
# "test" の部分は "usi", "csa" などに適宜置き換える。
mkdir -p out/xcode_test       # Xcode プロジェクトのディレクトリを作成
cd out/xcode_test
cmake -G Xcode ../../src/test # CMake を実行
```

### Visual Studio の場合

CMake へ以下のように値を設定して実行します。
(target は `usi`, `csa`, など)

| Name                        | Value                       |
|:----------------------------|----------------------------:|
| Where is the source code    | path/to/sunfish4/src/target |
| Where to build the binaries | path/to/sunfish4/vs/target  |

Makefile の Build Targets (開発者向け)
--------------------------------------

### CSA クライアント

CSA プロトコルで通信するアプリケーションをビルドします。
["CSA network Protocol"](http://www2.computer-shogi.org/protocol/) を参照。

```
make csa          # ビルド
vi config/csa.ini # 設定ファイルを記述
./sunfish_csa     # 実行
```

### USI エンジン

USI(Universal Shogi Interface) は将棋の思考エンジンと GUI アプリケーションが通信に関する規格です。
["About USI Protocol"](http://www.geocities.jp/shogidokoro/usi.html) を参照。

`src/usi` をビルドします。
例えば make コマンドを使う場合は次のようにします。

```
make usi

# デバッグモードの場合
#make usi-debug
```

`sunfish_usi.exe` または `sunfish_usi` を GUI アプリケーションに登録します。

### ユニットテスト

```
make test
```

### 実験

```
make expt
./sunfish_expt --help
```

### プロファイリング

```
make prof
```

### ベンチマーク

```
make bm
./sunfish_bm
```

### 機械学習

```
make ln
vi config/batch_learn.ini
./sunfish_ln
```

### 開発ツール

```
make dev
./sunfish_dev --help
```

ライセンス
----------

MIT License
