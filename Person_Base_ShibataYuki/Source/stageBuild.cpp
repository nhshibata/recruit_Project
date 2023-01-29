//========================================================
// [stageBuild.cpp]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

//--- インクルード部
#include <stageBuild.h>
#include <spellDefine.h>

#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Collision/boxCollision.h>

using namespace MySpace::Game;
using namespace Spell;

//========================================================
// コンストラクタ
//========================================================
CStageBuild::CStageBuild(MySpace::MyMath::Vector3 startPos, float marzin)
	:m_vStartPos(startPos), m_fMarzin(marzin)
{

}

//========================================================
// デストラクタ
//========================================================
CStageBuild::~CStageBuild()
{

}

//========================================================
// 構築
//========================================================
bool CStageBuild::Build(CResourceCSV* csv)
{
	auto aData = csv->GetintArray();
	CGameObject::PtrWeak obj;
	const char* aModelName[] = {
		MODEL_PATH2(ToyStage/WorkBlock.fbx),
		MODEL_PATH2(ToyStage/WorkOctagon.fbx),
		MODEL_PATH2(ToyStage/MovingBlock.fbx),
	};
	int data = 0;

	// 縦
	for (int column = 0; column < aData.size(); ++column)
	{
		// 横
		for (int row = 0; row < aData[column].size(); ++row)
		{
			//--- ステージ番号取得
			data = aData[column][row];
			// 設置対象確認
			if (data == static_cast<int>(EStageNo::NONE))
				continue;

			//--- オブジェクト生成
			obj = CGameObject::CreateObject();

			// 仮の名前設定
			obj.lock()->SetName("Cell_" + std::to_string(column) + std::to_string(row));

			// tag設定
			obj.lock()->SetObjTag(Spell::TAG_LAND);

			// 位置決定
			Vector3 pos = m_vStartPos;
			pos.x = m_vStartPos.x * row * m_fMarzin;
			pos.z = m_vStartPos.z * column * m_fMarzin;
			obj.lock()->GetTransform()->SetPos(pos);

			//--- モデル設定
			auto model = obj.lock()->AddComponent<CModelRenderer>();
			model->SetModel(aModelName[data]);
			model->SetStatic(CMeshRenderer::EStaticMode::STATIC);

			//--- 当たり判定追加
			auto box = obj.lock()->AddComponent<CBoxCollision>();
			// ﾄﾘｶﾞｰ設定
			box->SetTrigger(false);

		}

	}

	return true;
}

