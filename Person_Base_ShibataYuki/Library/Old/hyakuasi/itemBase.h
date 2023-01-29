//=========================================================
// [itemBase.h] 
// 作成:2022/08/16
//---------------------------------------------------------
//=========================================================

#ifndef __ITEM_BASE_COMPONENT_H__
#define __ITEM_BASE_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>

class CItemManager;
class CItemBase : public CComponent
{
private:
	std::weak_ptr<CItemManager> m_pManager;
	int m_nUseCnt = 1;		// 使用回数

public:
	CItemBase() {};
	CItemBase(std::shared_ptr<CGameObject> owner);
	~CItemBase() {};
	void Awake();

	inline void SetUseCnt(int num) { m_nUseCnt = num; };
	inline void SetManager(std::shared_ptr<CItemManager> ptr) { m_pManager = ptr; }
	void OnTriggerEnter(CGameObject* other) { OnTriggerStay(other); };
	void OnTriggerStay(CGameObject* other);
};
class CLifeItem : public CItemBase
{
private:
	std::weak_ptr<CItemManager> m_pManager;
	int m_nUseCnt = 1;		// 使用回数

public:
	CLifeItem() {};
	CLifeItem(std::shared_ptr<CGameObject> owner) :CItemBase(owner){};
	~CLifeItem() {};
	void Awake();

	void OnTriggerEnter(CGameObject* other) { OnTriggerStay(other); };
	void OnTriggerStay(CGameObject* other);
};

class CItemManager : public CComponent
{
public:
	static inline constexpr const char* TAG_NAME = "ItemManager";

private:
	std::vector<std::weak_ptr<CItemBase>> m_pItemList;
	std::vector<std::weak_ptr<CItemBase>> m_pWaitItem;

public:
	CItemManager() {};
	CItemManager(std::shared_ptr<CGameObject> owner);
	~CItemManager() {};
	void Awake();
	void Init();
	void Update();

	void Create(Vector3 pos, int cnt = 2);
	void SetStandby(std::shared_ptr<CItemBase> ptr) { m_pWaitItem.push_back(ptr); }
};

#endif // !__ITEM_BASE_COMPONENT_H__
