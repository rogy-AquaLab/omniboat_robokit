# 制御

## 基本的な開発フロー

[GiHubフロー](https://docs.github.com/en/get-started/using-github/github-flow)を前提とします。

## プログラム開発

配布するプログラムは[PlatformIO](https://platformio.org)を利用して開発されています。特に[mbedフレームワーク](https://docs.platformio.org/en/latest/frameworks/mbed.html)を利用し、フォーマッターに[clang-format](https://clang.llvm.org/docs/ClangFormat.html), リンターに[clang-tidy](https://clang.llvm.org/extra/clang-tidy/)を使用しています。

使用するエディターは[VSCode](https://code.visualstudio.com)を想定していますが、[CLion](https://www.jetbrains.com/ja-jp/clion/)などPlatformIOが適切に動作する環境であれば特に問いません。

## GitHub Actions

GitHub Actionsを用いて自動的にプログラムのチェックなどが入るようになっています。GitHub Actionsについては[GitHub Docs](https://docs.github.com/en/actions)を参照してください。

<div class="warning">

プライベートリポジトリではGitHub Actionsを使いすぎると課金が発生します。詳しくは[GitHub Actions の支払いを管理する - GitHub Docs](https://docs.github.com/ja/billing/managing-billing-for-github-actions)

</div>

GitHub Actionsの設定は[`.github/workflows`](https://github.com/rogy-AquaLab/omniboat_robokit/tree/main/.github/workflows)以下に保存されています。

### main.yml

このワークフローファイル内のジョブは以下のタイミングで実行されます。

- `main`ブランチへのPull Request作成・更新時
- `main`ブランチの更新時

どちらの場合も、以下の内容が確認されます。

- PlatformIOのビルドが成功すること
- clang-formatによるプログラムのフォーマット確認 (format)
- clang-tidyによるプログラムの簡易なバグチェック (lint)
- mdBookのビルドが成功すること

特にPull Requestの場合は、変更内容に応じてそれぞれチェックが省略されます。詳しくはmain.ymlの`changes`ジョブ、および[dorny/paths-filter](https://github.com/dorny/paths-filter)を確認してください。逆に、変更に対して入ってほしいチェックが行われない場合はここを見てください。

### release.yml

TODO ([#76](https://github.com/rogy-AquaLab/omniboat_robokit/issues/76))

## リリース

TODO ([#76](https://github.com/rogy-AquaLab/omniboat_robokit/issues/76))

## 依存関係管理

プログラムを開発する上で様々なツール、ライブラリが必要になります。一般的にそれらは「依存関係」と呼ばれます。ここではこのリポジトリを管理する上で使用されている依存関係について説明し、それらの更新方法について説明します。

- [ソフトウェアはなぜバージョンアップしなければならないのか #バージョンアップ - Qiita](https://qiita.com/autotaker1984/items/a3091772dbb0fb91473d)

### GitHub Actions

GitHub Actionsの依存関係はDependabotで管理されています。依存関係何か更新があればDependabotが自動でPRを作成してくれます。Dependabotの設定は[.github/dependabot.yml](https://github.com/rogy-AquaLab/omniboat_robokit/blob/main/.github/dependabot.yml)に書いてあります。

Dependabotについてより詳しくは、[Dependabot を使う - GitHub Docs](https://docs.github.com/ja/code-security/dependabot/working-with-dependabot)を参照してください。

### mdBook

GitHub Actions内で使用されるmdBookのバージョンは自動で更新されません。main.ymlおよびrelease.ymlに`MDBOOK_VERSION: vX.Y.Z`のような記述があります。mdBookのバージョン一覧は[Releases · rust-lang/mdBook](https://github.com/rust-lang/mdBook/releases)にリストされているので、そこを見つつバージョンを更新してください。バージョンの更新は1年に1度ほどで問題ないです。

### PlatformIO

PlatformIOに関連する依存関係は2種類あります: PlatformIOそれ自身と、PlatformIO内で使用するツール群です。

**PlatformIO自身**

特に明記せず、最新版のPlatformIOを前提とします。GitHub Actions内でも最新版のPlatformIOを使用するように設定されています。PlatformIO関連で何か不具合があればバージョンの問題を考慮してみてください。

**PlatformIO内の依存関係**

これらはPlatformIOの設定ファイル`platformio.ini`に記述されています。具体的な記述については以下を参照してください。

- ["platformio.ini" (Project Configuration File) — PlatformIO latest documentation](https://docs.platformio.org/en/latest/projectconf/index.html)
- [platform_packages — PlatformIO latest documentation](https://docs.platformio.org/en/latest/projectconf/sections/env/options/platform/platform_packages.html)
- [lib_deps — PlatformIO latest documentation](https://docs.platformio.org/en/latest/projectconf/sections/env/options/library/lib_deps.html)

特にGCCコンパイラのバージョンは[platformio/toolchain-gccarmnoneeabi](https://registry.platformio.org/tools/platformio/toolchain-gccarmnoneeabi)で明記されているため、定期的(こちらも1年おきが目安)に更新する必要があります。
