//=====================================================
// [imGuiSceneGizmo.cpp]
// �쐬:2022/11/07
// �X�V:2022/11/13 �e�q�֌W�Ɍ����ďC��
// 
// ------------------------
// ImGui��Gizmo���g�p
// �X�e�[�W�쐬�p�̃I�u�W�F�N�g�̐ݒu�ɗ~��������
// �ʃv���W�F�N�g�Ŏ����Ă���ڐA
// RectTransform�͔Y�ݒ�
//=====================================================

//--- �C���N���[�h��
#include <ImGui/imgui.h>
#include <DebugSystem/imguiManager.h>
#include <DebugSystem/imGuiSceneGizmo.h>
#include <GameSystem/Component/Camera/camera.h>

using namespace MySpace::Debug;
using namespace MySpace::MyMath;
using namespace MySpace::Game;

void CMyGizmo::EditTransform(const CCamera& camera, CTransform* editTransform)
{
	Matrix4x4 matrix = editTransform->GetWorldMatrix();
	//ImGuizmo::SetDrawlist();
	ImGuizmo::AllowAxisFlip(false);
	
	if (GetAsyncKeyState('W'))
		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (GetAsyncKeyState('E'))
		m_CurrentGizmoOperation = ImGuizmo::ROTATE;
	if (GetAsyncKeyState('R')) // r Key
		m_CurrentGizmoOperation = ImGuizmo::SCALE;

	//--- ��ԕύX
	if (ImGui::RadioButton(u8"Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
		m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton(u8"Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
		m_CurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton(u8"Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
	{
		m_CurrentGizmoOperation = ImGuizmo::SCALE;
		m_CurrentGizmoMode = ImGuizmo::LOCAL;
	}

	XMFLOAT4X4 oldMatrix = matrix;
	XMFLOAT3 matrixTranslation, matrixRotation, matrixScale;
	// �}�g���b�N�X������𓾂�
	ImGuizmo::DecomposeMatrixToComponents((float*)oldMatrix.m, (float*)&matrixTranslation, (float*)&matrixRotation, (float*)&matrixScale);
	// slider + input
	ImGui::DragFloat3("Tr", (float*)&matrixTranslation);
	ImGui::DragFloat3("Rt", (float*)&matrixRotation);
	ImGui::DragFloat3("Sc", (float*)&matrixScale);
	ImGuizmo::RecomposeMatrixFromComponents((float*)&matrixTranslation, (float*)&matrixRotation, (float*)&matrixScale, (float*)oldMatrix.m);

	//--- ۰�فEܰ���
	if (m_CurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", m_CurrentGizmoMode == ImGuizmo::LOCAL))
			m_CurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", m_CurrentGizmoMode == ImGuizmo::WORLD))
			m_CurrentGizmoMode = ImGuizmo::WORLD;
	}
	if (ImGui::IsKeyPressed(83) || ImGui::IsKeyPressed(ImGuiKey_::ImGuiKey_Enter) || GetAsyncKeyState(VK_LSHIFT))
		m_bUseSnap = !m_bUseSnap;
	ImGui::Checkbox("Snap", &m_bUseSnap);
	ImGui::SameLine();
	switch (m_CurrentGizmoOperation)
	{
	case ImGuizmo::TRANSLATE:
	//snap = config.mSnapTranslation;
	ImGui::InputFloat3("Snap", &m_vSnapMove.x);
	break;
	case ImGuizmo::ROTATE:
	//snap = config.mSnapRotation;
	ImGui::InputFloat("Angle Snap", &m_vSnapMove.x);
	break;
	case ImGuizmo::SCALE:
	//snap = config.mSnapScale;
	ImGui::InputFloat("Scale Snap", &m_vSnapMove.x);
	break;
	}

	ImGuiIO& io = ImGui::GetIO();
	//--- �r���[�|�[�g
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	//--- �M�Y��
	XMFLOAT4X4 worldMatrix = oldMatrix;
	ImGuizmo::Manipulate(
		(float*)CCamera::GetMain()->GetViewMatrix().m, (float*)CCamera::GetMain()->GetProjMatrix().m, 
		m_CurrentGizmoOperation, 
		m_CurrentGizmoMode, 
		(float*)worldMatrix.m, 
		NULL, m_bUseSnap ? (float*)&m_vSnapMove : NULL,
		m_bBoundSizing ? m_aBoundsSnapMove : NULL);

	XMFLOAT3 newTrans, newRot, newScal, oldTrans, oldRot, oldScal;
	ImGuizmo::DecomposeMatrixToComponents((float*)&worldMatrix.m, (float*)&newTrans, (float*)&newRot, (float*)&newScal);
	ImGuizmo::DecomposeMatrixToComponents((float*)&oldMatrix.m, (float*)&oldTrans, (float*)&oldRot, (float*)&oldScal);

	XMFLOAT4X4 local = worldMatrix;
	// �e������
	if (auto obj = editTransform->GetParent();obj.lock())
	{
		// ���[���h���W�̋t�s��(lcal)
		XMStoreFloat4x4(&local, XMMatrixMultiply(
			XMLoadFloat4x4(&local),
			XMMatrixInverse(nullptr, XMLoadFloat4x4(&obj.lock()->GetWorldMatrix()))
		));
	}
	XMFLOAT3 localTrans, localRot, localScal;
	ImGuizmo::DecomposeMatrixToComponents((float*)local.m, (float*)&localTrans, (float*)&localRot, (float*)&localScal);

	// �s��ݒ�
	//editTransform->SetWorldMatrix(matrix);
	editTransform->SetWorldMatrix(localTrans, localRot, localScal);

	// �I����Ԑݒ�
	if (ImGuizmo::IsUsing())
	{
		ImGuiManager::Get().UpHover(ImGuiManager::EIsHovered::HOVERED_GIZMO);
	}
	else
		ImGuiManager::Get().DownHover(ImGuiManager::EIsHovered::HOVERED_GIZMO);

	//--- ��ׂ̎p��
	auto viewMatrix = CCamera::GetMain()->GetViewMatrix().m;
	//auto view = CCamera::GetMain()->GetViewMatrix();
	//if (CScreen::ScreenJudg(Vector3(view._41, view._42, view._43)))
	{
		ImGuizmo::ViewManipulate((float*)&viewMatrix, 8, ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImVec2(128, 128), 0x10101010);
	}

	// 


}