//=========================================================
// [BaseObject.cpp]
// ���N���X
// �쐬:2022/06/07
// �X�}�[�g�|�C���^���g���₷�����邽�߂ɍ쐬�����׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __BASEOBJECT_H__
#define __BASEOBJECT_H__

//--- �C���N���[�h��
#include <typeinfo>
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Util/cerealCommon.h>
#include <DebugSystem/debug.h>

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
			//--- �Ē�`
			using Ptr = std::weak_ptr<T>;
		private:
			std::string m_objName = std::string();		// ���O
			Ptr m_spThisPtr;			// ���g�̃X�}�[�g�|�C���^

		public:
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

			// <Summary>
			// NOTE: �I�u�W�F�N�g�A�R���|�[�l���g�������Ƀ|�C���^��n���Ȃ���΂����Ȃ�
			// </Summary>
			void SetPtr(Ptr ptr) { m_spThisPtr = ptr; }

			// <Summary>
			// weakPtr�̎擾
			// </Summary>
			Ptr GetPtr() { return m_spThisPtr; }

			// <Summary>
			// weakPtr����
			// �z��ɂ��邩�m�F
			// </Summary>
			template <class T2, class S>
			bool IsPtrIn(T2 ptr, S obj) {
				auto it = ptr.begin();
				for (; it != ptr.end(); ++it)
				{
					if ((*it) == obj) { return true; }
				}
				return false;
			}

			// <Summary>
			// list����
			// </Summary>
			template <class T1>
			bool IsPtrIn(std::list<T1> ptr, T1 obj) {
				auto it = ptr.begin();
				for (; it != ptr.end(); ++it)
				{
					if ((*it).lock() == obj.lock()) { return true; }		// weak
					if ((*it) == obj) { return true; }					// shared
				}
				return false;
			}

#ifdef BUILD_MODE
		protected:
			//bool m_bDebug;
		public:
			// �f�o�b�O�֌W
			virtual void ImGuiDebug() {};
			//void SetDebug(bool flg) { m_bDebug = flg; }
			//bool GetDebug() { return m_bDebug; }
			//bool SwitchDebug() { m_bDebug ^= true; return m_bDebug; }
			//bool* GetDebugAdress() { return &m_bDebug; }
			//static void SetScene(const const CScene* pScene) { m_pScene = pScene; }
#endif // BUILD_MODE
		};
	}
}

#endif // !#define __BASEOBJECT_H__

