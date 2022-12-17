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
				STATE_DESTROY,
				MAX
			};
			//--- �\���̒�`
			struct SearchSet				// �����Z�b�g
			{
				ESearchTerms eTerms = ESearchTerms::OBJ_NAME;
				bool bSearchCriteria = false;
				std::string inputName;
			};
		private:
			static constexpr const char* DESC_SELECT_OBJ = u8"selectGameObjcet";	// *@�h���b�O���h���b�v�F���p������

			bool m_bLoadSaveWindow;
			SearchSet m_Search;
			std::string m_savePath;
			std::string m_loadPath;
			std::vector<std::string> m_scenePathList;

		private:
			//--- �����o�֐�
			// *@�Z�[�u�ƃ��[�h�\��
			void DispSaveLoadMenu();
			// *@�����X�V
			void DispSearch();
			// *@���������ƈ�v���邩�m�F����
			bool DispCheck(MySpace::Game::CGameObject* obj);
			// *@���X�g���̈ړ��p�֐�
			template<class T>
			_NODISCARD std::list<T> MovingInList(std::list<T> list, T newT, int index);
			// *@�q�v�f�̕\��
			// *@���̕\���Ȃǂ��s�����߁A�ċA����
			void DispChild(MySpace::Debug::ImGuiManager* manager, std::weak_ptr<MySpace::Game::CGameObject> obj);

		public:
			CHierachy();
			~CHierachy();

			void Init();
			void Uninit();
			void Update(MySpace::Debug::ImGuiManager*);

			void SetPath(std::string name) { m_savePath = name; };
			// *scene�t�@�C���̍Ď擾
			void LoadScenePathList();
		};
	}
}
#endif //BUILD_MODE

#endif // !__HIERARCHY_H__
