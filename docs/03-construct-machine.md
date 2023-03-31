# オムニボートの機体組み立て方法
<span style="color: red; ">**分からないことがあったら遠慮なく聞いてください!!**</span>


**パーツが破損するなど、予備が必要な場合も遠慮せずに申し出てください!!**

## 注意点
### パーツは優しく扱おう
3Dプリンターで出力したパーツは一般にプロトタイピング用のパーツとして扱われており、その分他の造形方法に比べて脆いです。<span style="color: red; ">**やさしく扱ってください。**</span>

### パーツ紛失に注意
小さいパーツが多いのでとても無くしやすいです(体験談)。<span style="color: red; ">**片付いた部屋や机の上で作業する**</span>ことを勧めます。

### 水没厳禁
今回のキットの機械(モーター周りや端子)、回路は防水性能を持ちません。水上で想定されるちょっとした水しぶきを防ぐ程度の防御性能はありますが、DCモーターやサーボモーターが水没すると壊れます。そのため水上実験の際は<span style="color: red; ">**board2より上は水に付けない**</span>ようにしましょう。


### 安全対策
モーターが高速で回転するため、回転中にパーツが外れると非常に危険です。空中で回転実験する際は<span style="color: red; ">**プロペラ部分をのぞきこまない**</span>ようにしてください。もしのぞき込む場合は防護眼鏡などを着用してください。


## 用意するもの
組み立てには次の道具が必要です。工具は一般のご家庭にあるもので問題ありません。そこまで高品質なものでなくとも十分なので100均やホームセンターで売っているものでも大丈夫です。
- プラスドライバー
    - キットに含まれるM2ネジをなめずに回せる様な小さいもの。ドライバーのサイズ規格の目安は#0 or #1
- ニッパー
- ラジオペンチ×2
- ハンマー、かなづち、木づちなど
- 雑巾やタオルのような厚手の布
- ハンマーを使う際の捨て板とする適度に硬い板
    - 木の板などで十分。ただしハンマーでへこんでしまっても良いもの。
- セロハンテープ
- 輪ゴム×4
    - 16号(一般的なサイズ)

## 組み立て手順
### パーツのバリ取り
3Dプリンターの仕組み上、パーツにサポート材と呼ばれる印刷の際の支えが残ってしまう。パーツの滑らかでない面は付属のやすりで丁寧に削る。この精度が後の組み立ての正確性に影響する。

### DCモーターの回転方向の確認
回路説明書に基づいてはんだ付けされたDCモーターは、回路につないだ時互いに反対に回転するようになっているため、回転方向を特定する必要がある。

それぞれのモーターに旗のようにテープを巻き付け、回路につないでモーターを回転させて回転方向を見る。図の方向(軸からモーターを見る方向)から見て時計回りに軸が回転していればCW(clockwise)、反時計回りに回転していればCCW(counter clockwise)と<span style="color: red; ">**テープなどで目印をつけておく**</span>。この後の組み立ては回転方向によって手順が異なる部分があるため、区別しておく必要がある。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1NnLZCr2QoSQeOK8d_1CTcOu5fGkIJJQ4" width=200 /></p>

### サーボホーンの取り付け(2つ作る)
可動域-90°~+90°を持つSG90に<span style="color: red; ">**PWM Period 20ms, Palse Width 1.45ms**</span>のPWMを出力しするとサーボモーターが0°を向く。この状態で、下図のようにサーボホーンが本体と垂直になるようにサーボホーンを取り付け、サーボホーン同梱のネジ(一番小さなものが好ましい)でねじ止めする。次に右図のようにサーボホーン両端の<span style="color: red; ">**外側から2番目の穴**</span>にスズメッキ線を通し、servoConnectorの両端の穴にも通してぐるぐる巻きにすることでこれらを固定する。上からのぞいた時に中心の穴を通してサーボホーン取り付けネジが見えていることを確認する。

この部分を間違えるとプロペラの推進方向の可動域が変わるので要注意。取り付け後にもう一度PWM Period 20ms, Palse Width 1.45msのPWM信号を流して、サーボホーンが下図のようにサーボモーターと直角の向きになればうまくできている。

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=12ME7Xeg_XfVyJiwRXnjB20jwG0ZKBmt8" width=180 />    <img src="https://drive.google.com/uc?export=view&id=1PRk7nVO3amvb6OxNI0-vz9hpQDxor_4o" width=180 />
</p>

### サーボモーターとDCモーターの取り付け(2つ作る)
サーボモーターの軸がDCモーター側に来るようにboard2に取り付ける。図のようにサーボホーンをM2×10ネジとナットで固定し、DCモーターをはめ込む。DCモーターは特にネジなどで固定しない。モーターに張っておいたCW,CCWを区別するテープは見やすい位置に張り替えておくとよい。
<p align="center"><img src="https://drive.google.com/uc?export=view&id=1gsGabJ_zs99jCpi-WIGXs_zz9pFPm9S5" width=200 /></p>

### motorCoverをかぶせる(2つ作る)
motorCoverの横穴にモーター、サーボモーターの配線を内から通し、motorCoverをかぶせる。<span style="color: red; ">**motorCoverの大きな配線用横穴がDCモーター側に来るように組み立てる。**</span>

<p align="center"><img src="https://drive.google.com/uc?export=view&id=16pc9SEHGkV5IHctwzKgKhw27AfOJ3xWB" width=200 /></p>

### pillar1、pillar2を取り付ける(2つ作る)
board2とmotorCoverのDCモーター側のはじに空いている2mm穴にワッシャーを先に通したM2×15ネジを入れる。左図のようにpillar2(又はM2ナット)を先に通し、あとからpillar1を取り付ける。pillar2は方向が正しければboard2やpillar1の溝にぴったりはまるようになっている。pillar1の穴にはネジ切り加工がされているため、ネジを締めることで固定できる。なお、このpillar2は船体取り付け用の取っ手であるが、M2ナットに置き換えて組み立てることが可能になっている。そのため作る船体の形状にから見てpillar2が邪魔になる場合はこれをM2ナットに置き換えてもよい。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1ff9NKm6znETd7WJIL7DMcZ3lYKo9_XHs" width=200 /></p>

### board1を取り付ける(2つ作る)
同じ要領でboard2、motorCoverのサーボモーター側の2mm穴にワッシャーを通したM2×15ネジを通し、board1の柱の部分の穴にねじ止めする。この穴もpillar1同様にネジ切り加工がされている。

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1AS6mrPOncnSq8a68yxQnwwvNgj-GkZCx" width=180 />
<img src="https://drive.google.com/uc?export=view&id=1UgqDDgb4ttHz66kYFX5iZ0lHqvzK-8_Q" width=180 />
</p>

### board1とpillar1,2をねじ止めする(2つ作る)
pillar1,2をつけた時と同じ要領でboard1とpillar1の間にもう一つのpillar2(又はM2ナット)を挟んで、先にワッシャーを通したM2×15ネジでねじ止めする。なお、このpillar2は船体取り付け用の取っ手であるが、M2ナットに置き換えて組み立てることが可能になっている。そのため作る船体の形状にから見てpillar2が邪魔になる場合はこれをM2ナットに置き換えてもよい。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1EbXZl0ilSZsssofCAOuk0Aeox8csUG3R" width=200 /></p>

### board1にフランジブッシュを取り付ける(2つ作る)
フランジブッシュは3種類同梱されている。そのうち内径が小さいもの(内径2mm)をboard1の小さい穴にはめる。内径が大きいもの(内径3mm,袋に入っている)は2種類あるが、長さが短いもの(長さ3mm)の物をboard1の大きいほうの穴にはめる。はめた後は図のようになる。
<span style="color: red; ">**はめる向きを間違えるとプロペラが上手く回らない恐れがある**</span>ので注意する。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1yY1cP0qCtzZgHZ32mx-YapXnAIUQyrBj" width=200 /></p>

### シリコンチューブカップリングを作る(2つ作る)
同梱されている2種類のシリコンチューブ(細く長いもの×2と太く短いもの×4)を用意する。太いチューブを細いチューブの両端にはめ込み、図のようなカップリングを作る。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1xQ9frnsAfyMa-Yuo8UUKy-Dsj6gyi7Qi" width=200 /></p>

### Φ2真鍮シャフトをpulley1に圧入する(2つつくる)
図のように下のテーブルなどが傷つかないように捨て板を用意して、そのうえで作業することを勧める。ハンマーを短く持って<span style="color: red; ">**Φ2の真鍮製シャフトがゆがまないように気を付けながら**</span>pulley1の穴に奥まで打ち込んで圧入する。木づちやゴム製のハンマーの場合は雑巾などを挟むとハンマーが傷つきにくい。

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1Atr6WnDFc-E5m13HT9SrVj9lmlojRefx" width=180 />
<img src="https://drive.google.com/uc?export=view&id=1uxaMr3Wk7R9FYDjT7tKY_b6meesPuVix" width=180 />
</p>

### motorshaftStopperにネジをつける(2つ作る)
M2×10ネジを図のようにmotorshaftStopperに2つ通し、ナットで締める。ここでは切り込みが入ったほうのナットは緩めておく。切り込みがないほうのネジとナットは、このパーツの重心が中心からずれることによって生じる振動の対策なので、回転対称になるようにねじをつけるとよい。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1bkWYkLBfxFbxwv7rj-aDdUspC8WE2wiO" width=200 /></p>

### pulley1のシャフトをDCモーターにつなげる(2つつくる)
左図のようにboard1のDCモーターと同軸上のフランジブッシュに真鍮シャフトのついたpulley1を通し、そこにM2ワッシャー2つとmotorshaftStopperを通す。つぎに右図のように先に作ったカップリングでDCモーターのシャフトと真鍮シャフトをつなぎ、motorshaftStopperをboard1の近くに動かして切り込みのほうのネジを締めることで軸に固定する。固定できたらpulley1を回してみて特に引っ掛かりがなく回ることを確認する。
<span style="color: red; ">**ここまでの段階で同じものが二つできているはずである。**</span>

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1kudI_gUDvmazpXuXopX83mZ0ZC6LIms5" width=180 />
<img src="https://drive.google.com/uc?export=view&id=1eUluc42uEhR5JCbpzReUKo8A7LqOKYme" width=180 />
</p>

### pulley2の下準備(2つ作る)
pulley2の各2mm穴に<span style="color: red; ">**皿ねじ用の面取りがされている方からM2×40の皿ねじを通し**</span>、反対からM2ワッシャー、M2スプリングワッシャー、M2ナットの順で通し、ナットを締める。これを5つの穴すべてに行う。続いてネジが出ている方と同じ方向に面が出るように内径3mm、長さ6mmのフランジブッシュを中心の穴に取り付ける。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1dbhbVPmocNYxuC6dgz-IaoDzoRTzy8Mb"
width=300 /></p>

### linkの圧入とネジ固定(２つ作る)
linkの面取りがされていない、内径3mmの非貫通穴にpulley1と同じ要領でΦ3メカニカルシャフト(銀色のシャフト)を奥まで圧入する。次に、Φ3シャフトとは反対方向にネジが出るようにM2×35(又はM2×30)の皿ねじをもう一方のネジ切りされた穴にねじを回して通す。ネジが入り切ったらM2ワッシャー、M2スプリングワッシャー、M2ナットとネジに通していき、ナットをラジオペンチで回してしっかり固定する。それができたらΦ3シャフトを右図のようにpulley2に通す。

<span style="color: red; ">**左図のようにリンクのΦ3シャフトとネジが平行になっていることを確認する。**</span>少し曲がっている場合は予備のlinkに入れ替えて試してみる。平行でないと後の引っかかりの原因となる。

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1pI5aElj6qVAAyscx3fNOTEJwPrkgFAL8" width=180 />
<img src="https://drive.google.com/uc?export=view&id=1BxwFDRLr0_ka-6UUgtQLugg2pJYhn22l" width=180 />
</p>

### [<span style="color: red; ">重要</span>]プロペラを通す(<span style="color: red; ">CW,CCWで違うので注意</span>)

#### プロペラを仕上げる(10個すべてのプロペラに行う)
プロペラは2つの袋に梱包されており、それぞれにpropeller1~5が入っている。混同しないように袋ごとに作業することを勧める。

プロペラはこのキットの中で最も繊細な部分であり、ここに引っかかりがあるとモーターが上手く回らない。郵送されたそのままの状態ではバリがあるので、まずニッパーで大まかなバリを落とした後、付属の紙やすりの目の粗いものから細かいものへとバリの部分にやすりがけをしていく。続いて、リンク溝や羽に空いた2mm穴の部分にM2×35皿ねじの余りを通してみて、スムーズに通るか確かめる。もし引っ掛かりがあるようなら、そのままねじをギコギコとこすって削る。この作業をすべてのpropellerに行う。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1PKJyDHTCEGj0VBRpHGuF4akCF58d5A6_" width=300 /></p>

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1P7kxfSdgCIBjrbeLjjSyIUsjXXFRny7P" width=180 />
<img src="https://drive.google.com/uc?export=view&id=1qJCAIY1slscEVyO0js-K_1m9V6U2BWZe" width=180 />
</p>

#### CW版
まずは最上図のように羽の丸いほうを左に向けた状態で左上から左下に階段状にリンク溝部分が出るように並べる。この状態で左から順にpropeller1,2,3,4,5である。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1mN4S9YIdgCv7xrDjZUfZNCYWZvPk6k0o"
width=300 /></p>

CW版では、<span style="color: red; ">**propeller1から順に先のように並べた状態の上側から**</span>pulley2のねじとlinkのねじに通していく。
propeller1を通す前にpulley2のねじにM2ワッシャーを通し、linkのねじ(以降リンク軸と呼ぶ)にもM2ワッシャーを通す。その上からpropeller1を羽の短い方向から通す。propeller1を通した後は下図のようになる。

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1-AJ33Vbq51AJvHLd4eoVGYf7PdD31X68" width=180 />
<img src="https://drive.google.com/uc?export=view&id=19W3Wri2sfiRAemRsqOcFXE72-CJ7A63e" width=180 />
</p>

propellerを一つ通すごとに、pulley2を抑えてΦ3シャフトを引きながらまわして引っ掛かりがないか確かめる。propellerがしっぽを振るような動きをするはずである。これが手で十分回るなら問題ない。もし引っ掛かりがあるようなら新たにはめたpropellerが原因の可能性が高いので、疑わしいpropellerに対して再びpropellerのやすり掛けの工程を行う。

続いて、propeller2を通す。以後、propellerはpropeller1を通したところから星を描くように順番に入れていく。これは羽の不均一をなるべくなくすためである。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1dbhbVPmocNYxuC6dgz-IaoDzoRTzy8Mb"
width=200 /></p>

propeller2を通す前にそれを通すpulley2のねじとリンク軸にM2ワッシャーを通す。以降propeller3~5も同様である。五つ通した状態が次の図である。

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1yDSKdBht5OyjoiGsjdb_HhvdWVW1XIDq" width=180 />
<img src="https://drive.google.com/uc?export=view&id=1EIkQQsmZtGoCFg-kKbolYCIV2PJXqP7b" width=180 />
</p>

最後にリンク軸にM2ナット、M2スプリングワッシャー、M2ナットの順に通す。このようにナットを2つ使って閉めることをダブルナットという。

このダブルナットはpropellerに近すぎると摩擦が生じて引っ掛かり、逆に遠すぎるとがたつきが生じて引っかかりやすくなることがある。そのため、<span style="color: red; ">**下図のように少し遊びを持たせて締める**</span>。この際、linkとpulley2のフランジブッシュのつばが密着した状態で締める。締め方は、1つのラジオペンチで奥側のナットを持ち、手前のナットをもう一つのラジオペンチで回すことでしっかり締めることができる。間に挟んだスプリングワッシャーがしっかりと圧縮されていることを確認しよう。(下図は説明と異なりダブルナットに普通のワッシャーを挟んでいるが、スプリングワッシャーを挟むのが正しい。)

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1UIyENcV4ctywLtrUtf-deBSEpoEzRmtb" width=300 />
<img src="https://drive.google.com/uc?export=view&id=10KC0tPSRsgoDAMTn1vP-WrOQrdHmPeui" width=150 />
</p>

下記ののURLの動画くらいに滑らかに回転していれば十分である。

[なめらかさ目安動画](https://drive.google.com/file/d/1gpxRMjuuxmHPymj4KCzXW7L7yiWkX1sZ/view?usp=sharing)

#### CCW版
基本的にはCW版と同じであるが、羽を入れる順番が異なる。CW版ではpropeller1から順に上側から通したが、<span style="color: red; ">**CCW版ではpropeller5から降順に、下側から**</span>pulley2のねじとリンク軸に通していく。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1mN4S9YIdgCv7xrDjZUfZNCYWZvPk6k0o"
width=300 /></p>

その他の手順は同様で、propellerを通す前にワッシャーを通しておくことを忘れないように気を付ける。すべてとおしたらCWと同様にダブルナットを締め、滑らかに回転することを確かめる。下記のURLの動画くらいに滑らかに回転していれば十分である。(動画はCW版のものと同じで、回っているのはCW版)

[なめらかさ目安動画](https://drive.google.com/file/d/1gpxRMjuuxmHPymj4KCzXW7L7yiWkX1sZ/view?usp=sharing)

正しくできていればCW版とプロペラの向きが異なるものになる。

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1Ewu6PLKWO77-PzZzIhG-gou-xvyKr4tT" width=180 />
<img src="https://drive.google.com/uc?export=view&id=1UUVAjoYsW348jvy5YNhACUE08qsuieAV" width=180 />
</p>

### プロペラの軸をサーボモーターにつなげる
続いてサーボモーターの回転軸と同軸上のフランジブッシュにプロペラのΦ3シャフトを通し、先端にセロハンテープを2-3周巻く。この軸をservoConnectorに下図のように通す。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1cc9prUEmrgvw7dSkI0OfN6JcpeSAfPUW"
width=300 /></p>

ワッシャーを先に通したM2×15ねじをservoConnectorの横穴に上図のように通し、反対からM2ワッシャー、スプリングワッシャー、ナットの順に通して緩く締める。この段階ではきつく締めない。

次に、サーボモーターが0°(-90°~+90°の可動域としたとき)になるようにPWM信号を入力する。サーボホーン取り付け時の過程で間違いがなければ<span style="color: red; ">**サーボホーンは真横を向くはず**</span>である。

サーボが0°の状態で下図の方向から見て真右側にリンク軸が出ている状態にしておく。下記の図は左がCW版、右がCCW版である。どちらも図の見方でリンク軸が右に偏心している状態が正しい。こうすることで、CW版は図の下側に水をふき、CCW版は図の上側に水をふくようになる。そのため<span style="color: red; ">**CW版が船体の前方、CCW版が後方**</span>となる(サンプルプログラムを使用した場合)。

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=14qg8Wtg9Zc8OJ7AtAYM3DAyJsznzGN_R" width=200 />
<img src="https://drive.google.com/uc?export=view&id=1fxEruFGVulIwRY2xQwjbk00RItD0bNCU" width=180 />
</p>

Φ3シャフトをラジオペンチなどで持ってサーボホーン側になるべく入れ込んでおく。この際あまり奥に入れ込みすぎると摩擦が大きくなるので、<span style="color: red; ">**下図のようにboard1のフランジブッシュのつばとpulley2の間に0.5mm~1mm程度の遊びがあると滑らかにまわってくれる**</span>。この状態でサーボを0°にするようにPWMを流し、上図のようにリンク軸の方向が真右になっていることを再び確認してからナットを占めてΦ3シャフトを固定する。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1qWPsuiLVGXnUcw1lQfgnoAcF_wK36tRu" width=300 /></p>

Φ３シャフトを固定したらpulley2を回してみて滑らかに回ることを確かめる。下記の動画くらいの滑らかさが目標となる。

[なめらかさ目安動画](https://drive.google.com/file/d/16h_Z8pa_WN6MJG2Ct8YLRuoBxXhaIb-m/view?usp=sharing)

### 輪ゴムをかけて完成!!
pulley1とpulley2の間に輪ゴムを2つかけて完成!(下図はCW版)
回路につないで動作チェックしよう!
この機械のプロペラ部分は壊れやすいので、保管する際はプロペラ部に力がかかりすぎないように注意する。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1k8pxcMDcT4UT6MVvpN3XkhouvbQEjoIT" width=300 /></p>

## 船体作成のヒント
オムニボートの動力となる部分は完成しましたが、このままでは沈んでしまうため浮力を備えた船体部分を作る必要があります。次のヒントを元に、自分で作ってみましょう。

<span style="color: red; ">**注意**</span>:このキットの電子部品、回路は防水ではありません。水の飛沫が少し飛ぶ程度なら問題ありませんが、水にモーターを付けると死にます。回路が壊れる可能性もありますので、DCモーターやサーボモーター、回路、ケーブルの金属がむき出しになっているところは<span style="color: red; ">**絶対に水に付けないでください**</span>。

### プロペラどうしをつなぐ「骨」を作るのがおすすめ
プロペラモジュールだけでは当然沈んでしまうため、発泡スチロールなどの浮力材をつけなくてはいけません。しかし発泡スチロールは脆いので、直接発泡スチロールにプロペラモジュールをつけると壊れやすいです。そのため、先にプロペラモジュールどうしをつなぐ”骨”を作っていおいて、その骨に浮力材で肉付けしていくのがおすすめです。

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1gGwioUOrYLnrHhgIDZgzyZCxp2RGbOl2" width=400 /></p>

<p align="center"><img src="https://drive.google.com/uc?export=view&id=1GXc7Z-VmRg5DAIz07nGuOUdrzeqFysZA" width=400 /></p>

### 回路を保護する
回路と電池ボックスが入るような市販のタッパーのような容器(100均で十分)でこれらを水しぶきから保護しましょう。

### (作例)

100均のタッパーを用いて回路を保護しています。完全な防水性能はないですが、水しぶきを防ぐには十分です。万が一の回路の短絡を防ぐために、基板の背面には絶縁体(養生テープ)を貼っています。

<p align="center">
<img src="https://drive.google.com/uc?export=view&id=1SeET5WbTV-mojgHpr7eyNtIcnQEOLmJL"
width=180 />
<img src="https://drive.google.com/uc?export=view&id=1g0aUpJN_FavBg12DZqMJQqDU3u4dz8NT"
width=180 />
</p>


### 船体のバランスを考える
基本的に水上(水中)ロボットは重心が下で浮心が上になるように設計します。ですが、今回のモーターモジュールの設計では水に付けてはいけないモーターが上の方に来ており、回路も水につけてはいけないため上の方に置くことが想定されます。つまり、<span style="color: red; ">**今回のキットで船を作ると重心のほうが浮心より上に来てしまう可能性が高いです**</span>。特に工夫なくモーターモジュールに発泡スチロールなどの浮力を付けただけでは転覆し、回路が水没してしまう恐れがあります。もし、一度作った船を手で支えたまま水につけてみて、<span style="color: red; ">**横転するようなトルクを感じる場合は船体の両サイドにペットボトルや発泡スチロールで作った浮きをつけてみてください**</span>。こうすることで、機体を転覆させるようなトルクがかかったとしても両端の浮力が外側でそれを打ち消すように働き、船体の姿勢を安定化してくれます。

#### (作例)
アクリル板で骨組みを作りシュナイダープロペラユニットを固定しています。100均の発泡スチロールを浮力材とし、その上に電池や回路が入った100均のタッパーを配置しています。
<p align="center"><img src="https://drive.google.com/uc?export=view&id=1qL_pSGlj9fTZf1VZoKK8d8WW6qs6rTY9"
width=400 /></p>

#### (作例の設計の問題点)
浮力材である発泡スチロールはただ四角く切り出しただけであり、水の抵抗が大きい形状です。この形を工夫することで、目的の方向への直進性を高めることができると考えられます。形を工夫してみましょう！

