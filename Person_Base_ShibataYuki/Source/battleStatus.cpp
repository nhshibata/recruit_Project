//=========================================================
// [battleStatus.cpp]
// �쐬:2022/09/21
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h��
#include <battleStatus.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>

using namespace Spell;

void CBattleStatus::Awake()
{
	// TODO: 2D���A3D���A�G�t�F�N�g�ŕ\�����邩����
	//CGameObject::Ptr obj;
	//CPolygonRenderer* render;
	//// �`��ǉ�
	//for (int cnt = 0; cnt < 10; ++cnt)
	//{
	//	obj = CGameObject::CreateObject().lock();
	//	render = obj->AddComponent<CPolygonRenderer>().get();
	//	render->GetRectTransform()->SetPos();
	//}
	//m_pLifeRender;

	// �W��
	this->Life.accessor = 10;
	this->Atk.accessor = 1;
	this->Damage.accessor = 0;
}
void CBattleStatus::Init()
{
}
void CBattleStatus::Update()
{
	if (this->Damage.accessor == 0)	// �_���[�W���Ȃ��Ȃ甲����
		return;
	if (CFps::Get().GetTime() % 4 == 0)	// �Ԋu���󂯂�
		return;

	int add = 0;
	add = (0 - this->Damage.accessor) / 10;
	this->Life.accessor += add;
}
void CBattleStatus::FixedUpdate()
{

}
