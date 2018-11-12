={what-do-you-think} 独りで開発する、ということ

//lead{
本章では筆者自身のこれまで体験をベースに、プライベートと業務における「独りで開発する」ことに関するそれぞれの状況と、そこからプロダクト開発を進めるにあたって心構えはどうすべきだったかを記載しています。
//}

== はじめに

エンジニア独りで開発をするにあたって、自身のいる状況を冷静に分析することで、その後のプロダクトの進め方、設計において何を気にするべきかの方向が見えてくるかもしれません。

本章ではプライベートと業務（仕事）の場合に分けて、「独りで開発しているとはどういう状況なんだろう」というのを分析し、内容をまとめました。

== プライベートにおける「独り」という状況

前書きでも触れましたが、筆者はteam Y.G.E.というプライベートのチームでアプリやサービスを開発しています。
team Y.G.E.は名称こそチームと呼称していますが、実際のところほかのメンバーは一部のデザインやアイデア出しの手伝いをしてもらっているレベルで、企画から運用までは基本的に筆者ひとりで行っています。
そのせいもあって、なかなかやれる範囲も少ないし、保守でも後手後手に回ることばかりです。

=== なぜプライベートでプロダクトを作っているのか

ではなぜ、そもそもプライベートでプロダクトを作っているのでしょうか。
そこまでして作りたい、やりたいプロダクトなのであれば、たとえば所属している会社に提案すれば、会社としてバックアップがもらえるかもしれません。
いっそ独立して、そのプロダクトで食っていくという選択肢もあります。

そうですね、おそらくこれを読んだあなたは「お前なにを言っているんだ」と思ったことでしょう。
私もそう思います。

結局のところ、独りでプライベートの時間を使ってプロダクトを作っているのはワケがあります。

 * 自分の裁量で自由に作り進めたい（過度な責任を負いたくない）
 * 過度な収益を目標にしたくない
 * 同等の熱量（ほかにも技術面など）を持ってやれる仲間がいない
 ** ここは個人的にイレギュラーな部分なので考えないことにします

といった、要はやりたいことや作りたいものに対して没頭できることが、プライベートにおけるプロダクト開発における魅力のはずです。
ちなみに、ここまで読んで同意する部分がなければ本章は読み飛ばしてください。

//image[wdyt-work-hard][業務でありがちな辛み（かといって、プライベートなら無縁かと言われると…）][scale=1.00]{
//}

=== やりたいこと、作りたいことに没頭しよう

ということで、プライベートでのプロダクトは誰かに強制されたものではなく、あなた自身の「やりたい！」という気持ちで始まっているはずです。
そうでないなら、そのプロダクトはさっさと閉じてしまいましょう。

さらにプライベートでのプロダクト開発は、言ってしまえば業務でやるほどの収益的な「旨味」はありません。
プライベートと業務では投入できるリソースがまるで違うわけですから、当然とも言えます。
逆に業務と違ってあなたにすべての裁量があるため、プロダクトをいつまでも続けることも、突然終了することもできます。
これは大きなメリットでもあるし、デメリットであるとも言えます。

ここは完全に筆者の個人的な意見になりますが、これを踏まえるとプライベートでプロダクトを作って運用し続けるためには、モチベーションの維持がとても重要ではないかと感じます。
そういった意味で、タイトルにある「やりたいこと、作りたいことに没頭」できる環境を整えることが、プライベートにおけるプロダクト開発の重要な要素のように感じます。

=== モチベーションを下げる要因を排除していこう

モチベーションを高く維持し続けることは、とても難しいです。
気分というものは、まったく関係のない外部要因でも左右されがちです。
ただ、「確実にモチベーションが下がる要因」を排除することで、結果的にモチベーションを維持し続けることができるかもしれません。

たとえば前の話に戻すと「やりたいこと、作りたいことに没頭」するためには、逆にやりたくないこと、作りたくないものはやらずに済むようにするだけでもモチベーションの維持につながってきます。
たとえば筆者は不定期でやる必要があるけど、毎回手順を忘れがちなデプロイ作業や、定期的に実施する事務的な運用作業がとても苦痛です。
こういった作業を自動化させることで、好きな開発そのものに没頭できる時間が増えます。
こうした地道な排除活動が、プライベートにおけるプロダクト開発ではとても大事なことではないでしょうか。

== 業務（企業）における「独り」という状況

業務での「このプロジェクトのエンジニアは私一人です」という状況は、非常に良いものではありません。
なぜなら多くの場合、「ワケあって一人」というネガティブな理由だからです。

=== プライベートで無視できた要素すべてが無視できなくなる

たいていのプロダクトの企画を考える人は、開発の難易度を把握することに関しては素人です。
あなたの目の前には、これひとりで作るにはちょっと…という内容の企画書や仕様書が渡されているかもしれません。
最悪の場合、口頭でひたすら話されているかもしれません。
ほとんど法則のようなものですが、業務におけるプロダクトはスムーズにことが運びません。
ひとりのプロダクトと違って、いろんな人や集団（チームだったり、部署だったり、企業だったり）の思惑が挟まるからです。
そして、一番初めのこの企画書（仕様書）のやりとりがスムーズでなかったものは、さらにその後の物事がスムーズに運びません。

先ほどのプライベートの開発では避けられた要因が、今度は避けられないものとしてあなたに襲いかかります。

//image[wdyt-work-hard-require][業務ではこれらと向き合わなければいけません][scale=1.00]{
//}

=== 業務ではコーディングに没頭し続けることは不可能

プライベートにおけるプロダクト開発では、少なくとも初回リリースまでのすべての作業担当は自身だけでした。
ですが業務に関しては、あくまでも「チーム内における特定範囲（もしくはすべて）のエンジニアが独り」という状況です。
そのため、仕様の認識合わせに限らず、多くのステップにおいてチーム内とコミュニケーションによるやりとりが発生します。
こういった部分は、チーム開発として最適化されたり効率化されることもありますが、ゼロにすることはできません。
つまり、常にプライベートの時のような「完全没頭」を作り出すことはできません。

=== 限りある時間を効率よく進める

プライベートでの開発とは状況が異なりますが、「完全没頭」こそできないものの、きちんと没頭できる時間を作り出すことが重要になります。
特に人によるイレギュラーが発生してしまうことはある程度許容したうえで、少なくとも自身の開発の上で分かりきっている自動化できるような作業を先に自動化させておくことで、少しでも自身の開発に時間を割けられる状況を生み出せるように努めることが、業務において重要なところでもあるかと思います。

=== 第三者に引き渡しやすい状況を作っておく

業務のメリットとして、（上の判断があれば）新たなメンバーが加わる可能性があるところです。
それはエンジニアではないかもしれませんが、いくつかの作業を簡単にできる形にしておくことで、作業の一部を引き渡しやすくなります。

業務で自分ひとりが開発していると、どうしても運用面も自分ひとりという認識にいきがちです。
自分がいま手を動かした方が早いから、という理由で第三者から見れば小難しいマニュアルの必要なルーチンワークを作ってしまいます。
こうなるといざというときに作業分担がしづらく、状況次第では分担できる作業づくりのために、さらなる余計な時間をとられてしまうことになってしまいます。

===[column] あなたが技術監督であることを忘れないように

結局のところ、業務でてんやわんやになって品質の悪いプロダクトを作ってしまう最大の原因は、エンジニアの技術力とそれに見合う工数、期間、品質管理をなあなあのまま進めてしまっていることにあります。
極端な話、1日で十分完成するレベルのプロダクトを１週間分の期間でもって開発すれば、十分品質の向上に時間をかけられるわけです。

じゃあなぜこんなてんやわんやになっているのでしょうか。
それは見合っていない仕様の数や難易度、工数、期間が不十分なままOKを通した人たちがいるからです。
そして、エンジニアがあなた独りという状況であれば、あなたもそのOKを通した一人であるはずです。
あなたが自分の力を正しく発揮して、あなたの優れたプロダクトを世に披露するためにも、コーディング以外の面でもプロダクトの品質を守るために動きましょう。

それがお金をもらって仕事をする人の義務だと私は思います。

== 結局、やることは同じ

結局のところ、目的は違えど共通して言えることは「やるべき作業に集中して取り組める状況を作り出す」ということが重要でした。

筆者は、こうした「やるべき作業」以外をどんどんと自動化させたり、外部のサービスを活用する形で実現できるように、プロダクト全体の設計を行うようにしています。具体的なプロダクトの設計については、次の章で解説します。
