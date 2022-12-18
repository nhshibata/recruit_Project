//=========================================================
// [BaseObject.cpp]
//---------------------
// �쐬:2022/06/07
//---------------------
// ���N���X
// �X�}�[�g�|�C���^���g���₷�����邽�߂ɍ쐬�����׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __BASEOBJECT_H__
#define __BASEOBJECT_H__

//--- �C���N���[�h��
#include <typeinfo>
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		template <class T>
		class CBaseObject
		{
		private:
			// �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(m_objName), CEREAL_NVP(m_spThisPtr));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(m_objName), CEREAL_NVP(m_spThisPtr));
			}
		public:
			//--- �G�C���A�X
			using Ptr = std::weak_ptr<T>;

		protected:
			//--- �����o�ϐ�
			std::string m_objName = std::string();	// ���O
		private:
			Ptr m_spThisPtr;						// ���g�̃X�}�[�g�|�C���^

		public:
			//--- �����o�֐�
			CBaseObject() : m_objName(typeid(*this).name()) {};
			CBaseObject(const CBaseObject & obj)  
			{
				this->m_objName = obj.m_objName;
			};
			~CBaseObject() {};

			// *@�j�������یĂяo�����
			virtual void Uninit() {}

			std::string GetName() { return m_objName; }
			void SetName(std::string name) { m_objName = name; };
			type_info& GetType() { return typeid(*this); }

			
			// NOTE: �I�u�W�F�N�g�A�R���|�[�l���g�������Ƀ|�C���^��n���Ȃ���΂����Ȃ�
			// *@SP�ݒ�
			void SetPtr(Ptr ptr) { m_spThisPtr = ptr; }

			// *@weakPtr�擾
			Ptr GetPtr() { return m_spThisPtr; }

			// *@weakPtr����
			// *@�z��ɂ��邩�m�F
			/*template <class T2, class S>
			bool IsPtrIn(T2 ptr, S obj) {
				auto it = ptr.begin();
				for (; it != ptr.end(); ++it)
				{
					if ((*it) == obj) { return true; }
				}
				return false;
			}*/

			// *@list����
			//template <class T1>
			//bool IsPtrIn(std::list<T1> ptr, T1 obj) {
			//	auto it = ptr.begin();
			//	for (; it != ptr.end(); ++it)
			//	{
			//		if ((*it).lock() == obj.lock()) { return true; }		// weak
			//		if ((*it) == obj) { return true; }					// shared
			//	}
			//	return false;
			//}

#ifdef BUILD_MODE
		public:
			// �f�o�b�O�֌W
			virtual void ImGuiDebug() {};
#endif // BUILD_MODE
		};
	}
}

#endif // !#define __BASEOBJECT_H__

