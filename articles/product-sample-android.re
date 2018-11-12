={product-sample-android} Androidでよく使うコードと解説

//lead{
本章では、実際に私がAndroidアプリを開発する際によく使うコード類の紹介とその解説を記載します。
//}

== Circle CIでのAndroid自動デプロイ設定

これはAndroidのコードか？と言われるのはさて置き、どのプロダクトでも共通してよく使うコードはCIの設定です。
特に筆者はCircle CIを愛用しています。GitHubやBitbucketのプライベートリポジトリにも対応しているのが理由です。
GitLabをリポジトリとして使っているならGitLab CIやJenkinsといった選択肢になってしまいますが、基本的な作りはほとんど変わりません。

=== AndroidのCIはそんなに難しくない

現在のAndroidプロジェクトのすばらしいところは、gradleによるCUI上の各種処理が標準で構築されているところです。
そのため、基本的にCIでは「Androidがビルドできる環境を作る」ことさえできれば、だいたいのことができます。

そのため、本項での解説のほとんどは、ビルド環境の構築にほぼ焦点を当てて解説しています。

=== CircleCIのDockerイメージを使おう

本来Android SDKをpublicで公開することはAndroid SDKの再配布となってしまうため、多くは各自でUbuntuのイメージにAndroid SDKをインストールしたDockerfileを作成しています。

ですがCircle CIはライセンスの問題を解決しているのか、すでに必要なAndroid SDKがインストールされた状態のDockerイメージをDocker Hub上に公開しています。
せっかくあるので使ってしまいましょう。

=== .circleci/config.ymlのサンプル

実際に筆者がプロダクトで利用しているymlのサンプルを掲載します。

//list[android-circleci-yml][.circleci/config.yml][yml]{
version: 2

jobs:
  build:
    docker:
      - image: circleci/android:api-28-alpha
    environment:
      JVM_OPTS: "-Xms3072m -Xmx3072m"
      GRADLE_OPTS: |
        '-Dorg.gradle.daemon=false -Dorg.gradle.jvmargs="-Xmx3072m 
        -XX:+HeapDumpOnOutOfMemoryError"'
    working_directory: ~/code
    steps:
      - checkout
      - run:
          name: dependencies
          command: |
            ./gradlew dependencies
      - run:
          name: build and test
          command: |
            ./gradlew testDebugUnitTest build
      - store_artifacts:
          path: app/build/outputs/apk/dev/app-debug.apk
          destination: app-debug.apk
      - store_artifacts:
          path: app/build/outputs/apk/release/app-release.apk
          destination: app-release.apk
      - store_test_results:
          path: app/build/test-results
//}

GRADLE_OPTS の部分は、本書の都合上無理やり２行にわけて記載していますが本来は１行にまとめています。

このymlは単純にユニットテストとビルドの２つを実行し、できあがったレポートファイルとAPKをアーティファクトとして保存する設定になっています。
DeployGateやFabricのデプロイ設定をしたい場合は、各種コマンドをたたくrunを追加するだけで済みます。簡単ですね。

=== 秘匿情報をどのようにして管理するか

当然、リリース作業のデプロイもCIでやりたいわけですが、その場合秘匿情報@<fn>{footnote-secret-data}をどう管理すべきでしょうか。

//footnote[footnote-secret-data][keystoreファイルAPIキーが記載されたファイル、またそれに付随するパスワード]

筆者は、

 * keystore、serviceAccountKeyなどはAES暗号化したものをリポジトリ内で管理
 * 上記の暗号化キー、並びに各種証明書のパスワードはlocal.propertiesもしくはシステム環境変数から取得する
 * 暗号化キーやパスワード自体は各社のセキュリティルールにもとづいた場所で管理
 ** team Y.G.E.はパスワード管理サービスを使って管理しています

という形で管理しています。
最後のキーそのものの管理は悩みどころなのでこれで正解とは言い切れません。
ただプライベートで個人で管理すればよいだけで済むものであれば、二段階認証の必要な場所におくだけもかなり強固に守れると思います。

=== 秘匿情報をlocal.propertiesまたはシステム環境変数から取得する

次に掲載するコードは、local.propertiesまたはシステム環境変数から変数の内容を取得し、（releaseのみ）keyAliasや各種パスワードの値として利用するgradleのサンプルコードになります。
ちなみにlocal.propertiesの中身の方が優先されます。

//list[android-load-properties][app/build.gradle][gradle]{
//...
android {
    Properties properties = new Properties()
    properties.load(project.rootProject
        .file('local.properties')
        .newDataInputStream())

    signingConfigs {
        dev {
            storeFile rootProject.file("debug.jks")
            storePassword "android"
            keyAlias "androiddebugkey"
            keyPassword "android"
        }
        release {
            storeFile rootProject.file('release.jks')
            storePassword properties.getProperty("RELEASE_STORE_PASSWORD"
                , System.getenv("RELEASE_STORE_PASSWORD"))
            keyAlias properties.getProperty("RELEASE_KEY_ALIAS"
                , System.getenv("RELEASE_KEY_ALIAS"))
            keyPassword properties.getProperty("RELEASE_KEY_PASSWORD"
                , System.getenv("RELEASE_KEY_PASSWORD"))
        }
    }

    // ...

    buildTypes {
        dev {
            signingConfig signingConfigs.dev
        }
        release {
            signingConfig signingConfigs.release
        }
    }
}
//}

なお、この処理の場合、local.propertiesファイルそのものがないとエラーが起きてしまいます。
CIなどで実行する場合は @<code>{echo > local.properties} などでビルド前に空のファイルを生成するようにしてください。

=== Circle CI提供イメージを使って、Circle CIに頼らないDockerfileでビルドする

先ほどのコードは、Circle CI上での実行を前提として作られています。
普段はCircle CIでビルドしているので問題はありませんが、何かしらの理由でJenkinsなどでも利用したい場合があるでしょう。
というわけで、Circle CIとほぼ同様の環境下（ユーザー名、ビルド先のパス）で動作するDockerfileとcomposeファイルを掲載します。

//list[android-build-dockerfile][{root}/dockerfiles/ci.dockerfile][dockerfile]{
FROM circleci/android:api-28-alpha

WORKDIR /builds/yamacraft/project

# 表示用に改行していますが、一行にまとめます
ENV GRADLE_OPTS '-Dorg.gradle.daemon=false 
        -Dorg.gradle.jvmargs="-Xmx3072m -XX:+HeapDumpOnOutOfMemoryError"'
ENV JAVA_OPTS "-Xms3072m -Xmx3072m"

USER circleci
ADD . /builds/yamacraft/project

RUN sudo chown -R circleci:circleci /builds/yamacraft/project

CMD tail -f /dev/null
//}

//list[android-build-dockerfile-yml][{root}/dockerfiles/ci-compose.yml][yml]{
version: '2'
services:
  gitlab_ci:
    build:
      context: ../
      dockerfile: dockerfiles/ci.dockerfile
    command: bash -c "./gradlew --stacktrace clean testDebugUnitTest build"
//}

これでプロジェクトのルートから @<code>{docker-compose -f dockerfiles/ci-compose.yml up --build} と実行すれば、アプリがビルドされます。

ただし、上記のコードはボリュームマウントで行っていないため、コンテナの実行終了（破棄）とともに、ビルドされたAPKも消えてしまいます。
必要に応じて、compose.ymlの最後のcommandに、デプロイサービスへのアップロード処理も実行するようにコマンドを追加してください。

== Gitのリビジョン番号などをAndroidで利用できるようにする

開発中で、しかも業務などで第三者に動作チェックしている段階になると、いざ不具合が見つかったとしても、どのコードで起きたのか特定ができなくなる場合があります。
ですので、あらかじめversionNameだったり画面内のどこかでリポジトリのリビジョン番号を表示しておくことで、問題の確認ができやすくなることができます。

//list[android-set-git-commit][app/build.gradle][gradle]{
// リビジョン番号の取得
def gitSha = 'git rev-parse --short HEAD'.execute([], project.rootDir)
    .text.trim()

// ブランチ名の取得
def gitBranch = (System.getenv("CIRCLE_BRANCH") != null) 
    ? System.getenv("CIRCLE_BRANCH") :
        'git rev-parse --abbrev-ref HEAD'.execute([], project.rootDir)
            .text.trim()

// 現在時間の取得（ビルド時間として利用）
def buildTime = 
    new Date().format("yyyy-MM-dd'T'HH:mm'Z'", TimeZone.getTimeZone("JST"))

android {
  // ...

  defaultConfig {
    versionName "${versionMajor}.${versionMinor}.${versionPatch}-${gitSha}"

    // BuildConfig.XXX としてコードで利用できるようにする
    buildConfigField "String", "GIT_SHA", "\"${gitSha}\""
    buildConfigField "String", "GIT_BRANCH", "\"${gitBranch}\""
    buildConfigField "String", "BUILD_TIME", "\"${buildTime}\""

    // ...
  }
}
//}

当然ですが、実行するCIでのコードの取得方法が @<code>{git clone} ではなくエクスポート形式の取得となる場合、これらの変数は正しく表示されません。
注意してください。

=== versionNameの考え方について

いまのところ大きな不便もないため、筆者が作るプロダクトのバージョン表記は @<code>{(major).(minor).(patch)-(rev)} と設定しています。

 * メジャー番号 … 大規模なデザインや根幹部分の機能変更
 * マイナー番号 … 機能（画面）の一部がハッキリわかる形で変更、ユーザーにわかるレベルでの機能追加
 * パッチ番号　…　バグフィックス、小さなデザインの変更

これに合わせ、先程のリポジトリのリビジョン番号を付与しています。

ユーザーにとってversionNameというものは、現在インストールしているアプリが最新版であるかどうかを視覚的に判断するためのものでしかないものと認識しています。
これを読んでいる方で、いまご自身が端末に入れているアプリのversionNameがどう表記されているかをすべて把握している方はほぼいないのではないでしょうか。

ごくたまにversionNameの見え方を気にする方と仕事をしたことがありましたが、こんなことを気にするぐらいなら、もっとユーザーが目にする部分の見え方を注力すべきです。

== TimberとCrashlyticsを駆使してログ管理を便利にする（Kotlin版）

Timberという、かのJake Wharton氏が作成したログ出力用のライブラリがあります。
これを利用することで、リリース版では必要なレベルのログ以外を非表示にできるため、作業中にLog.*を作っては削除という不毛な作業を減らすことができます。

このライブラリが発表された当時はまだjava全盛期だったためjavaコードでの紹介ばかりでした。
とはいえ、kotlinでも実装はほとんど変わりありません。

さらにTreeクラス内にCrashlyticsのログ出力処理を追加することで、特定のレベル以上ではCrashlyticsにログを残すこともできます。

//list[android-timber][app/build.gradle][gradle]{
class CustomApplication : Application() {
    override fun onCreate() {
        super.onCreate()

        // Debug版以外ではカスタムTreeによる出力を行う
        if (BuildConfig.DEBUG) {
            Timber.plant(Timber.DebugTree())
        } else {
            Timber.plant(ReleaseTree())
        }
    }

    // Timber.i以上のものをCrashlyticsに送信する
    private class ReleaseTree : Timber.Tree() {
        override fun log(priority: Int, tag: String?, message: String, 
                t: Throwable?) {
            if (priority == Log.VERBOSE || priority == Log.DEBUG) {
                return
            }
            if (priority != Log.ERROR) {
                Crashlytics.log(message)
                return
            }

            var throwable = t
            if (throwable == null || !TextUtils.isEmpty(message)) {
                throwable = Exception(message)
            }
            Crashlytics.logException(throwable)
        }
    }
}
//}

上記のコードでは、開発版以外では同じツリーを使うようにしていますが、たとえばこのクラスのみ環境ごとにコードを分けて、staging版のみSlackのAPIを直接叩いて通知する、なんてこともできるかと思います。