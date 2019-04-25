就職活動用の軽量ゲームエンジン

　低機能なゲームエンジンです、
　DirectXⅡの教材.docxのChapter8までの内容が実装されたものとなります。
　このエンジンをベースに開発を行っても構いませんし、
　このエンジンを参考にして、新しいエンジンを再構築してもらっても構いません。
　DirectXⅢで学習する、グラフィックテクニック、サウンドエンジン、Effekseerは
　自身の力で組み込んでいってください。
　下記に４点注意すべき点を記載しています。読んでおいてください。

①VisualStudioに追加する必要があるアセットは下記の二つだけです。
　　・fbxファイル
　　・2D表示に使いたい画像データ。(ImageContentsPipelineを指定する必要あり。)
　
②tkEngineからの廃止項目
　　・カスタムビルドツールの指定。
　　　　→ヒューマンエラーのもとだったため、Assetsフォルダに追加するだけに仕様変更。
　　　　　→いらないファイルがAssetsフォルダに入るが、そっちの方がマシだと判断。
　　　　　　→これに伴い、VisualStudioに追加するファイルは上記の二つだけとなった。
　　　　　
③toolsの中に新しいtkExporterあり。
　→右クリックで管理者として実行してね.batを実行したらコピーされる。
　
④グローバルなインスタンスは下記の４つ
　GraphicsEngineのg_graphicsEngine
　Padのg_pad
　PhysicsWorldのg_physics
　Cameraのg_camera3D


　
