//=========================================================
// [UIAnime.h] 
// 作成:2022/09/05
//---------------------------------------------------------
// title,resultでUIムカデを動かせるためのｸﾗｽ
// 基底:1 派生:2
//=========================================================

//--- インクルードガード
#ifndef __UI_ANIME_COMPONENT_H__
#define __UI_ANIME_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>

/*
	*@ UIでキャラクターを移動させる管理ｸﾗｽ
	*@ objectを生成し、コントロールをこのｸﾗｽが行う
*/
class CUIAnime : public CComponent
{
protected:
	// *@polygonに渡す情報
	struct STParam
	{
		Vector2 vPos;
		float fAngle;
		STParam() {};
		STParam(Vector2 pos, float angle) :vPos(pos), fAngle(angle) {};
	};
protected:
	//--- ﾒﾝﾊﾞ変数(よりカプセル化すべき?)
	RectTransWeakPtr m_pMyRect;			// 自身のRect
	std::vector<std::weak_ptr<CRectTransform>> m_pRectList;	// 移動させたいポリゴンの参照
	std::vector<STParam> m_vParamList;	// 座標、角度格納用
	int m_nPolygonNum = 10;				// 格納数
	int m_nUpdateCnt = 0;				// 座標を引き渡すカウント
	float m_fPolygonDir = 0.0f;			// 移動方向
	float m_fSpeed = 1.0f;				// 移動速度

public:
	//--- ﾒﾝﾊﾞ関数
	CUIAnime() {};
	CUIAnime(std::shared_ptr<CGameObject> owner);
	~CUIAnime() {};

	virtual void Awake();
	virtual void Update();
};
// *@中心を回転するだけ
class CUIAnimeRot : public CUIAnime
{
private:

public:
	CUIAnimeRot() {};
	CUIAnimeRot(std::shared_ptr<CGameObject> owner) : CUIAnime(owner) { m_nPolygonNum = 50; };//百足
	~CUIAnimeRot() {};

	virtual void Awake();
	virtual void Update();
};
// *@カーブに沿って移動させる
class CUIAnimeCurve : public CUIAnime
{
private:
	std::vector<Vector2> m_vCurvePos;
	int m_nCurveIdx = 0;

private:
	void InitCurve();
public:
	CUIAnimeCurve() {};
	CUIAnimeCurve(std::shared_ptr<CGameObject> owner) : CUIAnime(owner) {};
	~CUIAnimeCurve() {};

	virtual void Awake();
	virtual void Update();
};

#endif //!__UI_ANIME_COMPONENT_H__