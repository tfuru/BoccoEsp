# BoccoEsp
[BOCCO API β](http://api-docs.bocco.me/index.html)  を ESP8266 core for Arduino で利用する為のライブラリ

| API | 実装 | 追加 権限 |
|:-----------:|:------------:|:------------:|
| アクセストークンの取得 | ◯ |  |
| チャットルームの取得 | ◯ |  |
| テキストメッセージの送信 | ◯ |  |
| 音声メッセージの送信 | ☓ |  |
| 画像メッセージの送信 | ☓ |  |
| メッセージ一覧の取得  | ☓ | ◯ |
| イベントの取得 | ☓ | ◯ |
| メッセージを既読にする | ☓ | ◯ |

## 依存
事前に以下のライブラリを利用しているので Arduino IDE でインストールしておく必要があります。
[WiFiManager](https://github.com/tzapu/WiFiManager)  
[ArduinoJson](https://github.com/bblanchon/ArduinoJson)  
[ESP8266TrueRandom](https://github.com/marvinroger/ESP8266TrueRandom)  

## 動作確認ずみESP8266モジュール
[ESPr Developer（ESP-WROOM-02開発ボード）](http://amzn.to/2f6g4zw)  
電源回路やリセットスイッチ,タクトスイッチがついていてスケッチ開発に非常に便利なのでオススメです。

## インストール方法
このリポジトリを zip でダウンロードして Arduino IDE から 指定してインポートをする。  
1. zip ダウンロード 
2. Arduino IDEを開く
3. スケッチ → ライブラリをインクルード → zip形式のライブラリをインストールを選択する.
4. 1でダウンロードした zip ファイルを指定する。

## サンプルコード実行
Arduino IDE で サンプルを読み込んでビルドして実行する。  
1. Arduino IDEを開く  
2. ファイル → スケッチ例 → BoccoEsp → sample を選択  
3. ビルドする



