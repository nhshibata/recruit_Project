//========================================================
// [gameSceneManager.cpp]
//---------------------------------------------------------
// 作成:2023/01/02
//---------------------------------------------------------
// 
//========================================================

//--- インクルード部
#include "titleSceneManager.h"
#include "fadeController.h"
#include "spellDefine.h"
#include "titleMove.h"

#include <CoreSystem/Input/input.h>
#include <CoreSystem/Sound/Sound.h>

using namespace Spell;

// 後で分ける
class CAlphaMove : public CComponent
{
private: 
	bool m_bDir;
	CPolygonRenderer* m_pPolygon;
public:
	CAlphaMove()
		:m_bDir(false)
	{};
	CAlphaMove(CGameObject::Ptr owner)
		:CComponent(owner)
		,m_bDir(false)
	{};
	~CAlphaMove() {};
	
	// *@生成時呼び出し
	virtual void Awake() {};
	void Init()
	{
		m_pPolygon = GetOwner()->GetComponent<CPolygonRenderer>().lock().get();
	}
	void Update()
	{
		Color color = m_pPolygon->GetColor();
		if (m_bDir)
		{
			color.a += CFps::Get().DeltaTime();
			if (color.a >= 1.0f)
				m_bDir = false;
		}
		else
		{
			color.a -= CFps::Get().DeltaTime();
			if (color.a <= 0.0f)
				m_bDir = true;
		}
		m_pPolygon->SetColor(color);
	}

};

class CParticle : public CComponent
{
private:
	CRectTransform* m_pRect;
	Vector2 m_vVel;				// 移動量
	Vector2 m_vForce;			// 力
	float m_fGravity;
	float m_fMass;
	Vector2 m_vStartPos;

public:
	CParticle() 
	{}
	CParticle(CGameObject::Ptr owner)
		:CComponent(owner)
		,m_fGravity(0.98f), m_fMass(1.0f)
	{

	}
	~CParticle()
	{}

	void Awake()
	{
		auto image = GetComponent<CPolygonRenderer>();
		m_pRect = image->GetRectTransform().get();

		m_vVel = Vector2(float(rand() % 22 - 10), float((rand() % 26 + 26) * -1));
		m_vForce = Vector2(0, -0.9f);

		m_vStartPos = m_pRect->GetPos();
	}
	void Update()
	{
		Vector2 pos = m_pRect->GetPos();
		m_vForce.y += m_fGravity * CFps::Get().DeltaTime();
		Vector2 vec = m_vForce / m_fMass;
		m_vVel += vec * CFps::Get().DeltaTime();
		pos += m_vVel * CFps::Get().DeltaTime();
		
		if (pos.y >= CScreen::GetHalfHeight() || pos.y <= -CScreen::GetHalfHeight() ||
			pos.x >= CScreen::GetHalfWidth() || pos.x <= -CScreen::GetHalfWidth())
		{
			pos = m_vStartPos;
			m_vVel = Vector2(float(rand() % 22 - 10), float((rand() % 26 + 26) * -1));
			m_vForce = Vector2(0, -0.9f);
		}
		m_pRect->SetPos(pos);
	}

	void SetPos(const Vector2 pos) { m_vStartPos = pos;}
};

class CColorMove : public CComponent
{
private:
	std::vector<Vector3> m_aCurveList;
	CPolygonRenderer* m_pPolygon;
	double m_dwTime;

public:
	CColorMove() 
		:m_pPolygon(nullptr), m_dwTime(0)
	{};
	CColorMove(CGameObject::Ptr owner)
		:CComponent(owner)
		,m_pPolygon(nullptr), m_dwTime(0)
	{};
	~CColorMove() {};

	void Awake()
	{
		m_pPolygon = GetComponent<CPolygonRenderer>();
		
	}
	void Update()
	{
		// 道が決まっていない
		if (m_aCurveList.size() == 0)
		{
			auto color = m_pPolygon->GetColor();
			Vector3 vec1, vec2, startPos, endPos;
			int num = rand() % 15 + 10;
			float dir = color.r + color.g + color.b;
			startPos.x = color.r;
			startPos.y = color.g;
			startPos.z = color.b;
			endPos = startPos;
			vec1 = Vector3(float(rand() % 2 ? 1 : -1), float(rand() % 2 ? 1 : -1), float(rand() % 2 ? 1 : -1));
			vec2 = Vector3(float(rand() % 2 ? 1 : -1), float(rand() % 2 ? 1 : -1), float(rand() % 2 ? 1 : -1));

			if (dir < 3.0f / 2)
			{
				endPos = Vector3(1, 1, 1);
			}
			else
			{
				endPos = Vector3(0.3f, 0.3f, 0.3f);
			}

			m_aCurveList = ST_MyMath::MakeHermiteCurve(startPos, vec1, endPos, vec2, num);
			return;
		}

		++m_dwTime;
		if (m_dwTime > 4)
		{
			m_dwTime = 0;
		}
		else
			return;

		// 先頭から順に移動
		if (m_aCurveList.size() != 0)
		{
			auto value = m_aCurveList.front();
			m_pPolygon->SetColor(Color(value.x, value.y, value.z, 1));
			m_aCurveList.erase(m_aCurveList.begin());
		}
	}
};

//========================================================
// コンストラクタ
//========================================================
CTitleSceneManager::CTitleSceneManager()
{
}

//========================================================
// 引き数付きコンストラクタ
//========================================================
CTitleSceneManager::CTitleSceneManager(CGameObject::Ptr owner)
	:CComponent(owner)
{
}

//========================================================
// デストラクタ
//========================================================
CTitleSceneManager::~CTitleSceneManager()
{
	CSound::Stop(eBGM::BGM_TITLE);
}

//========================================================
// 生成時呼び出し
//========================================================
void CTitleSceneManager::Create()
{
	// 名前設定
	GetOwner()->SetName(Spell::OBJ_NAME_TITLE);

	//--- 音再生
	CSound::Play(eBGM::BGM_TITLE);
	
	//--- オブジェクト作成
	{
		//--- ロゴ
		CGameObject::Ptr obj = CGameObject::CreateObject().lock();
		const auto image = obj->AddComponent<CPolygonRenderer>();
		int num = image.use_count();
		image->SetImageName(FORDER_DIR(Data/Texture/title_Logo.png));
		image->SetZ(static_cast<int>(CPolygonRenderer::EZValue::DEFAULT));
		image->GetRectTransform()->SetPos(0, CScreen::GetHalfHeight() - CScreen::GetHalfHeight() * 0.6f);
		image->GetRectTransform()->SetSize(CScreen::GetWidth() / 3, CScreen::GetHeight() / 3);
		obj->SetName("Logo");
	}
	{
		//--- ロゴ背景
		CGameObject::Ptr obj = CGameObject::CreateObject().lock();
		auto image = obj->AddComponent<CPolygonRenderer>().get();
		image->SetImageName(FORDER_DIR(Data/Texture/title_Logo.png));
		image->SetZ(static_cast<int>(CPolygonRenderer::EZValue::DEFAULT) - 1);
		image->GetRectTransform()->SetPos(0, CScreen::GetHalfHeight() - CScreen::GetHalfHeight() * 0.6f);
		image->GetRectTransform()->SetSize(CScreen::GetWidth() / 3 * 1.05f, CScreen::GetHeight() / 3 * 1.05f);
		obj->AddComponent<CColorMove>();
		obj->SetName("LogoBG");
	}

	{
		//--- ボタン
		CGameObject* obj = CGameObject::CreateObject().lock().get();
		auto image = obj->AddComponent<CPolygonRenderer>().get();
		image->SetImageName(FORDER_DIR(Data/Texture/title_Button.png));
		image->SetZ(static_cast<int>(CPolygonRenderer::EZValue::DEFAULT));
		image->GetRectTransform()->SetPos(0, -CScreen::GetHalfHeight() + CScreen::GetHalfHeight() * 0.5f);
		image->GetRectTransform()->SetSize(CScreen::GetWidth() / 6, CScreen::GetHeight() / 6);
		obj->AddComponent<CAlphaMove>();
		obj->SetName("Button");
	}

	{
		//--- 背景
		CGameObject* obj = CGameObject::CreateObject().lock().get();
		auto image = obj->AddComponent<CPolygonRenderer>();
		image->SetImageName(FORDER_DIR(Data/Texture/title_BG.png));
		image->SetZ(static_cast<int>(CPolygonRenderer::EZValue::BG));
		image->GetRectTransform()->SetSize(CScreen::GetWidth(), CScreen::GetHeight());
		obj->SetName("BG");
	}

	//--- パーティクル
	//const LPCSTR aImage[] = {
	//	FORDER_DIR(Data/Texture/sticon1b-3.png),
	//	FORDER_DIR(Data/Texture/sticon1c-2.png),
	//	FORDER_DIR(Data/Texture/sticon1d-2.png),
	//};
	//for (int num = 0; num < 20; ++num)
	//{
	//	CGameObject* obj = CGameObject::CreateObject().lock().get();
	//	auto image = obj->AddComponent<CPolygonRenderer>();
	//	image->SetImageName(aImage[rand() % 3]);
	//	image->SetLayer(static_cast<int>(CLayer::E_Layer::UI) - 1);
	//	image->GetRectTransform()->SetPos(0, CScreen::GetHalfHeight() - CScreen::GetHalfHeight()/6);
	//	image->GetRectTransform()->SetSize(CScreen::GetHalfWidth() / 6, CScreen::GetHalfHeight() / 6);
	//	obj->AddComponent<CParticle>();
	//	obj->SetName("Particle_" + std::to_string(num));
	//}

	//--- 曲線移動オブジェクト
	{
		CGameObject* obj = CGameObject::CreateObject().lock().get();
		obj->AddComponent<CPolygonRenderer>();
		auto move = obj->AddComponent<Spell::CTitleMove>();
		move->SetStartPos(Vector2(-CScreen::GetWidth(), CScreen::GetHalfHeight() - CScreen::GetHalfHeight() / 7));
		auto image = obj->GetComponent<CPolygonRenderer>().lock().get();
		image->SetImageName(FORDER_DIR(Data/Texture/sticon4d-3.png));
		image->SetZ(static_cast<int>(CPolygonRenderer::EZValue::DEFAULT));
		image->GetRectTransform()->SetSize(CScreen::GetHalfWidth() / 6, CScreen::GetHalfHeight() / 6);
		obj->SetName("Curve1");
	}

	{
		CGameObject* obj = CGameObject::CreateObject().lock().get();
		obj->AddComponent<CPolygonRenderer>();
		auto move = obj->AddComponent<Spell::CTitleMove>();
		move->SetStartPos(Vector2(CScreen::GetWidth(), -CScreen::GetHalfHeight() + CScreen::GetHalfHeight() / 7));
		auto image = obj->GetComponent<CPolygonRenderer>().lock().get();
		image->SetImageName(FORDER_DIR(Data/Texture/sticon4f-3.png));
		image->SetZ(static_cast<int>(CPolygonRenderer::EZValue::DEFAULT));
		image->GetRectTransform()->SetSize(CScreen::GetHalfWidth() / 6, CScreen::GetHalfHeight() / 6);
		obj->SetName("Curve2");
	}
}

#pragma optimize("", off)
//========================================================
// 初期化
//========================================================
void CTitleSceneManager::Init()
{
	// ﾎﾟｲﾝﾀの取得
	auto obj = CGameObject::FindGameObjectWithTag(Spell::TAG_FADE);
	m_pFade = obj.lock()->GetComponent<CFadeController>();


}
#pragma optimize("", on)

//========================================================
// 更新
//========================================================
void CTitleSceneManager::Update()
{
	// フェード中は処理しない
	if (m_pFade.lock()->IsFadeProcess())
		return;

	if (CInput::GetKeyTrigger(VK_RETURN))
	{
		m_pFade.lock()->SetScene(Spell::OBJ_NAME_GAME);
	}

}

