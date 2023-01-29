//========================================================
// [stageBuild.cpp]
//------------------------
// �쐬:2023/01/02
//------------------------
//
//========================================================

//--- �C���N���[�h��
#include <stageBuild.h>
#include <spellDefine.h>

#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Collision/boxCollision.h>

using namespace MySpace::Game;
using namespace Spell;

//========================================================
// �R���X�g���N�^
//========================================================
CStageBuild::CStageBuild(MySpace::MyMath::Vector3 startPos, float marzin)
	:m_vStartPos(startPos), m_fMarzin(marzin)
{

}

//========================================================
// �f�X�g���N�^
//========================================================
CStageBuild::~CStageBuild()
{

}

//========================================================
// �\�z
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

	// �c
	for (int column = 0; column < aData.size(); ++column)
	{
		// ��
		for (int row = 0; row < aData[column].size(); ++row)
		{
			//--- �X�e�[�W�ԍ��擾
			data = aData[column][row];
			// �ݒu�Ώۊm�F
			if (data == static_cast<int>(EStageNo::NONE))
				continue;

			//--- �I�u�W�F�N�g����
			obj = CGameObject::CreateObject();

			// ���̖��O�ݒ�
			obj.lock()->SetName("Cell_" + std::to_string(column) + std::to_string(row));

			// tag�ݒ�
			obj.lock()->SetObjTag(Spell::TAG_LAND);

			// �ʒu����
			Vector3 pos = m_vStartPos;
			pos.x = m_vStartPos.x * row * m_fMarzin;
			pos.z = m_vStartPos.z * column * m_fMarzin;
			obj.lock()->GetTransform()->SetPos(pos);

			//--- ���f���ݒ�
			auto model = obj.lock()->AddComponent<CModelRenderer>();
			model->SetModel(aModelName[data]);
			model->SetStatic(CMeshRenderer::EStaticMode::STATIC);

			//--- �����蔻��ǉ�
			auto box = obj.lock()->AddComponent<CBoxCollision>();
			// �ضް�ݒ�
			box->SetTrigger(false);

		}

	}

	return true;
}

