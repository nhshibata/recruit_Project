//=========================================================
// [boxCollision.h]
//---------------------------------------------------------
//作成:2022/05/02
//---------------------------------------------------------
// コリジョンｸﾗｽ(当たり判定 ) : 派生ｸﾗｽ
//=========================================================

//--- インクルード部
#include <GameSystem/Component/Collision/boxCollision.h>
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <GameSystem/Component/Transform/rigidbody.h>
#include <GameSystem/GameObject/gameObject.h>
#include <GameSystem/Manager/sceneManager.h>
#include <CoreSystem/Time/fps.h>

#if BUILD_MODE

#include <DebugSystem/imGuiPackage.h>
#include <GraphicsSystem/Render/box.h>
#include <GameSystem/Manager/drawSystem.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Renderer/sphereRenderer.h>

#endif // BUILD_MODE

using namespace MySpace::Game;

//==========================================================
// コンストラクタ
//==========================================================
CBoxCollision::CBoxCollision()
{

#if BUILD_MODE
	m_pDebugBox = std::make_shared<CBox>();
	m_pDebugBox->Init(Vector3(1,1,1));
	m_pDebugBox->SetDiffuse(Vector4(0, 1, 0, 0.5f));
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->SetDebugMesh(
		std::string(std::to_string(m_pDebugBox->GetIndexNum()) + std::to_string(m_pDebugBox->GetMaterial()->GetFloat())),
		m_pDebugBox.get()
	);
#endif // BUILD_MODE

}
//==========================================================
// 引き数付きコンストラクタ
//==========================================================
CBoxCollision::CBoxCollision(std::shared_ptr<CGameObject> owner, Vector3 size)
	: CCollision(owner), m_vSize(size), m_bOBBMode(true)
{

#if BUILD_MODE
	m_pDebugBox = std::make_shared<CBox>();
	m_pDebugBox->Init(size);
	m_pDebugBox->SetDiffuse(Vector4(0, 1, 0, 0.5f));
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->SetDebugMesh(
		std::string(std::to_string(m_pDebugBox->GetIndexNum()) + std::to_string(m_pDebugBox->GetMaterial()->GetFloat())),
		m_pDebugBox.get()
	);
#endif // BUILD_MODE

}

//==========================================================
// デストラクタ
//==========================================================
CBoxCollision::~CBoxCollision()
{
#if BUILD_MODE
	auto sys = SceneManager::CSceneManager::Get()->GetDrawSystem();
	sys->ReleaseDebugMesh(m_pDebugBox.get());
	m_pDebugBox->Fin();
	m_pDebugBox.reset();
#endif // BUILD_MODE
}

//==========================================================
// 当たり判定関数
//==========================================================
bool CBoxCollision::Box(Vector3 Apos, Vector3 Asize, Vector3 Bpos, Vector3 Bsize)
{
	return (Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		(Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		(Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		(Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		(Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		(Bpos.z - Bsize.z <= Apos.z + Asize.z);
}

//==========================================================
// 自身と引き数情報からの当たり判定
//==========================================================
bool CBoxCollision::CollisionAABB(Vector3 pos, Vector3 size)
{
	if (Box(Transform()->GetPos(), m_vSize, pos, size))
	{
		return true;
	}
	return false;
}

//==========================================================
// 当たり判定OBB
//==========================================================
bool CBoxCollision::CollisionOBB(CTransform* trans, Vector3 center, Vector3 size)
{
	// ワールドマトリックス取得
	XMFLOAT4X4 mW1 = Transform()->GetWorldMatrix();
	XMFLOAT4X4 mW2 = trans->GetWorldMatrix();

	// 中心座標取得
	XMFLOAT3 vP1, vP2;
	XMStoreFloat3(&vP1, XMVector3TransformCoord(XMLoadFloat3(&m_vCenter), XMLoadFloat4x4(&mW1)));
	XMStoreFloat3(&vP2, XMVector3TransformCoord(XMLoadFloat3(&center), XMLoadFloat4x4(&mW2)));

	// 中心座標間のベクトルを求める
	XMVECTOR vD;
	vD = XMVectorSet(vP2.x - vP1.x, vP2.y - vP1.y, vP2.z - vP1.z, 0.0f);

	// モデル座標軸を求める
	XMVECTOR vN[6];
	vN[0] = XMVectorSet(mW1._11, mW1._12, mW1._13, 0.0f);
	vN[1] = XMVectorSet(mW1._21, mW1._22, mW1._23, 0.0f);
	vN[2] = XMVectorSet(mW1._31, mW1._32, mW1._33, 0.0f);
	vN[3] = XMVectorSet(mW2._11, mW2._12, mW2._13, 0.0f);
	vN[4] = XMVectorSet(mW2._21, mW2._22, mW2._23, 0.0f);
	vN[5] = XMVectorSet(mW2._31, mW2._32, mW2._33, 0.0f);

	// OBBの大きさ(半分)を掛けたベクトルを求める
	XMFLOAT3& vBB1 = m_vSize;
	XMFLOAT3& vBB2 = size;
	XMVECTOR vL[6];
	vL[0] = XMVectorSet(mW1._11 * vBB1.x, mW1._12 * vBB1.x, mW1._13 * vBB1.x, 0.0f);
	vL[1] = XMVectorSet(mW1._21 * vBB1.y, mW1._22 * vBB1.y, mW1._23 * vBB1.y, 0.0f);
	vL[2] = XMVectorSet(mW1._31 * vBB1.z, mW1._32 * vBB1.z, mW1._33 * vBB1.z, 0.0f);
	vL[3] = XMVectorSet(mW2._11 * vBB2.x, mW2._12 * vBB1.x, mW2._13 * vBB2.x, 0.0f);
	vL[4] = XMVectorSet(mW2._21 * vBB2.y, mW2._22 * vBB1.y, mW2._23 * vBB2.y, 0.0f);
	vL[5] = XMVectorSet(mW2._31 * vBB2.z, mW2._32 * vBB1.z, mW2._33 * vBB2.z, 0.0f);

	// 分離軸候補が面の法線ベクトル(モデル座標軸)
	float fL, f, fD;
	for (int cnt = 0; cnt < 5; ++cnt)
	{
		XMVECTOR& vS = vN[cnt]; // 分離軸候補
		// OBBの影(の半分)の合計
		fL = 0.0f;
		for (int cntObb = 0; cntObb < 6; ++cntObb)
		{
			XMStoreFloat(&f, XMVector3Dot(vS, vL[cntObb]));
			fL += fabsf(f);
		}

		// 中心座標間の距離と比較
		XMStoreFloat(&fD, XMVector3Dot(vS, vD));
		fD = fabsf(fD);
		if (fL < fD)
			return false; // 当たっていない
	}

	// 分離軸候補がそれぞれの辺に垂直なベクトル
	XMVECTOR vS;
	for (int cntI = 0; cntI < 3; ++cntI)
	{
		for (int cntJ = 0; cntJ < 6; ++cntJ)
		{
			// 分離軸候補を求める
			vS = XMVector3Normalize(XMVector3Cross(vN[cntI], vN[cntJ]));

			// OBBの影（の半分)の合計
			fL = 0.0f;
			for (int cntK = 0; cntK < 6; ++cntK)
			{
				XMStoreFloat(&f, XMVector3Dot(vS, vL[cntK]));
				fL += fabsf(f);
			}

			// 中心座標間の距離と比較
			XMStoreFloat(&fD, XMVector3Dot(vS, vD));
			fD = fabsf(fD);
			if (fL < fD)
				return false;	// 当たっていない
		}
	}
	return true;
}

//==========================================================
// Collision同士の当たり判定
// コリジョンｸﾗｽを引き数にとって、当たり判定を行う
//==========================================================
bool CBoxCollision::HitCheckPtr(CCollision* other)
{
	// null確認
	if (!other)
		return false;

	// size調整
	/*if (m_vOldScale != Transform()->GetScale())
	{
		auto scl = Transform()->GetScale() / m_vOldScale;
		SetSize(GetSize()* scl.GetLargeValue());
		m_vOldScale = Transform()->GetScale();
	}*/

	Vector3 size;
	// 派生クラスへのキャスト
	if (CBoxCollision* com = dynamic_cast<CBoxCollision*>(other); com)
	{
		size = com->GetSize();
		// 当たり判定
		if (m_bOBBMode)
		{
			if (!CollisionOBB(other->Transform(), other->GetCenter(), size))
				return false;
		}
		else
		{
			if (!CollisionAABB(other->Transform()->GetPos(), size))
				return false;
		}
	}
	else if (CSphereCollision* com = dynamic_cast<CSphereCollision*>(other); com)
	{
		size = com->GetRadius();
		if (!this->IsCollidedWithBox(com))
		{
			return false;
		}
	}
	else // ここに来ることは想定していない
	{
		return false;
	}

	// トリガーがOFFなら押し出し
	if (!IsTrigger() && !other->IsTrigger())
	{
		size = other->Transform()->GetScale() * (size * 2);
		/*XMFLOAT4X4 mW1 = Transform()->GetWorldMatrix();
		XMFLOAT4X4 invWorld;
		XMMATRIX worldMat = XMLoadFloat4x4(&mW1);
		XMVECTOR det;
		worldMat = XMMatrixInverse(&det, worldMat);
		auto vSize = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(size.x, size.y, size.z, 1.0f), worldMat);
		DirectX::XMStoreFloat3(&size, vSize);*/

		PosAdjustment(other, other->Transform()->GetPos(), size);
		other->HitResponse(this);
	}
	this->HitResponse(other);

	return true;
}

//==========================================================
// 押し出し
// TODO:完全ではない。当たった方向、適切なsizeによる押し出しではない
//==========================================================
void CBoxCollision::PosAdjustment(CCollision* otherCol, Vector3 otherPos, Vector3 otherSize)
{
#if 1
	const Vector3 size = Transform()->GetScale() * (m_vSize * 2);
	const Vector3 currentPos = Transform()->GetPos();
	// 距離
	float overlapDistance = CalculateOverlap(currentPos, otherPos, size, otherSize);
	Vector3 moveVec = CalculateMoveVector(currentPos, otherPos, overlapDistance);
	// 新しい座標
	Vector3 newPos = currentPos;
	Vector3 newOtherPos = otherPos;
	// 移動しているかフラグ
	bool sleep = false;
	bool otherSleep = false;
	{
		auto oldPos = Transform()->GetOldPos();
		sleep = currentPos != oldPos;
		oldPos = otherCol->Transform()->GetOldPos();
		otherSleep = otherPos != oldPos;
	}
	if (!sleep && !otherSleep)
		return;

	if(sleep)
		newPos += moveVec;
	if(otherSleep)
		newOtherPos += moveVec;

	int loopCount = 0;
	const int maxLoopCount = 10;
	const float minOverlapDistance = 0.01f;
	while (loopCount < maxLoopCount)
	{
		if (CBoxCollision::Box(newPos, size, newOtherPos, otherSize))
		{
			overlapDistance = CalculateOverlap(newPos, newOtherPos, size, otherSize);
			moveVec = CalculateMoveVector(newPos, newOtherPos, overlapDistance);
			if (sleep)
				newPos += moveVec;
			if (otherSleep)
				newOtherPos += moveVec;				

			// めり込み距離が一定値以下になった場合はループを終了
			if (overlapDistance < minOverlapDistance)
				break;

			loopCount++;
		}
		else
			break;
	}

	Transform()->SetPos(newPos);
	otherCol->Transform()->SetPos(newOtherPos);
	

#elif 2
	// 現在位置と以前の位置を取得
	Vector3 currentPos = Transform()->GetPos();
	const Vector3 oldPos = Transform()->GetOldPos();
	
	// 2つの矩形の距離を計算
	Vector3 distance = otherPos - currentPos;
	const float totalRadius = (GetSize().GetLargeValue() + otherSize.GetLargeValue());
	const float distanceLength = distance.Length();

	// 矩形同士が重なっている場合
	if (distanceLength < totalRadius)
	{
		// 押し戻し処理
		if (distanceLength > 0.0f)
		{
			Vector3 direction = distance.Normalize();
			Vector3 displacement = direction * (totalRadius - distanceLength);
			Vector3 newPos = currentPos - displacement;

			Transform()->SetPos(newPos);

			if (CBoxCollision::Box(newPos, this->m_vSize, otherPos, otherSize))
			{
				Transform()->SetPos(oldPos);
			}
		}
		else
		{
			// 矩形同士が重なっているが、位置が同じ場合
			// 移動が停止してしまうため、一度前の位置に戻す
			Transform()->SetPos(oldPos);
		}
	}
#elif 3
	// 現在位置と以前の位置を取得
	Vector3 currentPos = Transform()->GetPos();
	const Vector3 oldPos = Transform()->GetOldPos();

	// 中心点からの距離を計算
	Vector3 distance = otherPos - currentPos;
	float distanceX = fabsf(distance.x);
	float distanceY = fabsf(distance.y);
	float distanceZ = fabsf(distance.z);

	// 各矩形のサイズの半分を計算
	float myHalfX = m_vSize.x / 2.0f;
	float myHalfY = m_vSize.y / 2.0f;
	float myHalfZ = m_vSize.z / 2.0f;
	float otherHalfX = otherSize.x / 2.0f;
	float otherHalfY = otherSize.y / 2.0f;
	float otherHalfZ = otherSize.z / 2.0f;

	// 矩形同士が重なっている場合
	if (distanceX < myHalfX + otherHalfX &&
		distanceY < myHalfY + otherHalfY &&
		distanceZ < myHalfZ + otherHalfZ)
	{
		// 押し戻し処理
		float xOverlap = myHalfX + otherHalfX - distanceX;
		float yOverlap = myHalfY + otherHalfY - distanceY;
		float zOverlap = myHalfZ + otherHalfZ - distanceZ;

		if (xOverlap < yOverlap && xOverlap < zOverlap)
		{
			// x軸方向に押し戻す
			if (distance.x < 0)
			{
				currentPos.x -= xOverlap;
			}
			else
			{
				currentPos.x += xOverlap;
			}
		}
		else if (yOverlap < zOverlap)
		{
			// y軸方向に押し戻す
			if (distance.y < 0)
			{
				currentPos.y -= yOverlap;
			}
			else
			{
				currentPos.y += yOverlap;
			}
		}
		else
		{
			// z軸方向に押し戻す
			if (distance.z < 0)
			{
				currentPos.z -= zOverlap;
			}
			else
			{
				currentPos.z += zOverlap;
			}
		}

		// 座標を更新
		Transform()->SetPos(currentPos);
	}
#else

	Vector3 checkPos = Transform()->GetPos();
	Vector3 oldPos = Transform()->GetOldPos();

	//---  押し出し
	// 2点間と２半径の差
	Vector3 distance = Transform()->GetPos() - otherPos;
	float len = (GetSize().GetLargeValue() * 2 + otherSize.GetLargeValue() * 2) - distance.Length();
	// 押し出す方向
	distance = distance.Normalize();
	Vector3 vec = distance * len;
	// 押し出し
	Transform()->SetPos(Transform()->GetPos() + vec);

#endif // 1

}

bool CBoxCollision::IsCollidedWithBox(CSphereCollision* sphere)
{
	// 球と矩形の中心点の距離
	Vector3 distance = sphere->Transform()->GetPos() - this->Transform()->GetPos();
	float distLen = distance.Length();

	// 矩形の対角線の長さ
	float diagonalLen = sqrt(pow(this->GetSize().x, 2) + pow(this->GetSize().y, 2) + pow(this->GetSize().z, 2)) / 2;

	// 球と矩形の中心点の距離が、球の半径と矩形の対角線の長さの半分よりも小さい場合、当たりとみなす
	if (distLen < sphere->GetRadius() + diagonalLen)
	{
		return true;
	}

	return false;
}

//==========================================================
// 2つのボックスがめり込んでいる距離を計算する関数
//==========================================================
float CBoxCollision::CalculateOverlap(
	const Vector3& currentPos, const Vector3& otherPos, 
	const Vector3& size1, const Vector3& size2)
{
	// 2つの矩形の中心間の距離を計算
	Vector3 bPos = otherPos;
	Vector3 centerOffset =  bPos - currentPos;

	// 2つの矩形の幅の合計
	float totalWidth = size1.x + size2.x;

	// 2つの矩形の高さの合計
	float totalHeight = size1.y + size2.y;

	// 2つの矩形の奥行きの合計
	float totalDepth = size1.z + size2.z;

	// 2つの矩形の幅、高さ、奥行きのうち、最小のものを取得
	float minDimension = std::min(std::min(totalWidth, totalHeight), totalDepth);
	
	float length = centerOffset.Length();

	// 2つの矩形の中心間の距離が最小の矩形の大きさより小さい場合、めり込んでいる距離を計算
	if (length < minDimension / 2)
	{
		return minDimension / 2 - length;
	}

	return 0.0f;
}

//==========================================================
// めり込んだ距離から移動ベクトルを計算する関数
//==========================================================
Vector3 CBoxCollision::CalculateMoveVector(const Vector3& currentPos, const Vector3& otherPos, const float& overlap)
{
	// 2つのボックスの位置の差分を取得
	/*Vector3 aPos = currentPos;
	Vector3 direction = aPos - otherPos;*/
	Vector3 aPos = otherPos;
	Vector3 direction = aPos - currentPos;

	// ノーマライズしてめり込み分の距離を移動するベクトルを取得
	direction = direction.Normalize();
	direction *= overlap;

	return direction;
}



#ifdef BUILD_MODE

void CBoxCollision::ImGuiDebug()
{
	CCollision::ImGuiDebug();

	ImGui::Separator();

	Debug::SetTextAndAligned("OBB");
	ImGui::Checkbox("##OBB", (bool*)&m_bOBBMode);

	Debug::SetTextAndAligned("box size");
	// 3次元座標
	if (ImGui::DragFloat3("##boxsize", (float*)&m_vSize))
	{
		m_pDebugBox->Init(m_vSize);
	}

	if (ImGui::Button("box resize"))
	{
		if (auto model = GetComponent<CModelRenderer>(); model)
			m_vSize = model->GetModel().lock()->GetBBox();
		else
			m_vSize = Transform()->GetScale();
	}

}

// とりあえずここに仮実装
void CBoxCollision::Update()
{
	//--- デバッグ表示
	XMFLOAT4X4 mW;
	if (m_bOBBMode)
	{
		// 平行移動マトリックス生成
		Vector3 center = GetCenter();
		// 0.001はZファイティング防止
		auto renderSize = Transform()->GetScale() * (m_vSize / 2) * 1.001f;
		XMMATRIX mMove = XMMatrixTranslation(center.x, center.y, center.z);
		Matrix4x4 mtx = Matrix4x4::CalcWorldMatrix(Transform()->GetPos(), Transform()->GetRot(), renderSize);
		
		// モデル
		XMMATRIX mWorld = XMLoadFloat4x4(&mtx);
		mWorld = XMMatrixMultiply(mMove, mWorld);
		// 境界ボックス表示
		XMStoreFloat4x4(&mW, mWorld);
		m_pDebugBox->SetWorld(&mW);
	}
	else
	{
		XMVECTOR vCenter = XMLoadFloat3(&GetCenter());
		// 0.001はZファイティング防止
		auto renderSize = Transform()->GetScale() * (m_vSize / 2) * 1.001f;
		Matrix4x4 mtx = Matrix4x4::CalcWorldMatrix(Transform()->GetPos(), Transform()->GetRot(), renderSize);
		XMMATRIX mWorld = XMLoadFloat4x4(&mtx);

		vCenter = XMVector3TransformCoord(vCenter, mWorld);
		mWorld = XMMatrixTranslationFromVector(vCenter);
		XMStoreFloat4x4(&mW, mWorld);

		m_pDebugBox->SetWorld(&mW);
	}

}

#endif // BUILD_MODE