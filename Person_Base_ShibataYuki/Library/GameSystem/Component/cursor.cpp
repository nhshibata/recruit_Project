//=========================================================
// �쐬:2022/04/21
// �J�[�\��: ���i�׽
// unity�ŃJ�[�\�����Ǘ����Ă�����@�Ɠ���
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/cursor.h>
#include <GameSystem/GameObject/gameObject.h>
#include <CoreSystem/Input/input.h>

using namespace MySpace::Game;

// �R���X�g���N�^
CCursor::CCursor()
	:m_bInput(false),m_nMax(1),m_nPoint(0),m_nOldPoint(0), m_bAuthority(false)
{
}
// �����t���R���X�g���N�^
CCursor::CCursor(std::shared_ptr<CGameObject> owner)
	:CComponent(owner), m_bInput(false), m_nMax(1), m_nPoint(0), m_nOldPoint(0), m_bAuthority(false)
{
}
// �f�X�g���N�^
CCursor::~CCursor()
{

}
// ������
void CCursor::Init()
{

}
// �X�V
void CCursor::Update()
{
	if (m_nPoint != m_nOldPoint)
	{
		m_nOldPoint = m_nPoint;
	}
}