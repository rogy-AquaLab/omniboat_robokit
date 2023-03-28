# プログラミング環境構築

ここではロボキットのプログラムを書き、そのプログラムでロボキットを動かすための環境構築を紹介します。概要は次の通りです。

<!-- no toc -->
1. [VSCodeをインストールする](#1-vscodeをインストールする)
2. [プログラムをダウンロードする](#2-プログラムをダウンロードする)
3. [VSCodeでプログラムを開く](#3-vscodeでプログラムを開く)
4. [必要なVSCodeの拡張機能をインストールする](#4-必要なvscodeの拡張機能をインストールする)

ここではMac, Windowsでの環境構築について紹介します。環境構築でわからないところがある場合は[質問・バグ報告について](./08-contributing.md)を参照してください。

## 1: VSCodeをインストールする

[VSCode](https://code.visualstudio.com/)(**V**isual **S**tudio **Code**)とは、microsoftが無料で公開しているコードエディタです。プログラミングを行う際はこのエディタを使うことが多いです。

### Windows

<!-- TODO -->
PowerShellで↓

```shell
winget upgrade --all
winget install -e --id Microsoft.VisualStudioCode
```

### Mac

<!-- TODO -->
Terminal.appで↓

```shell
xcode-select -install
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install --cask visual-studio-code
```

## 2: プログラムをダウンロードする

[![](https://img.shields.io/badge/ダウンロードはこちら📥%20-%234f88d1.svg?style=for-the-badge&logoColor=black)](https://github.com/rogy-AquaLab/2023_robokit/archive/refs/heads/main.zip)

↑のバッジを押すとzipファイルのダウンロードが始まります。展開して得られるフォルダの中にプログラムが入っています。以降、このフォルダは`ダウンロード`フォルダの中にあるものとします。

## 3: VSCodeでプログラムを開く

```shell
code Downloads/2023_robokit-main
```

## 4: 必要なVSCodeの拡張機能をインストールする

VSCodeでは拡張機能と呼ばれるものをインストールしてVSCodeをより多機能にすることができます。ここでは以下の2つの拡張機能をインストールします。

1. [Japanese Language Pack for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=MS-CEINTL.vscode-language-pack-ja)
2. [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

> **注意**
> VSCodeをインストールした後にVSCodeの日本語化を行なっている場合は、1つ目の拡張機能が既にインストールされています。2つ目の拡張機能を入れるところまで飛ばしてください。

まずは1つ目の拡張機能を入れます。最初にVSCodeでプログラムを開くと、↓のようなポップアップがウィンドウ右下に出てきます。

<img src="./assets/vscode-extension-instruction1.png" style="max-width: 500px">

まずは日本語の言語パックをインストールしてVSCodeを再起動させましょう。再起動後、VSCodeの言語が日本語になっているはずです。

次に2つ目の拡張機能を入れます。日本語化できたのち、ウィンドウ右側には次のようなポップアップが表示されるはずです。

<img src="./assets/vscode-extension-instruction2.png" style="max-width: 500px">

インストール後、必要に応じてVSCodeの再起動などを行なってください。VSCodeのウィンドウ左側に<img src="./assets/vscode-platformio-icon.png" style="height: 2em;">のアイコンが表示されていれば環境構築は完了です。

## 参考

- [winget ツールを使用したアプリケーションのインストールと管理 | Microsoft Learn](https://learn.microsoft.com/ja-jp/windows/package-manager/winget/)
- [Download and install Microsoft Visual Studio Code with winget](https://winget.run/pkg/Microsoft/VisualStudioCode)
- [Homebrew — The Missing Package Manager for macOS (or Linux)](https://brew.sh/)
- [visual-studio-code — Homebrew Formulae](https://formulae.brew.sh/cask/visual-studio-code)
- [PlatformIO IDE for VSCode — PlatformIO stable documentation #Installation](https://docs.platformio.org/en/stable/integration/ide/vscode.html#installation)
