//=========================================================
// [fadeController.h] 
// �쐬:2022/08/20
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __FADE_CONTROLLER_COMPONENT_H__
#define __FADE_CONTROLLER_COMPONENT_H__

//--- �C���N���[�h��
#include <functional>
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		class CPolygonRenderer;
	}
}

class CFadeController : public CComponent
{
public:
	enum class E_FadeState
	{
		NONE,			// *@�j���[�g����
		FADE_OUT_START,	// *@�t�F�[�h�A�E�g�J�n
		FADE_OUT,		// *@�t�F�[�h�A�E�g��(�֐��|�C���^�ł͏I����)
		FADE_IN_START,	// *@�t�F�[�h�C���J�n
		FADE_IN,		// *@�t�F�[�h�C��(�֐��|�C���^�ł͏I����)
		WAIT,			// *@�҂����
		MAX,
	};

private:
	std::weak_ptr<MySpace::Game::CPolygonRenderer> m_pPolygon;						// �|���S��ptr
	std::string m_SceneName;										// �ؑ֐�
	E_FadeState m_eState;											// ���
	float m_fFadeTime;												// �t�F�[�h���s����
	
	static inline std::map<E_FadeState, std::vector<std::function<void(void)>>> m_aFuncMap;		// �t�F�[�h���s���֐��|�C���^
	static inline std::map<E_FadeState, std::vector<std::function<void(void)>>> m_aLoopFuncMap;	// �t�F�[�h���s���֐��|�C���^(���[�v)
	
private:
	// *@�o�^���ꂽ�֐��̎��s
	void Call(E_FadeState state);

public:
	CFadeController();
	CFadeController(std::shared_ptr<CGameObject> owner);
	~CFadeController();

	virtual void Awake();
	virtual void Init();
	void Update();

	// *@�t�F�[�h�A�E�g��ɃV�[���ؑ�(�V�[�������������w��)
	// *@�ؑ֌�A�t�F�[�h�C��
	void SetScene(std::string sceneName);

	// *@�t�F�[�h�A�E�g�J�n
	void StartFadeOut();
	// *@�t�F�[�h�C���J�n
	void StartFadeIn();

	// *@�֐��|�C���^�ݒ�
	// *@�Ăяo���^�C�~���O�w��
	// *@������:void(class)(void)�^�֐��|�C���^, �C���X�^���X�|�C���^, (bool�^�ǉ��Ŕj������Ȃ�)
	template<class T>
	void SetTypeFunction(E_FadeState state, void (T::*func)(void), T* ptr)
	{
		m_aFuncMap[state].push_back(std::bind(func, ptr));
	}

	// *@�֐��|�C���^�ݒ�
	// *@�Ăяo���^�C�~���O�w��
	// *@������:void(class)(void)�^�֐��|�C���^, �C���X�^���X�|�C���^, (bool�^�ǉ��Ŕj������Ȃ�)
	template<class T>
	void SetTypeFunction(E_FadeState state, void (T::*func)(void), T* ptr, bool flg)
	{
		m_aLoopFuncMap[state].push_back(std::bind(func, ptr));
	}

	//--- �Q�b�^�[�E�Z�b�^�[

	// *@fade�󋵎擾
	const E_FadeState GetFadeState() const { return m_eState; }
	
	// *@�����Ă����Ԃ��m�F
	// *@fade���Ȃ�true
	const bool IsFadeProcess() { return (m_eState != E_FadeState::NONE); }
};


#endif // !__FADE_CONTROLLER_COMPONENT_H__
