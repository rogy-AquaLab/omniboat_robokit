# 2023_robokit

2023新歓で使うロボキット

## Requirements

- [VSCode](https://code.visualstudio.com/)
- [PlatformIO](https://platformio.org/)

以下は各OSごとのインストール方法の記事です。

- Windows
    - [VSCode-PlatformIO IDEを使って、ESP32の開発環境を構築およびLチカ](https://zenn.dev/kotaproj/articles/esp32_vscode_pio)
- Mac
    - [Mac環境でPlatformIO IDEを用いてハードウェアコーディングをする時の環境セットアップについて - Qiita](https://qiita.com/sss463/items/e9f6173f13532960d6c5)
- Ubuntu
    1. [Running Visual Studio Code on Linux](https://code.visualstudio.com/docs/setup/linux)
    2. [Installation · PlatformIO](https://platformio.org/install/ide?install=vscode)

PlatformIOのインストールまでできたら次に進んでください。

## Build

プログラムをコンパイルする方法です。

https://user-images.githubusercontent.com/54711422/226151624-13bab5bc-eb08-43a8-b263-44089ec6c027.mp4

ソースコード(`src/main.cpp`)を開くと右上に表示されます。

## Upload

1. Nucleo F303K8(ロボキットに入っていた白い基板)とパソコンをケーブルで繋ぐ
2. 上述の`Build`ボタンの下にある`Upload`ボタンを押す

コンパイルで生成されたバイナリファイルが基板に書き込まれます。
