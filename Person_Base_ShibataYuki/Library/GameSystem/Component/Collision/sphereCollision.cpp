//=========================================================
//�쐬:2022/05/02
// �R���W�����׽(�����蔻�� ) : �h���׽
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>

#ifdef BUILD_MODE

#include <ImGui/imgui.h>
#include <GraphicsSystem/Render/Sphere.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Renderer/sphereRenderer.h>

#endif // BUILD_MODE

using namespace MySpace::Game;

//==========================================================
// �������t���R���X�g���N�^
//==========================================================
CSphereCollision::CSphereCollision(std::shared_ptr<CGameObject> owner, float radius)
	:CCollision(owner),m_fRadius(radius)
{
#if BUILD_MODE
	m_pDebugSphere = std::make_shared<CSphere>();
	m_pDebugSphere->Init(16, 8, radius);
	m_pDebugSphere->SetDiffuse(Vector4(0, 1, 0, 0.5f));
#endif //
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CSphereCollision::~CSphereCollision()
{
#if BUILD_MODE
	m_pDebugSphere->Fin();
	m_pDebugSphere.reset();
#endif // BUILD_MODE
}

//==========================================================
// ���̓����蔻��
//==========================================================
bool CSphereCollision::Sphere(Vector3 Apos, float Ar, Vector3 Bpos, float Br)
{
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

//==========================================================
// ���̓����蔻��
//==========================================================
bool CSphereCollision::CollisionSphere(Vector3 pos, float radius)
{
	if (Sphere(Transform()->GetPos(), m_fRadius, pos, radius))
	{
		return true;
	}
	return false;
}

//==========================================================
// �����蔻��
// �R���W�����׽���������ɂƂ��āA�����蔻����s��
//==========================================================
bool CSphereCollision::HitCheckPtr(CCollision* other)
{
	bool trigger = other->IsTrigger() | IsTrigger();
#ifdef _DEBUG
	auto other_name = other->GetOwner()->GetName();
	auto other_flg = other->IsActive();
	auto name = GetOwner()->GetName();
	auto flg = IsActive();
#endif // _DEBUG
	if (m_vOldScale != Transform()->GetScale())
	{
		auto scl = Transform()->GetScale() / m_vOldScale;
		SetRadius(GetRadius()* scl.GetLargeValue());
		m_vOldScale = Transform()->GetScale();
	}

	// ���߰��Ă̊m�F
	if (CSphereCollision* com = other->GetComponent<CSphereCollision>(); com)
	{
		// ���̔���
		if (CollisionSphere(other->Transform()->GetPos(), com->GetRadius()))
		{
			// �����o��
			if (!trigger)
			{
				PushObject(com, com->GetRadius());
				// �����o�����s���Ƒ���͔��肳��Ȃ����߁A�����ōs��
				other->HitResponse(this);
			}
			this->HitResponse(other);
			
			return true;
		}
	}
	else if (auto otherBox = other->GetComponent<CBoxCollision>(); otherBox)
	{
		Vector3 size = { this->GetRadius() ,this->GetRadius() ,this->GetRadius() };
		if (otherBox->Box(Transform()->GetPos(), size, other->Transform()->GetPos(), otherBox->GetSize()))
		{
			// �����o��
			if (!trigger)
			{				
				PushObject(other, otherBox->GetSize().GetLargeValue());
				// �����o�����s���Ƒ���͔��肳��Ȃ����߁A�����ōs��
				other->HitResponse(this);
			}
			this->HitResponse(other);

			return true;
		}
	}

	return false;
}

//==========================================================
// �����o��
//==========================================================
void CSphereCollision::PushObject(CCollision* other, float radius)
{
	//---  �����o��
	// ��_�ԂƂQ���a�̍�
	Vector3 distance = Transform()->GetPos() - other->Transform()->GetPos();
	float len = (GetRadius() + radius) - distance.Length();
	// �����o������
	distance = distance.Normalize();
	Vector3 vec = distance * len;
	// �����o��
	Transform()->SetPos(Transform()->GetPos() + vec);
}


#ifdef BUILD_MODE

void CSphereCollision::ImGuiDebug()
{
	CCollision::ImGuiDebug();
	if (!m_pDebugSphere)
		return;

	if (ImGui::DragFloat("radius", &m_fRadius))
	{
		m_pDebugSphere->Init(16, 8, m_fRadius);
	}

	if (ImGui::Button("sphere resize"))
	{
		if (auto model = GetComponent<CModelRenderer>(); model)
			m_fRadius = model->GetModel().lock()->GetRadius();
		else
			m_fRadius = Transform()->GetScale().GetLargeValue();
	}

	// debug�\��
	XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
	XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());

	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	mWorld = XMMatrixTranslationFromVector(vCenter);
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);
	m_pDebugSphere->SetWorld(&mW);

	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->SetDebugMesh(
		std::string(std::to_string(m_pDebugSphere->GetIndexNum()) + std::to_string(m_pDebugSphere->GetMaterial()->GetFloat())),
		mW,
		m_pDebugSphere.get()
	);

}

// ������
void CSphereCollision::Update()
{
	//if (!this->IsActive())
		return;

	// debug�\��
	XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
	XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());

	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	mWorld = XMMatrixTranslationFromVector(vCenter);
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);
	m_pDebugSphere->SetWorld(&mW);

	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->SetDebugMesh(
		std::string(std::to_string(m_pDebugSphere->GetIndexNum()) + std::to_string(m_pDebugSphere->GetMaterial()->GetFloat())),
		mW,
		m_pDebugSphere.get()
	);
}

#endif // BUILD_MODE