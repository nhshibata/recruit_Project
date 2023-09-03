//==========================================================
// [lightCollision.cpp]
//----------------------------------------------------------
// �쐬:2023/05/19 
// 
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/DarkGame/lightCollision.h>
#include <GameSystem/DarkGame/lightCollisionManager.h>
#include <GameSystem/Manager/lightManager.h>

using namespace DarkGame;

//==========================================================
// �R���X�g���N�^
//==========================================================
CLightCollision::CLightCollision()
{

}

//==========================================================
// �R���X�g���N�^
//==========================================================
CLightCollision::CLightCollision(CGameObject::Ptr ptr)
	:CComponent(ptr)
{

}

//==========================================================
// �f�X�g���N�^
//==========================================================
CLightCollision::~CLightCollision()
{

}

//==========================================================
// �ǂݍ��ݏ���
//==========================================================
void CLightCollision::OnLoad()
{
	// �ǉ�
	m_pLight = GetComponent<CPointLight>()->BaseToDerived<CPointLight>();
}

//==========================================================
// ����������
//==========================================================
void CLightCollision::Awake()
{
	// �ǉ�
	m_pLight = AddComponent<CPointLight>();
}

//==========================================================
// ����������
//==========================================================
void CLightCollision::Init()
{
	// �Ǘ��׽�ւ̈˗�
	auto obj = CGameObject::FindGameObjectWithTag(CLightCollisionManager::TAG);
	auto mgr = obj.lock()->GetComponent<CLightCollisionManager>();
	mgr.lock()->AddList(this);
}

//==========================================================
// �X�V
//==========================================================
void CLightCollision::Update()
{

}

//==========================================================
// �m�F
// �͈͓��̃��C�g�̃^�O��z��ŕԂ�
//==========================================================
bool CLightCollision::Check(const CLightCollision& other)
{
	// ���g�̏��
	const Vector3 Apos = this->Transform()->GetPos();
	/*const float Ar = m_pLight.lock()->GetRange();

	auto light = other.GetLightPtr();
	const Vector3 Bpos = light->Transform()->GetPos();
	const float Br = light->GetRange();*/

	const float Ar = 0;
	const Vector3 Bpos;
	const float Br = 0;

	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	// �͈͊m�F
	if (dx * dx + dy * dy + dz * dz <= dr * dr)
	{
		return true;
	}

	return false;
}