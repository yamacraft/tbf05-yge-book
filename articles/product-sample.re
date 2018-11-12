={product-sample} Android以外でよく使うコードと解説

//lead{
前章ではAndroidに関連したサンプルコードの解説を掲載しました。本章ではプロダクトのそれ以外の部分（主にFirebase関連）に関するサンプルコードの掲載と紹介を記載します。
//}

== Circle CIでFirebaseの自動デプロイを行わせる

はい、またしても自動デプロイの話です。

今回はCircle CIでFirebaseへの自動デプロイを行っているプロジェクトの解説をします。
サンプルプロジェクトではHostingしか利用していませんが、たとえば各RulesファイルやCroud Functionsの場合もやることに違いはありません。

 * https://github.com/yamacraft/cv

こちらは筆者の職務経歴書を掲載したリポジトリです。
勉強（とアピール）がてらに、職務経歴を記載したcv.mdファイルをPDFに変換、Firebase Hostingのプロジェクトフォルダであるpublicフォルダ内に出力した後、 @<code>{firebase deploy} でFirebase Hostingへアップロードするしくみになっています。

//image[product-flow-cv][cvのCI構成][scale=1.00]{
//}

=== cvプロジェクトの構成

今回は解説に必要な部分だけを抜粋します。
また、Firebaseプロジェクトの本当に基本的なところ（Firebaseプロジェクトの新規作成と、そこで作られるファイル群の説明）は省略します。

 * .circleci ... Circle CIの設定ファイル
 * /public ... Firebase Hostingのプロジェクトフォルダ
 * cv.md ... 職務経歴を記載したMarkdownファイル
 * deploy.dockerfile（+compose.yml） ... デプロイ用Dockerfile
 * output.dockerfile（+compose.yml） ... PDF出力用Dockerfile
 * firebaserc, firebase.json ... Firebase設定ファイル

hostings_local.dockerfileは、ローカルでFirebase Hostingの中身を見るためのDockerfileです。
現在はFirebase CLIの @<code>{firebase serve} でローカルホストを立ち上げられるようですので、特に必要はないかなと思います。

output.dockerfileの中身の詳しい説明は省きます（自分もよくわかっていないので）。
@<code>{docker-compose -f ./output-compose.yml up --build} すれば、publicフォルダ内にPDFファイルが出力されるとだけ覚えておいてください。

=== Firebase deployをDocker上で実施する

deploy自体に難しい作業はありません。
必要なツールをインストールして実施するだけの内容になっています。

//list[cv-deploy-dockerfile][deploy.dockerfile][dockerfile]{
FROM node:6.11-slim

RUN apt-get update
RUN npm install -g firebase-tools

RUN mkdir app
WORKDIR app

ARG FIREBASE_TOKEN="x/xxxxxxxxxxxx"

CMD firebase deploy --only hosting --token ${FIREBASE_TOKEN}
//}

//list[cv-deploy-docker-compose][deploy-compose.yml][yml]{
version: '2'
services:
  deploy:
    environment:
      - FIREBASE_TOKEN
    build:
      context: ./
      dockerfile: deploy.dockerfile
    volumes:
      - ./:/app
//}

composeファイルをわざわざ作っているのは、作成が多少面倒でも @<code>{docker-compose -f ./deploy-compose.yml up --build} というコマンドをたたくだけで済むからです。
composeに設定しているenvironmentのFIREBASE_TOKENですが、ここで値を設定しないことで、同名のシステム環境変数から値を引っ張ってくれます。
なのでCircle CIで利用したい場合、Cicle CIの環境変数設定画面でFIREBASE_TOKENを設定すればよいです。

Firebaseのtokenの説明は、いったん全体の動きの解説が終わってから記載します。

===[column] AlpineイメージとFirebase

Dockerイメージの容量を極力へらすため、本来ならAlpineイメージをベースにすることをお勧めします。
ですがどうもFirebaseに関連するgRPC系のライブラリがAlpineのサポートをしたりしなかったりすることでイメージ作成のエラーが多発していた時期があったため、安定性を重視してslimイメージを利用するようにしています。

===[/column]

=== Circle CIで各種Dockerを実行させる

ということで、必要なDockerfileの作成ができあがったので、あとはこれをCircle CIで必要なタイミングで実行するように設定すれば、自動デプロイの設定は完了します。

//list[cv-deploy-circle-yml][.circleci/config.yml][yml]{
version: 2
jobs:
  output_pdf:
    working_directory: ~/yamacraft-cv
    machine: true
    enviroment:
      - FIREBASE_TOKEN
    steps:
      - checkout
      - run:
          name: PDF output
          command: |
            docker-compose -f ./output-compose.yml up --build
      - save_cache:
          key: deploy-{{ epoch }}
          paths:
            - ~/yamacraft-cv
  deploy:
    working_directory: ~/yamacraft-cv
    machine: true
    steps:
      - restore_cache:
          keys:
            - deploy
      - run:
          name: deploy
          command: |
            docker-compose -f ./deploy-compose.yml up --build
workflows:
  version: 2
  deploy_hosting:
    jobs:
      - output_pdf:
          filters:
            branches:
              only: master
      - deploy:
          requires:
            - output_pdf
//}

Circle CIのjobで @<code>{macnihe: true} を設定すると、VM上でCircle CIを実行できます。
というより、今回はボリュームマウントが使いたいのですがこうしないと利用できないのが理由です。

あとはworkflows部分を見ていただければ、動きがなんとなく見て取れるかと思います。
masterブランチが更新されたときのみ、PDF出力のjobが実行されます。
成功することで次にdeploy実施のjobが呼び出され、publicの中身がFirebase Hostingsにデプロイされます。

内容そのものは指定された順番にdocker-composeを実施しているため、ローカル環境上でも同様の実行はできます。
せめてこうして、CIとローカルでそれぞれ同じ動作が期待できるように構築しておくことで、いざというときにローカルで実施、といった対応も期待できます。
やりたくはないですが…。

=== Firebase tokenについて

Firebase CLIを実施するには、firebaseにログインする必要があります。
この作業は　@<code>{firebase login} で実行できますが、WebブラウザでGoogleにログインする作業が必要になります。
これではCIでは利用できません。

この問題に対応するため、Firebase CLIには @<code>{firebase login:ci} というコマンドも用意されています。
ログイン自体は通常のものと同じですが、こちらはログイン後、tokenが返却されます。

//list[firebase-get-token][firebase login:ciの実行結果][sh]{
$ firebase login:ci

Visit this URL on any device to log in:
https://accounts.google.com/o/oauth2/auth?client_id=......

Waiting for authentication...

[check] Success! Use this token to login on a CI server:

x/xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

Example: firebase deploy --token "$FIREBASE_TOKEN
//}

この x/xxx〜 がfirebase tokenとなります。

このtokenは、ログインしたGoogleアカウントに紐付いたものとなるため、そのアカウントに権限が付与されているプロジェクトにも流用できてしまいます。
特にdevelopプロジェクトとreleaseプロジェクトに分けている場合、どちらにもアクセス可能なGoogleアカウントでtokenを作ってしまうとセキュリティ的に問題が出てきてしまいます。
注意してください。

ちなみに、 @<code>{firebase logout --token [token]} とすることで、発行したtokenを無効化できます。

=== deploy先を切り替える場合

先程のプロジェクトではデプロイ先のFirebaseプロジェクトは１種類しかありませんでした。
プロダクト構成図で触れていますが、Firebaseプロジェクトでデプロイ先を切り替えたい場合もあります。

その際は、まず必要な分のFirebaseプロジェクトを作成し、対象となるプロダクトの.firebasercに設定を記載しましょう。

//list[firebase-change-project][.firebaserc][json]{
{
  "projects": {
    "release": "project-id",
    "develop": "develop-project-id"
  }
}
//}

既存の.firebasercにはdefaultがありますが、明示的にデプロイ先を選択するように消してしまいましょう。
これで後は @<code>{firebase use XXXX} とコマンドをたたくことでデプロイ先を切り替えることができます。
これと先程のtokenの設定を活用すれば、誤ったデプロイを防ぐことをできるようになるでしょう。 

//list[firebase-change-project-sh][.kirikae][sh]{
# develop環境へdeployを実施する
$ firebase use develop
$ firebase deploy
//}

== Hubot＋SlackのDocker構成

実際にHubotでFirebase RealtimeDatabaseを管理する用に作ったDockerfileと実行部分のソースコードを公開します。

//list[hubot-dockerfile][hubot.dockerfile][dockerfile]{
FROM node:8-slim

RUN apt-get update && apt-get -y upgrade

RUN npm install -g yo generator-hubot
RUN npm install hubot-slack --save
RUN npm install underscore async github@8.2.1 weighted --save

RUN useradd -m hubot
USER hubot
WORKDIR /home/hubot

ARG PROJECT=develop
ARG HUBOT_NAME=slack_bot
ARG HUBOT_SLACK_TOKEN=xxxxxxxxxx

RUN yo hubot --defaults --name=${HUBOT_NAME} --adapter slack
ADD --chown=hubot:hubot ./hubot/package.json package.json
ADD --chown=hubot:hubot ./hubot/external-scripts.json external-scripts.json
ADD --chown=hubot:hubot ./hubot/scripts/* scripts/

CMD cp scripts/*.js scripts/ \
    && cp scripts/*.json scripts/ \
    && bin/hubot --adapter slack
//}

//list[hubot-compose][hubot-release-compose.yml][yml]{
version: '2'
services:
  hubot:
    environment:
      - PROJECT=prod
      - HUBOT_NAME=slack_bot
      - HUBOT_SLACK_TOKEN
    build:
      context: .
      dockerfile: hubot.dockerfile
//}

あとはあらかじめHUBOT_SLACK_TOKENの環境変数をシステムに登録し、 @<code>{docker-compose -f ./hubot-compose.yml up --build -d} を実行するだけで、Hubotが立ち上がります。
この構成であればvolumeマウントする部分もないため、ローカル上でもクラウドインスタンス上でも問題なく動かせるのではないかと判断しています。
実際にEC2やGCEで動かしていないので確証は持てませんが…。

この構成の場合、スクリプトを開発する際もコンテナをrestartするだけで済むはずですので、そこまで実行時間が気になることはないかと思います。
そこも短縮したいのであれば、ぜひこれをさらに改修したものをQiitaあたりに上げてもらえると、私がとてもうれしいです。よろしくおねがいします。

=== Firebase Admin SDK（nodejs）でFirebase Realtime Databaseにアクセスする

今回はHubotから利用するということで、nodejsからFirebase Admin SDKを使ってRealtime Databaseに書き込みを行うまでの簡単なサンプルを紹介します。

npmのfirebase-admin、並びにサンプルではmomentも利用するため、 @<code>{npm install firebase-admin moment} とでもコマンドを叩いてインストールをしてください。 
またserviceAccountKeyはFirebaseのコンソールサイトからダウンロードしましょう。

//image[product-get-serviceaccount][serviceAccountKey.jsonの取得][scale=1.00]{
//}

//list[firebase-admin-sdk-firebase-settings][firebaseSettings.js][js]{
let admin = require("firebase-admin");
let serviceAccount = require("./serviceAccountKey.json");
let app = admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://prototypeapp-dev.firebaseio.com"
});

exports.admin = admin;
exports.app = admin.app();
}
//}

//list[firebase-admin-sdk-put-log][hubot/scripts/respond.js][js]{
module.exports = function (robot) {
  robot.respond(/putLog/i, function (res) {
    // 時間取得にmomentを利用
    var moment = require("moment");
    moment.tz.setDefault("Asia/Tokyo");
    var today = moment();

    // admin権限の取得
    var firebaseSettings = require("./firebaseSettings.js");
    var admin = firebaseSettings.admin;
    var db = admin.database();

    // {root}/logs 以下にログをPush
    // Pushされた場合は自動的に一意のidが割り振られる（/logs/{id}/{data}）
    var logsRef = db.ref(`logs/`);
    logsRef.push({
      time: today.format(),
      message: "set log"
    });
  });
}
//}

Firebase Admin SDKはその名の通りAdmin権限で動作させるためのSDKです。
Admin権限であれば、Rulesで読み書き不可としている場所もAdmin権限にて自由にアクセスできます。
それだけ強力な機能でもあるので、管理ツールで利用するにはもってこいですが、誤ってデータベースを破壊しないように注意しましょう。

===[column] なぜFirestoreではないのか

Unityが現時点（2018年9月）でFirestoreに対応していないためです。あと自分が主にやっているプロダクトが、読み書きよりも同時アクセス数が課金に関わるRealtime Databaseの方が都合がよいためです。

===[/column]

=== Hubotでcron処理を実施する

Hubotのpackage.jsonにnode-cronを追加することで、Hubotにcron処理を実行させることができます。

//list[hubot-cron-package-json][hubot/package.json][json]{
{
  "name": "sample-hubot",
  "version": "1.0.0",
  "private": true,
  "author": "yamacraft <user@example.com>",
  "description": "A simple helpful robot for your Company",
  "dependencies": {
    "cron": "^1.3.0",
    // ...
  }
}
//}

//list[hubot-cron-script][hubot/scripts/cron.js][js]{
const cron = require('cron').CronJob;

module.exports = function (robot) {
    /**
     * 毎日AM0:15に投稿
     */
    new cron('15 0 * * *', function () {
        robot.send({ room: "general" }, "Hello!");
    }, null, true, 'Asia/Tokyo');
}
//}

簡単な処理であれば、Hubotにスケジュール実行させることでさらに運用時の負荷を減らせることになるでしょう。
