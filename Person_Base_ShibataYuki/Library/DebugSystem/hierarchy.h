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
			struct STCreateWindowSet
			{
				bool bDisp;				 // �\���t���O
				bool bCopy;				 // ����
				float fMargin;			 // �}�[�W��
				int nGrid;				 // �c�~��
				int nObjType;			 // �����łȂ��ꍇ�̐���Type(cpp�Œ�`)
				MyMath::Vector3 vCenter; // ���S
				std::string strName;	 // ���O
				STCreateWindowSet():bDisp(false),bCopy(true),fMargin(1),nGrid(3),nObjType(0) 
				{}
			};
		public:
			static constexpr const char* DESC_SELECT_OBJ = u8"selectGameObjcet";	// *@�h���b�O���h���b�v�F���p������
		private:

			bool m_bLoadSaveWindow;
			std::string m_strSavePath;
			std::string m_strLoadPath;
			std::vector<std::string> m_aScenePathList;
			SearchSet m_Search;
			STCreateWindowSet m_CreateParam;

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
			std::shared_ptr<MySpace::Game::CGameObject> CreateObject(const int No, std::shared_ptr<MySpace::Game::CGameObject> = std::shared_ptr<MySpace::Game::CGameObject>());

			// *@�N���b�N�ŃI�u�W�F�N�g��I��
			void ClickSelect(MySpace::Debug::ImGuiManager* manager);

		public:
			CHierachy();
			~CHierachy();

			void Update(MySpace::Debug::ImGuiManager* mgr);

			void SetPath(const std::string name) { m_strSavePath = name; };

			// *scene�t�@�C���̍Ď擾
			void LoadScenePathList();
		};
	}
}
#endif //BUILD_MODE

#endif // !__HIERARCHY_H__
