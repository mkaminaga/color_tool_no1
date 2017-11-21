﻿Color Tool No.1
====
正規分布でドットを配置するツール

特徴
----

 * 正規分布に従いドットを配置した画像を出力する
 * GUIによる直観的な操作が可能
 * 8色、24色Bitmap形式で出力が可能

操作方法
----
<img src="doc/screenshot.png" title="screen_shot"><br>

概要
------
上左半分の領域を「プレビュー」、上右半分の領域を「パレット」、下の表示を「色分布」と呼ぶ<br>
本アプリケーションの操作は「色分布」、「パレット」のクリックと3つのボタンで行う

パレット
------
パレットは16 x 16 = 256色の色を設定できる<br>
「Load color」ボタンを押すとパレット色情報ファイルを選択するダイアログが出現する<br>
色はパレット上左クリックで選択する<br>
パレット色情報ファイルはRGBの値をカンマ区切りで並べたテキストファイルである<br>
デフォルトでは[./colors/default.txt](./colors/default.txt)が読み込まれる<br>

色分布
------
色分布中のグラフは平均0、分散1の正規分布のグラフである(横軸目盛り間隔1、小目盛り間隔0.25)<br>
色分布では、おおよそ0から3の範囲を20個の領域に分割している<br>
分割された領域はクリックで選択でき、その状態でパレットの色を左クリックすると、その領域が選択色に設定される<br>
なお、色分布中で色を右クリックするとすべての領域を選択色で塗りつぶすことができる<br>

プレビュー
------
「Generate」ボタンを押すと「プレビュー」に、色分布で示された分布で画像が作成される<br>

ファイル出力
------
「Export」ボタンを押すと、「プレビュー」に示された画像を保存することができる<br>
対応画像形式:<br>
 * Windows形式8bitビットマップ(bmp)
 * Windows形式24bitビットマップ(bmp)

ライセンス
----
MITライセンスで公開する<br>
[LICENSE.md](LICENSE.md)を参照<br>
