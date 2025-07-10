# Astro Golf

学習のために作成したゲーム。

## 操作方法

### 通常プレイ

- ボール左クリックドラッグ -> ショット
- ボール以外左クリックドラッグ -> カメラ移動
- (デバッグ機能) `L-Shift + R-Click` で自由にボールを動かせます
- (デバッグ機能) `F3` で法線情報や重力情報などを表示できます
- (デバッグ機能) `F1` でUIを非表示にできます

### ステージエディター

- `4` キーでエディターに移動

## ゲーム画面

### 起動 → タイトル → ステージセレクト

![](https://github.com/user-attachments/assets/bb815714-5665-47c7-a23b-b951f5b9dcf4)

### ステージセレクト → ステージクリア

![](https://github.com/user-attachments/assets/6af5166f-2a9f-47d0-aa7e-b5902a1e59dd)

### ステージエディター

![](https://github.com/user-attachments/assets/654b3e81-79a8-48f0-b89a-fa73981715e6)

## 新規ステージの追加

1.`assets/data/stages.json` に書き足すことで追加できます。

## 新規惑星グラフィックの追加

1.`assets/data/sprite/planet/` に惑星のpng画像を配置します
2.`assets/data/graphs.json` に書き足すことで追加できます。
