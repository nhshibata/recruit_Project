//=========================================================
// [startUp.cpp]
//---------------------------------------------------------
// 作成:2022/04/19 (火曜日)
// 更新:2022/04/19 コンポーネント、ゲームオブジェクト、シーン。unityを参考にtagとlayerを定義。
// 更新:2022/04/20 cppの実装。collision、transform、cursor、fps作成
// 更新:2022/04/21 Factoryｸﾗｽ、playerｸﾗｽの実装
// 			!課題! オブジェクトのトランスフォームの決め方(座標、角度、大きさ)や名前、細かいコンポーネントの追加をどう実装するか
// 		描画はするが、その他の更新をしたくないオブジェクトはどうするか?スロー時の各更新はどう操作するか?
// 更新:2022/04/22 少し修正。
// 更新:2022/04/24 当たり判定更新。
//			!課題! UI関連の描画順の決め方。現在のComponentではなく、派生ｸﾗｽDrawComponentを定義し、描画順を決める変数を設ける。
// 更新:2022/06/03 追加はしていたものの、記録は残していない。あともう少しかな?
//			構想: UIはやはりUI側から描画依頼を申請する方法を検討。ポインタなどを渡しレイヤー順で並びかえ。
// 更新:2022/06/07 タグに悩み中。列挙体ではなく文字列として使う場合、どう扱うべきか?
//			ポインタをスマートポインタに変更。所有するポインタと弱参照のポインタをどう扱うべきか?
//			基底クラスを複数作成
// 更新:2022/06/08 大体スマートポインタに変更。ﾃﾝﾌﾟﾚｰﾄｸﾗｽによりなやまされる（エラーがわかりにくい（英語?
//			↑解決。std::list<std::weak_ptr<*>>とstd::findの組み合わせが悪かったよう。実ポインタの比較で解決
// 更新:2022/06/09 中途半端だったOnCollision終了。
// 更新:2022/06/10 
// 更新:2022/06/20 ウィンドウ生成 
// 更新:2022/08/20 存在を思い出す。なんか色々追加
//				プロジェクトを複数に分割。
// 更新:2022/08/23 課題制作開始。作品を制作して出たエラーや不便さ対策
// 更新:2022/08/25 Unityのシーン遷移時の呼び出し関数(デリゲート?)実装
//				std::funcction,std::bindを用いた関数ポインタおよび、可変長ﾃﾝﾌﾟﾚｰﾄ引き数を使って再現
// 更新:2022/08/28～8/31 UEぷちコン制作作業で休み
// 更新:2022/09/01 制作再開。ムカデの分割表示
// 更新:2022/11/01 インターンで停止していたので再開。
// 更新:2022/12/07 描画システムや当たり判定システム改善したり
//					NavMeshとAISystem(Behavior)を移植
// 更新:2022/12/08 昨日急に発生したC4743（cpp毎のバイトサイズ違い）警告を解決
//					仮想関数の宣言と実装を同時に行っていたのが原因?らしい。cppとhでわけることで解決
// 
//
// 内容:コンポーネント指向をベースとしたプログラム。
// これを元に個人制作のゲームを制作する。
// 
//=========================================================

#define NOMINMAX

//--- インクルード部
#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>

#include <stdio.h>
#include <memory>
#include <time.h>
#include <tchar.h>

#include <Application/main.h>
#include <GameSystem/Manager/sceneManager.h>
//#include <gameCentipedeMarch.h>

// ライブラリ参照
// プロパティで指定するか、ここで指定するか…悩みどころ
#pragma region LIBRARY

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex")
#else
#pragma comment(lib, "DirectXTex")
#endif // _DEBUG

#pragma comment(lib, "winmm")
#pragma comment(lib, "imm32")

#pragma comment(lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")	

//#pragma comment(lib, "Application")
//#pragma comment(lib, "CoreSystem")
//#pragma comment(lib, "DebugSystem")
//#pragma comment(lib, "GameSystem")
//#pragma comment(lib, "GraphicsSystem")
#pragma endregion

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	using namespace MySpace::SceneManager;

	// *@シーン作成時に呼び出すｸﾗｽ
	// *@静的なシーン作成の場合、動的な作成はファイル書き込みと読み込みを行う
	// *@通常の関数でもいいような
	class CMyScene
	{
	public:
		CMyScene()
		{
		}
		void* Load(CScene* scene, int mode)
		{
#if 0
			//auto obj = scene->GetObjManager()->CreateGameObject();
			//obj->AddComponent<CGameCentipedeMarch>();
#endif // 0
			scene->SetSceneName("BuildeScene");
			scene->CreateEmptyScene();
			return nullptr;
		}
	};

	CMyScene my;
	// シーン読み込み時呼び出す関数を設定
	CSceneManager::Get().SceneLoaded<CMyScene>(&CMyScene::Load, &my);
	
	// 開始
	// ウィンドウの生成などを行う
	StartUp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	// ゲームループ
	MainLoop();

	// 終了
	ShutDown();

	return 0;
}