//========================================================
// [stageBuild]
//------------------------
// �쐬:2023/01/02
//------------------------
//
//========================================================

#ifndef __STAGE_MANAGER_COMPONENT_H__
#define __STAGE_MANAGER_COMPONENT_H__

//--- �C���N���[�h��
#include <GameSystem/Component/component.h>
#include <CoreSystem/File/resourceCSV.h>

namespace Spell
{

	class CStageManager : public CComponent
	{
	public:
		struct GridPoint
		{
			int x, y;
			GridPoint() 
			{
				x = y = 0;
			};
			GridPoint(int x1, int y1)
			{
				x = x1;
				y = y1;
			};
		};

	private:
		CResourceCSV* m_pCSV;	// csv�ǂݍ��݌��ێ�
		Vector3 m_vCenter;		// ���S���W
		float m_fMargin;		// �O���b�h���̍�

	public:
		CStageManager();
		CStageManager(CGameObject::Ptr);
		~CStageManager();

		void Awake();

		void StartBuild(std::string file);

		// *@stageData���擾(Vector3)
		int GetData(const Vector3 pos);
		
		// *@stageData���擾(int)
		int GetData(const int idx);
		
		// *@�C���f�b�N�X����xy�擾
		GridPoint IndexToPoint(int index);

		// *@���W����xy��
		GridPoint PosToPoint(const Vector3 pos);

		// *@xy����C���f�b�N�X�ϊ�
		int PointToIndex(GridPoint point)const;

		// *@xy������W�ϊ�
		Vector3 PointToPos(GridPoint point);
		
	};

}

#endif //!__STAGE_MANAGER_COMPONENT_H__