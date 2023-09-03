//==========================================================
// [lightCollision.h]
//----------------------------------------------------------
// �쐬:2023/05/19 
//----------------------------------------------------------
// ���C�g�͈̔͊m�F�p
// �h�����邽�߂̊��N���X
//==========================================================

#ifndef __LIGHT_COLLISION_H__
#define __LIGHT_COLLISION_H__

#include <GameSystem/Component/component.h>
#include <GameSystem/Component/Light/pointLight.h>

namespace DarkGame
{
	using namespace MySpace::Game;

	class CLightCollision : public CComponent
	{
	private:
		std::weak_ptr<CPointLight> m_pLight;

	public:
		//--- �����o�֐�
		CLightCollision();
		CLightCollision(CGameObject::Ptr ptr);
		virtual ~CLightCollision();

		void OnLoad()override;
		virtual void Awake();
		void Init();
		virtual void Update();

		// *@���̃N���A
		virtual void Refresh() = 0;

		// *@�����𖞂����Ă��邩
		virtual bool IsRange(std::string spec) = 0;

		// *@Hit���ɊO������Ăяo���Ă��炤
		// *@�p�������ۂ�tag�Ŕ�����s���z��
		//virtual void SendHit(std::string hit) = 0;

		// *@�͈͊m�F
		// *@�m�F�ɏ����⓯���ɍs����������������ꍇ�͂��̊֐����I�[�o���C�h����
		virtual bool Check(const CLightCollision& other);

		// *@���C�g�̎擾
		// *@����I�u�W�F�N�g�ɃA�^�b�`����Ă���i���Ă��Ȃ���΂�������)
		CPointLight* GetLightPtr()const { return m_pLight.lock().get(); }
	};

}

#endif // !__LIGHT_COLLISION_H__
