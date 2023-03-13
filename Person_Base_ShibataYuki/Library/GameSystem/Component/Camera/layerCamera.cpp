//==========================================================
// [layerCamera.cpp]
//---------------------------------------------------------
// �쐬:2023/02/14 
//---------------------------------------------------------
// ���C���[�}�X�N�������������
//==========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Camera/layerCamera.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::Game;

//==========================================================
// �R���X�g���N�^
//==========================================================
CLayerCamera::CLayerCamera()
{
	// �����l�Ƃ��āA�S�Ẵr�b�g��1�ɂ���
	m_nLayerMask = (1u << 31) - 1;
}

//==========================================================
// �����t���R���X�g���N�^
//==========================================================
CLayerCamera::CLayerCamera(std::shared_ptr<CGameObject> owner)
	:CCamera(owner), m_nLayerMask(0)
{
	// �����l�Ƃ��āA�S�Ẵr�b�g��1�ɂ���
	m_nLayerMask = (1u << 31) - 1;
}

CLayerCamera::~CLayerCamera()
{
}

//==========================================================
// �}�X�N�Ώۂ��m�F
//==========================================================
bool CLayerCamera::IsMask(const int layer)const
{
	return m_nLayerMask & (1 << layer);
}

//==========================================================
// �}�X�N�ݒ�
//==========================================================
void CLayerCamera::SetMask(const int mask)
{
	m_nLayerMask = mask;
}

//==========================================================
// �}�X�N�ݒ�ǉ�
//==========================================================
void CLayerCamera::AddMask(const int layerNo)
{
	m_nLayerMask |= 1 << layerNo;
}

//==========================================================
// �}�X�N�ݒ����
//==========================================================
void CLayerCamera::ReleaseMask(const int layerNo)
{
	// �r�b�g��0�ɂ���
	// ~ �r�b�g���]
	m_nLayerMask &= ~(1 << layerNo);
}

#ifdef BUILD_MODE

//==========================================================
// �\��
//==========================================================
void CLayerCamera::ImGuiDebug()
{

	// LayerMask�ݒ�
	m_nLayerMask = CLayer::ImGuiSetLayerList(m_nLayerMask);

	ImGui::Separator();

	CCamera::ImGuiDebug();
}

#endif // BUILD_MODE
