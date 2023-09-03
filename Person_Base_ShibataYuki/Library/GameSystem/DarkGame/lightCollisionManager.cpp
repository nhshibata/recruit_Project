//==========================================================
// [lightCollisionManager.h]
//----------------------------------------------------------
// �쐬:2023/06/01
//----------------------------------------------------------
// ���C�g�͈̔͊m�F�N���X���Ǘ�����
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/DarkGame/lightCollisionManager.h>
#include <GameSystem/DarkGame/lightCollision.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CLightCollisionManager::CLightCollisionManager()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CLightCollisionManager::CLightCollisionManager(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CLightCollisionManager::~CLightCollisionManager()
{
	m_aLights.clear();
}

//==========================================================
// ����������
//==========================================================
void CLightCollisionManager::Awake()
{
	CTag::CreateTag(this->TAG);
	GetOwner()->SetObjTag(this->TAG);
}

//==========================================================
// ������
//==========================================================
void CLightCollisionManager::Init()
{

}

//==========================================================
// �X�V
//==========================================================
void CLightCollisionManager::Update()
{

	for (auto it = m_aLights.begin(); it != m_aLights.end(); ++it)
	{
		// �擾
		const auto& a = (*it);
		// ���t���b�V������
		a->Refresh();

		for (auto itB = m_aLights.begin(); itB != m_aLights.end(); ++itB)
		{
			if (it == itB)
				continue;

			const auto& b = *(*itB);
			// �͈͊m�F
			a->Check(b);
		}
	}


}
