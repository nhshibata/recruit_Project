//=========================================================
// �쐬:2022/04/21
// Factory���\�b�h
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __COMPONENT_FACTORY_H__
#define __COMPONENT_FACTORY_H__

//--- �C���N���[�h��
//#include <Application/main.h>
#include <map>
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		//--- �O���Q��
		class CGameObject;
		class CComponent;
	}
}

namespace MySpace
{
	namespace Game
	{
		//--- �N���X��`
		class CComponentFactory
		{
		public:
			//--- ���ފ֐�
			CComponentFactory();
			~CComponentFactory() {};

			// *@�I�u�W�F�N�g�ɺ��߰��Ă�ǉ�
			// *@�ʃN���X�œo�^���ꂽ���߰��Ă𕶎��񂩂琶������
			static bool ObjSetComponent(CGameObject& obj, std::string name);

			// *@���߰��Ė��S�擾
			static std::vector<std::string> GetNameList();

			// *@���O������߰��Ď擾
			//static std::shared_ptr<CComponent> CreateComponent(std::string name);

		/*	template <class T>
			static T* CreateComponent();

			template <class T>
			static std::shared_ptr<T> MakeComponent();*/
		};
	}
}
#endif // !__COMPONENT_FACTORY_H__

