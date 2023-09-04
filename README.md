# recruit_Project

実行環境確認用ブランチ作成
23_05


【動画】
https://youtu.be/J2kjDm3Vgf4

【概要】
DirectXを使用して作成したゲーム制作用のツールです。
HALに入学してから学んだことを実装し、ゲームを作る環境を整えたいと思い制作しました。一部（3Dモデル読み込み等）を除いて一から設計し、記述しました。
ゲーム制作を続けていく中思った「欲しい」「実装したい」機能を実装しています。
※コンポーネント指向で記述。C++17準拠でコードを書いています。

【説明】
ImGuiを使うことで、Unityを参考とした視覚的な操作を再現することができました。また、ImGuizmoを使うことで、GameObjectの移動、回転、拡縮を実装しました。
inspectorでは、GameObjectの状態変更、ｺﾝﾎﾟｰﾈﾝﾄの確認、変更、追加が可能。
hierarchyでは、GameObjectの生成や選択を行え、drag＆dropで親子関係を構築出来ます。新しいSceneの作成や、作成したSceneをjsonファイルとして出力、また、読み込みが可能です。
視錘台カリングによる描画スキップやインスタンシング描画など描画関係の状態確認。
FPSの確認、TimeScaleの変更をはじめとしたUpdateとFixedUpdateを再現するためのFPS操作。
behavior treeを参考にしたAIComponentの実装。および、AIに必要な情報として、経路探索に使えるWayPoint作成を行えます。
Unityを参考にしたOnCollision,OnTriggerの実装。

【操作方法】
マウスの右でｶﾒﾗ座標移動
マウスの中央で注視点移動
マウスの左でｶﾒﾗ回転

・Hierarchy
	ゲームオブジェクトの追加とSceneの作成、保存、読み込みができます。
	hierarchy上のGameObjectのButtonを押すと、inspectorが表示されます。
	hierarchy上のGameObjectのButton上で右クリックでメニュー表示。
	メニューのSceneを選ぶと、Sceneの保存、読み込みなどが行えます(デモ用のSceneを複数用意しています)。

・Inspector
	ゲームオブジェクトのパラメータやコンポーネント操作ができます。
	ウィンドウ上部のメニューでコンポーネントの追加、オブジェクトのコピー削除ができます。

・ステータス(左下のWindow)
	ゲーム内の情報を確認できます。
	FPSや描画情報を確認できます。
	
・右下Window
	はフレームの停止や1フレーム進行などができます。

