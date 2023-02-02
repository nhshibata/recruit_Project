//=========================================================
// [boxCollision.h]
//---------------------------------------------------------
//�쐬:2022/05/02
//---------------------------------------------------------
// �R���W�����׽(�����蔻�� ) : �h���׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __BOX_COLLISION_COMPONENT_H__
#define __BOX_COLLISION_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/Collision/collision.h>

#pragma region ForwardDeclaration
namespace MySpace
{
	namespace Graphics
	{
		class CBox;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		using MySpace::Graphics::CBox;

		//--- �N���X��`
		class CBoxCollision : public CCollision
		{
		private:
			// �V���A���C�Y
			friend cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(cereal::make_nvp("boxCollision", cereal::base_class<CCollision>(this)),
					CEREAL_NVP(m_vSize), CEREAL_NVP(m_bOBBMode));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(cereal::make_nvp("boxCollision", cereal::base_class<CCollision>(this)),
					CEREAL_NVP(m_vSize), CEREAL_NVP(m_bOBBMode));
			}
		private:
			Vector3 m_vSize;	// �����蔻��T�C�Y
			bool m_bOBBMode;	// OBB�������邩

		public:
			CBoxCollision()
				:m_bOBBMode(true)
			{};
			CBoxCollision(std::shared_ptr<CGameObject> owner, Vector3 size = Vector3(1, 1, 1));
			~CBoxCollision();

			bool Box(Vector3 Apos, Vector3 Asize, Vector3 Bpos, Vector3 Bsize);
			bool CollisionAABB(Vector3 pos, Vector3 size);
			bool CollisionOBB(CTransform* trans, Vector3 center, Vector3 size);

			//--- �Q�b�^�[�E�Z�b�^�[
			inline Vector3 GetSize() { return m_vSize; }
			inline void SetSize(Vector3 size) { m_vSize = size; }
			inline bool IsOBB() { return m_bOBBMode; }
			inline void SetOBB(bool flg) { m_bOBBMode = flg; }

			void PosAdjustment(Vector3 pos, Vector3 size);
			bool HitCheckPtr(CCollision* col);		// �R���W�����׽���������ɂƂ��āA�����蔻����s��

#ifdef BUILD_MODE
		private:
			std::shared_ptr<CBox> m_pDebugBox;
		public:
			virtual void ImGuiDebug();
			void Update();
#endif // BUILD_MODE

		};
	}
}

CEREAL_REGISTER_TYPE(MySpace::Game::CBoxCollision)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, MySpace::CBoxCollision)

#endif // __BOX_COLLISION_H__
