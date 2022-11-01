//=========================================================
// [ground.h] 
// 作成:2022/09/07
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <ground.h>
#include <GameSystem/Component/Collision/boxCollision.h>

void CGround::Awake()
{
	auto model = AddComponent<CModelRenderer>();
	auto col = AddComponent<CBoxCollision>();
	Vector3 boxSize = model->GetModel().lock()->GetBBox();
	col->SetSize(boxSize * Transform()->GetScale());
	col->SetTrigger(true);
}
void CGround::Init()
{
	
}
void CGround::OnCollisionEnter(CGameObject* other)
{

}
void CGround::OnCollisionStay(CGameObject* other)
{
	
}
void CGround::OnCollisionExit(CGameObject* other)
{
	// プレイヤーが離れていたら元に戻す
	/*if (other->GetTagPtr()->Compare("Player"))
	{
		other->GetTransform()->SetPos(other->GetTransform()->GetOldPos());
	}*/
}

//=====================================================
void CGroundManager::Awake()
{
	CModelManager::Get().Load(MODEL_PATH2(foor.obj));
	m_pModelPtr = CModelManager::Get().GetModel(MODEL_PATH2(foor.obj));
	auto boxSize = m_pModelPtr.lock()->GetBBox();

	CGameObject::PtrWeak obj;
	std::shared_ptr<CModelRenderer> com;
	obj = CGameObject::CreateObject();
	obj.lock()->GetTransform()->SetScale({300, 1, 300});
	obj.lock()->GetTransform()->SetPos({0, -20, 0});
	obj.lock()->AddComponent<CGround>();
	obj.lock()->SetLayer(static_cast<int>(CLayer::E_Layer::MODEL)-1);
	com = obj.lock()->GetComponent<CModelRenderer>().lock();
	com->SetModel(m_pModelPtr.lock());

	return;

	int num = 12;
	const int numX = num / 2;
	const int numZ = num - num / 2;
	float sizeX = boxSize.x * 60 * 2 * numX;
	float sizeZ = boxSize.z * 60 * 2 * numZ;
	float posX = -sizeX / 2;
	float posZ = -sizeZ / 2;
	for (int cnt = 0; cnt < num / 2; ++cnt)
	{
		for (int cntZ = 0; cntZ < numZ; ++cntZ)
		{
			posZ += boxSize.z * 60 * 2;

			obj = CGameObject::CreateObject();
			obj.lock()->SetName("ground" + std::to_string(cnt * numX + cntZ));
			obj.lock()->AddComponent<CGround>();
			com = obj.lock()->GetComponent<CModelRenderer>().lock();
			com->SetModel(m_pModelPtr.lock());
			obj.lock()->GetTransform()->SetScale({ 60, 1, 60 });
			obj.lock()->GetTransform()->SetPos({ posX, -10, posZ });
			obj.lock()->SetLayer(static_cast<int>(CLayer::E_Layer::MODEL) - 1);
		}
		posZ = -sizeZ / 2;
		posX += boxSize.x * 60 * 2;
	}
}
void CGroundManager::Update()
{

}