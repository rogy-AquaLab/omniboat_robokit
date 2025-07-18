# ドキュメント開発

このドキュメントは[mdBook](https://github.com/rust-lang/mdBook)を利用してGitHub Pagesの内容が生成されています。mdBookの詳細な使用方法に関しては、[mdBook Documentation](https://rust-lang.github.io/mdBook/)を参照してください。また、ドキュメント生成の際には以下のツールが必要です。

- [python](https://www.python.org)
- [curl](https://curl.se)
- [jq](https://jqlang.org)

## 画像の使用に関して

ドキュメント内では画像を使用することができます。`docs/assets`フォルダー内に使用したい画像ファイルを置き、ドキュメントのMarkdownファイルからその画像ファイルへの相対パスを用いて参照することができます。コミット[`b66c64a`](https://github.com/rogy-AquaLab/omniboat_robokit/commit/b66c64a3dd0157e4cfd909bbdfc5783c783d65d1)などを記述例として参照してください。

<div class="warning">

SNS等に画像をアップロードする場合とは異なり、gitリポジトリでは **[Exif情報](https://developer.mozilla.org/ja/docs/Glossary/EXIF)がそのままになっています。** exiftoolやImageMagickなどを用いて適切な処理を施したものを使用してください。

参照: [exif情報をコマンド一発で削除する #exif - Qiita](https://qiita.com/hori@github/items/b67e8fd5662dc3d77655), [ImageMagickでメタデータから不要な情報を削除する｜e](https://note.com/educator/n/nb08798f20f96)

</div>

## Google Driveの画像を使用する場合

Google Driveに保存されている画像ファイルは[Google Driveの画像をMarkdownで直接表示したい時に読む記事](https://zenn.dev/catallaxy_dev/articles/googledrive-images-directly-in-markdown)などで示される方法に従って参照することができます。しかしこの方法ではGitHub Pages上で画像が表示されない([#99](https://github.com/rogy-AquaLab/omniboat_robokit/issues/99))ため、一工夫加える必要があります。具体的には、以下の手順に従ってください。

**1: 使用したい画像のリンクと名前を控える**

ここでは画像のリンクを`https://drive.google.com/uc?export=view&id=xxx`, Google Driveに保存してある画像の名前を`xxx.jpeg`とします。

**2: 控えた情報をスクリプトに追記する**

`docs/scripts/fetch-gdrive-assets.bash`の`python3 ./docs/scripts/curl-parallel.py docs/gdrive << 'EOF'`とある行の次*など*に以下の内容を追記します。

```bash
python3 ./docs/scripts/curl-parallel.py docs/gdrive << 'EOF'
...
https://drive.google.com/uc?export=view&id=xxx xxx.jpeg
...
EOF
```

適当な場所に追記してください。

**3: Markdownで画像を参照する**

先ほどbashファイルに追記したことで、mdBookのビルド時に目的の画像が`docs/gdrive/xxx.jpeg`に配置されるようになります。Markdownファイルでは相対パスで、以下のように記述することで画像を参照可能です。

```markdown
![](./gdrive/xxx.jpeg)
```

<div class="warning">

こちらもSNS等とは異なり、**Google DriveはExif情報を編集しません。**

</div>
