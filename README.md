# IoTシステムの設計と可視化

IoTデバイスからのセンサデータをリアルタイムで収集し、機械学習（Isolation Forest）を用いて外れ値の検出とデータの可視化を行うシステムです。

## 実装結果
![実装結果のデモ](./public/visualization.gif)

## プロジェクト概要

本プロジェクトは、以下のコンポーネントで構成されています。
1.  **IoTデバイス (ESP8266/Arduino)**: 照度データと人感センサの状態を測定し、TCP通信で送信。
2.  **リレーサーバー (Python)**: TCP接続を待ち受け、受信したデータをバックエンドへ転送。
3.  **バックエンド (FastAPI/Python)**: データの保存、Isolation Forestによる外れ値検出。
4.  **フロントエンド (React/TypeScript)**: 収集したデータのリアルタイムグラフ表示（外れ値の強調表示）。

## システム構成図

```text
[ESP8266 Device] 
      | (TCP)
      v
[Relay Server (iot.hongo.wide.ad.jp)]
      | (HTTPS via ngrok)
      v
[Backend Server (FastAPI / SQLite)] <--- [Machine Learning (Isolation Forest)]
      | (REST API)
      v
[Frontend App (React / Vite)]
```

## 技術スタック

### Hardware / Device logic
- **ESP8266**: Arduino IDE (C++)
- **TCP Communication**: 低レベルソケット通信によるデータ転送

### Backend
- **FastAPI**: 高速なWebフレームワーク
- **SQLAlchemy / SQLite**: データベース管理
- **Scikit-learn**: Isolation Forestを用いた異常検知
- **Pandas**: データ処理

### Frontend
- **React / Vite**: UI開発
- **Recharts**: データの視覚化（ComposedChartを使用した外れ値のプロット）
- **Tailwind CSS**: スタイリング

## セットアップと実行方法

### 1. バックエンドの起動
```bash
cd Backend
source venv/bin/activate
pip install -r requirements.txt
python app.py
```
*注: 初回実行時は `alembic upgrade head` でデータベースのマイグレーションが必要です。*

### 2. 外部公開 (ngrok)
リレーサーバーからのデータを受け取るため、バックエンドを公開します。
```bash
ngrok http 8000
```

### 3. フロントエンドの起動
```bash
cd Frontend/vite-project
npm install
npm run dev
```

## 機械学習による外れ値検出
`Backend/algorithm/check_outeliers.py` に実装されています。
- **アルゴリズム**: Isolation Forest
- **特徴量**: 照度 (illuminance)、時間帯 (hour)、人感センサの状態 (is_people_detected)
- **動作**: 過去の正常なデータパターンを学習し、それらから乖離したデータ（例：不自然な時間帯の点灯。深夜に人がいないのに明るいなど）を「外れ値」として検出し、フロントエンド上で赤く強調表示します。

## GitHub リポジトリ
[https://github.com/junnamuzaurusu/IoTdevice](https://github.com/junnamuzaurusu/IoTdevice)
