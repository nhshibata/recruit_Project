//==========================================================
// [navMeshHeight.h]
//----------------
// �쐬:2022/12/28
//----------------
// �����p
// �ЂƂ܂�����
//==========================================================

#ifndef __NAV_MESH_HEIGHT_H__
#define __NAV_MESH_HEIGHT_H__

//--- �C���N���[�h��
#include <AISystem/Nav/navMeshBake.h>

namespace AI
{
	class CNavMeshHeight : public CNavMeshBake
	{
	private:
		//--- �����o�ϐ�
		float m_fHeight;

	private:
		// *@�|�C���g�Ԃ̋����擾�֐�
		float CalcDist(Vector3 a, Vector3 b);
		// *@�E�F�C�g�l�v�Z
		float CalcStepWeight(Vector3 a, Vector3 b, float height);

	public:
		//--- �����o�֐�
		CNavMeshHeight();
		CNavMeshHeight(Vector3 pos, int grid, float margin, float height);
		~CNavMeshHeight();

		void Bake(const float startPosY, const float endPosY);

		// *@�}�b�v�ް����烋�[�g���\�z
		MapRoute GetRoute(Vector3 start, Vector3 end, float height);

		inline void SetHeight(const float height) { m_fHeight = height; }
	};

}

#endif // !__NAV_MESH_HEIGHT_H__


