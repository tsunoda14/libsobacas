# libsobacas.so: PC/SCライブラリ(pcsc-lite)互換のECM復号ライブラリ

過去に2ch「Linuxでテレビ総合」スレでupされたソースを基にライブラリ化。  
PC/SCを介して実際のBCASカードで行なっていた処理を置き換えて、このライブラリだけで実現する。  
実際のECM復号機能はlibdyakisobaに依存しており、
本ライブラリはlibyakisobaの(PC/SC APIを提供する)ラッパーライブラリである。

## ビルド

1. 依存ライブラリの[libyakisoba](../libyakisoba/)をインストール
2. 本ライブラリのビルド・インストール
```
$ git clone [--depth 1] https://github.com/..../libsobacas
$ cd libsobacas; autoreconf -i; mkdir build; cd build
$ ../configure [--prefix=...]
$ make; sudo make install
```

### [注意]

デフォルトでは`--prefix=/usr/local`で構成されるので、
本ライブラリは他のシステムライブラリとは別に  
`/usr/local/lib[64]`, `/usr/local/include`, `/usr/local/etc`にインストールされる。  
Fedora等のシステムではデフォルトで`/usr/local/lib64`を`ldconfig`が検索**しない**ので、
```
$ echo /usr/local/lib64 > /etc/ld.so.conf.d/local64.conf
```
などの追加設定が必要となることに注意。  
これらの設定やインストールを行わずに、LD\_LIBRARY\_PATHを指定して直接利用することも可能。


## 今までPC/SCのライブラリを使用していたプログラムでの置き換え方法

- プログラムの実行時にLD\_PRELOADを使ってlibsobacas.soをロードする。
```
ex. $ LD_PRELOAD=/path/to/libsobacas.so client-program ....
```

または
- `-lpcsclite`の代わりに`-lsobacas`でプログラムをリンクし直す。

