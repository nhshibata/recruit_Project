//=========================================================
// [hirarchy.h] 
// �쐬:2022/07/10
//---------------------------------------------------------
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __HIERARCHY_H__
#define __HIERARCHY_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/stl.h>
#include <CoreSystem/Math/myVector.h>

#if BUILD_MODE

#pragma region forward_declaration
namespace MySpace
{
	namespace Game
	{
		class CGameObject;
	}
	namespace Debug
	{
		class ImGuiManager;
	}
}
#pragma endregion
		
namespace MySpace
{
	namespace Debug
	{
		class CHierachy
		{
		private:
			//--- �񋓑̒�`
			enum class ESearchTerms : int	// ��������
			{
				OBJ_NAME,
				TAG,
				COMPONENT,
				STATE_ACTIVE,
				STATE_STOP,
				MAX
			};
			//--- �\���̒�`
			struct SearchSet				// �����Z�b�g
			{
				ESearchTerms eTerms = ESearchTerms::OBJ_NAME;
				bool bSearchCriteria = false;
				std::string inputName;
			};
			struct CreateWindowSet
			{
				bool bDisp;
				float fMargin;
				int nGrid;
				int nObjType;
				MyMath::Vector3 vCenter;
				CreateWindowSet():bDisp(false),fMargin(1),nGrid(3),nObjType(0) {}
			};
		private:
			static constexpr const char* DESC_SELECT_OBJ = u8"selectGameObjcet";	// *@�h���b�O���h���b�v�F���p������

			bool m_bLoadSaveWindow;
			std::string m_strSavePath;
			std::string m_strLoadPath;
			std::vector<std::string> m_aScenePathList;
			SearchSet m_Search;
			CreateWindowSet m_CreateValue;

		private:
			//--- �����o�֐�
			// *@�Z�[�u�ƃ��[�h�\��
			void DispSaveLoadMenu();
			
			// *@�����X�V
			void DispSearch();
			
			// *@���������ƈ�v���邩�m�F����
			bool DispCheck(MySpace::Game::CGameObject* obj);
			
			// *@�q�v�f�̕\��
			// *@���̕\���Ȃǂ��s�����߁A�ċA����
			void DispChild(MySpace::Debug::ImGuiManager* manager, std::weak_ptr<MySpace::Game::CGameObject> obj);

			// *@�I�u�W�F�N�g�ꊇ����
			void CreateObjectsWindow();
			std::shared_ptr<MySpace::Game::CGameObject> CreateObject(int No, std::shared_ptr<MySpace::Game::CGameObject> = std::shared_ptr<MySpace::Game::CGameObject>());


		public:
			CHierachy();
			~CHierachy();

			void Update(MySpace::Debug::ImGuiManager* mgr);

			void SetPath(std::string name) { m_strSavePath = name; };

			// *scene�t�@�C���̍Ď擾
			void LoadScenePathList();
		};
	}
}
#endif //BUILD_MODE

#endif // !__HIERARCHY_H__
