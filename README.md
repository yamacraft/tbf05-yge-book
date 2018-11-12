『team Y.G.E. Developers guidebook』 Re:VIEWプロジェクト
=====================================================

2018年10月8日に開催された技術書オンリーイベント『[技術書典５](https://techbookfest.org/event/tbf05)』にて頒布した同人誌のRe:VIEWプロジェクトです。

紙版を印刷所（ねこのしっぽ様）に入稿した際の本文PDFファイル、並びにboothにて販売している電子版のPDFファイルはこのプロジェクトから生成しています。

[team Y\.G\.E\. Developers guidebook \- team Y\.G\.E\. \- BOOTH（同人誌通販・ダウンロード）](https://team-yge.booth.pm/items/1035820)

## ビルド方法

Docker（Docker-compose含）がインストールされているマシンで以下のコマンドを実行することで、PDFファイルが生成されます。

```
$ docker-compose -f build-compose.yml up --build
```

## 本書についてのお問い合わせ

カジュアルなノリでのお問い合わせはTwitter、かしこまった形式でのお問い合わせが必要な場合はGoogleフォームからご連絡ください。

- [山田航空\(@yamacraft\)さん \| Twitter](https://twitter.com/yamacraft)
- [yamacraft 連絡フォーム](https://docs.google.com/forms/d/e/1FAIpQLSeOZkKzr5AuRXSN4MvT3K4RqlytB_cF-Y1e35XnMSYDr4xkLQ/viewform)

## 謝辞

本プロジェクトは、techbooster様とvvakame様が公開されているRe:VIEWプロジェクトをベースに作成しました。特にPDFのグレイスケール化とリンクの文字色並びに下線部非表示処理は大変お世話になりました。ありがとうございます。

- [TechBooster/C89\-FirstStepReVIEW\-v2: 技術書をかこう！はじめてのRe:VIEWは技術書の執筆ノウハウ本のリポジトリです](https://github.com/TechBooster/C89-FirstStepReVIEW-v2)
- [vvakame/C92\-Splatoon2Guide: 負けたくない！Splatoon2本のリポジトリ](https://github.com/vvakame/C92-Splatoon2Guide)