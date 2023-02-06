//=========================================================
//作成:2022/05/02
// コリジョンｸﾗｽ(当たり判定 ) : 派生ｸﾗｽ
//=========================================================

//--- インクルード部
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
// 引き数付きコンストラクタ
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
// デストラクタ
//==========================================================
CSphereCollision::~CSphereCollision()
{
#if BUILD_MODE
	m_pDebugSphere->Fin();
	m_pDebugSphere.reset();
#endif // BUILD_MODE
}

//==========================================================
// 球の当たり判定
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
// 球の当たり判定
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
// 当たり判定
// コリジョンｸﾗｽを引き数にとって、当たり判定を行う
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

	// ｺﾝﾎﾟｰﾈﾝﾄの確認
	if (CSphereCollision* com = other->GetComponent<CSphereCollision>(); com)
	{
		// 球の判定
		if (CollisionSphere(other->Transform()->GetPos(), com->GetRadius()))
		{
			// 押し出し
			if (!trigger)
			{
				PushObject(com, com->GetRadius());
				// 押し出しを行うと相手は判定されないため、ここで行う
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
			// 押し出し
			if (!trigger)
			{				
				PushObject(other, otherBox->GetSize().GetLargeValue());
				// 押し出しを行うと相手は判定されないため、ここで行う
				other->HitResponse(this);
			}
			this->HitResponse(other);

			return true;
		}
	}

	return false;
}

//==========================================================
// 押し出し
//==========================================================
void CSphereCollision::PushObject(CCollision* other, float radius)
{
	//---  押し出し
	// 二点間と２半径の差
	Vector3 distance = Transform()->GetPos() - other->Transform()->GetPos();
	float len = (GetRadius() + radius) - distance.Length();
	// 押し出す方向
	distance = distance.Normalize();
	Vector3 vec = distance * len;
	// 押し出し
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

	// debug表示
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

// 仮実装
void CSphereCollision::Update()
{
	//if (!this->IsActive())
		return;

	// debug表示
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