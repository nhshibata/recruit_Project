//=========================================================
// [UIAnime.h] 
// �쐬:2022/09/05
//---------------------------------------------------------
// title,result��UI���J�f�𓮂����邽�߂̸׽
// ���:1 �h��:2
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __UI_ANIME_COMPONENT_H__
#define __UI_ANIME_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>

/*
	*@ UI�ŃL�����N�^�[���ړ�������Ǘ��׽
	*@ object�𐶐����A�R���g���[�������̸׽���s��
*/
class CUIAnime : public CComponent
{
protected:
	// *@polygon�ɓn�����
	struct STParam
	{
		Vector2 vPos;
		float fAngle;
		STParam() {};
		STParam(Vector2 pos, float angle) :vPos(pos), fAngle(angle) {};
	};
protected:
	//--- ���ޕϐ�(���J�v�Z�������ׂ�?)
	RectTransWeakPtr m_pMyRect;			// ���g��Rect
	std::vector<std::weak_ptr<CRectTransform>> m_pRectList;	// �ړ����������|���S���̎Q��
	std::vector<STParam> m_vParamList;	// ���W�A�p�x�i�[�p
	int m_nPolygonNum = 10;				// �i�[��
	int m_nUpdateCnt = 0;				// ���W�������n���J�E���g
	float m_fPolygonDir = 0.0f;			// �ړ�����
	float m_fSpeed = 1.0f;				// �ړ����x

public:
	//--- ���ފ֐�
	CUIAnime() {};
	CUIAnime(std::shared_ptr<CGameObject> owner);
	~CUIAnime() {};

	virtual void Awake();
	virtual void Update();
};
// *@���S����]���邾��
class CUIAnimeRot : public CUIAnime
{
private:

public:
	CUIAnimeRot() {};
	CUIAnimeRot(std::shared_ptr<CGameObject> owner) : CUIAnime(owner) { m_nPolygonNum = 50; };//�S��
	~CUIAnimeRot() {};

	virtual void Awake();
	virtual void Update();
};
// *@�J�[�u�ɉ����Ĉړ�������
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