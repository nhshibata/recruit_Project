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

#include <GraphicsSystem/Render/Sphere.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Renderer/sphereRenderer.h>

#include <DebugSystem/imGuiPackage.h>

#endif // BUILD_MODE

using namespace MySpace::Game;

//==========================================================
// コンストラクタ
//==========================================================
CSphereCollision::CSphereCollision()
{

#if BUILD_MODE
	m_pDebugSphere = std::make_shared<CSphere>();
	m_pDebugSphere->Init(16, 8, m_fRadius);
	m_pDebugSphere->SetDiffuse(Vector4(0, 1, 0, 0.5f));
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->SetDebugMesh(
		std::string(std::to_string(m_pDebugSphere->GetIndexNum()) + std::to_string(m_pDebugSphere->GetMaterial()->GetFloat())),
		m_pDebugSphere.get()
	);
#endif //

}

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
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->SetDebugMesh(
		std::string(std::to_string(m_pDebugSphere->GetIndexNum()) + std::to_string(m_pDebugSphere->GetMaterial()->GetFloat())),
		m_pDebugSphere.get()
	);
#endif //
}

//==========================================================
// デストラクタ
//==========================================================
CSphereCollision::~CSphereCollision()
{
#if BUILD_MODE
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->ReleaseDebugMesh(m_pDebugSphere.get());
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
				PushBack(com, com->GetRadius());
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
		//if (otherBox->Box(Transform()->GetPos(), size, other->Transform()->GetPos(), otherBox->GetSize()))
		if(BoxSphere(otherBox, this))
		{
			// 押し出し
			if (!trigger)
			{				
				PushBack(other, otherBox->GetSize().GetLargeValue());
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
void CSphereCollision::PushBack(CCollision* other, float radius)
{
#if 1
	CTransform* transform = Transform();
	CTransform* otherTransform = other->Transform();

	// 2点間と２半径の差
	Vector3 distance = transform->GetPos() - otherTransform->GetPos();
	float len = (GetRadius() + radius) - distance.Length();

	// 押し出す方向
	distance = distance.Normalize();
	Vector3 pushVec = distance * len;

	// 押し戻し
	transform->SetPos(transform->GetPos() + pushVec);
#else
	//---  押し出し
		// 二点間と２半径の差
	Vector3 distance = Transform()->GetPos() - other->Transform()->GetPos();
	float len = (GetRadius() + radius) - distance.Length();
	// 押し出す方向
	distance = distance.Normalize();
	Vector3 vec = distance * len;
	// 押し出し
	Transform()->SetPos(Transform()->GetPos() + vec);
#endif // 1

}

bool CSphereCollision::BoxSphere(CBoxCollision* box, CSphereCollision* sphere)
{
	Vector3 boxCenter = box->Transform()->GetPos();
	Vector3 sphereCenter = sphere->Transform()->GetPos();
	float distance = (boxCenter - sphereCenter).Length();

	// 矩形の中心から最も近い球の表面上の点を求める
	Vector3 closestPoint = sphereCenter;
	if (distance > 0)
	{
		closestPoint = boxCenter + (sphereCenter - boxCenter) * (box->GetSize() / distance);
	}

	// その点と球の中心点の距離を求め、半径と比較する
	distance = (closestPoint - sphereCenter).Length();
	return distance <= sphere->GetRadius();
}

#ifdef BUILD_MODE

void CSphereCollision::ImGuiDebug()
{
	CCollision::ImGuiDebug();

	ImGui::Separator();

	if (!m_pDebugSphere)
		return;

	Debug::SetTextAndAligned("collision radius");
	if (ImGui::DragFloat("##radius", &m_fRadius))
	{
		m_pDebugSphere->Init(16, 8, m_fRadius);
	}

	Debug::SetTextAndAligned("sphere resize");
	if (ImGui::Button("##sphereresize"))
	{
		if (auto model = GetComponent<CModelRenderer>(); model)
			m_fRadius = model->GetModel().lock()->GetRadius();
		else
			m_fRadius = Transform()->GetScale().GetLargeValue();
	}

}

// 仮実装
void CSphereCollision::Update()
{
	// debug表示
	XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
	XMMATRIX mWorld = XMLoadFloat4x4(&Transform()->GetWorldMatrix());

	vCenter = XMVector3TransformCoord(vCenter, mWorld);
	mWorld = XMMatrixTranslationFromVector(vCenter);
	XMFLOAT4X4 mW;
	XMStoreFloat4x4(&mW, mWorld);
	m_pDebugSphere->SetWorld(&mW);
}

#endif // BUILD_MODE