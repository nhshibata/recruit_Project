//==========================================================
// [navMeshBake.h]
//----------------------------------------------------------
// �쐬:2022/12/05		�|�C���g���������i�f�o�b�O�p�̕\����
// �X�V:2022/12/06		�o�H�T��(A*)�Ń��[�g�\�z����,��Q���ݒ���m�F
// �X�V:2022/12/07		�A�E��i�ւ̈ڐA
//----------------------------------------------------------
// �i�r���b�V���\�z�p�׽
// �Ǘ����s���Ă��邽�߁A�����~�X��������...
//==========================================================

#ifndef __NAV_MESH_BAKE_H__
#define __NAV_MESH_BAKE_H__

//--- �C���N���[�h��
#include <unordered_map>
#include <vector>
#include <memory>
#include <CoreSystem/Math/MyMath.h>

//--- �萔��`
#define DEBUG_POINT_DRAW		_DEBUG | true		// *@�|�C���g�\���p�萔


#pragma region ForwardDeclaration
namespace MySpace
{
	namespace Graphics
	{
		class CSphere;
		class CBox;
	}
}
class AIMapOperator;
#pragma endregion


namespace AI
{
	using namespace MySpace::MyMath;
	using MySpace::Graphics::CSphere;
	using MySpace::Graphics::CBox;

	// *@�i�r���b�V���\�z�p�׽
	class CNavMeshBake
	{
	protected:
		//--- �\���̒�`
		struct Obstacle		// ��Q���ް�
		{
			Vector3 pos;
			Vector3 size;
			Obstacle(Vector3 p, Vector3 s) { pos = p; size = s; }
		};
		struct Point		// �z��Ǘ��p(�C���f�b�N�X,�Y���ȂǂɎg��)
		{
			int x;	
			int y;
			// �R���X�g���N�^
			Point() { x = y = 0; }
			Point(int x1, int y1) { x = x1; y = y1; }
			// ���Z�q�̃I�[�o�[���[�h
			bool operator==(Point other) { return x == other.x && y == other.y; }
			bool operator!=(Point other) { return !(*this == other); }
		};
	public:
		struct NodeData		// �m�[�h�������ް�
		{
			Vector3 pos;
			bool close = false;
			std::vector<int> surrounding;
			std::map<int, float> aHeightMap;
		};

	private:
		//--- �G�C���A�X
		using NavMap = std::unordered_map<int, NodeData>;		// �쐬�����m�[�h���i�[(Key��int�^��p����)
	public:
		using MapRoute = std::vector<Vector3>;					// �}�b�v���[�g�����n���p
		using Obstacles = std::vector<Obstacle>;				// ��Q���ް��i�[

	protected:
		//--- �����o�ϐ�
		NavMap m_aNavMap;						// Nav���
		Obstacles m_aObstacle;					// ��Q�����
		Vector3 m_vCenter = Vector3(0,0,0);		// ���S���W
		float m_fMargin;						// �O���b�h���̍�
		int m_nGrid;							// �O���b�h�c��

	protected:
		//--- ���ފ֐�
		// *@��Q���ǉ�
		void AddScore(const Vector3& pos, const Vector3& size);

	public:
		CNavMeshBake();
		CNavMeshBake(Vector3 pos, int grid, float margin);
		virtual ~CNavMeshBake();

		// *@�������֐�
		void Init();

		// *@�|�C���g��񐶐��Ăяo��
		// *@������:�����̎n�_, �I�_, �����鍂��
		virtual void Bake(const float startPos, const float endPos);


		// *@�O���b�h�O����
		inline bool IsOutOfPoint(Point index)const
		{
			return index.x < 0 || index.y < 0 || m_nGrid <= index.x || m_nGrid <= index.y;
		}

		// *@�C���f�b�N�X����xy�擾
		inline Point IndexToPoint(int index) { return Point{ index % m_nGrid, index / m_nGrid }; }
		
		// *@���W����xy��
		Point PosToPoint(const Vector3 pos)
		{
			float startX = m_vCenter.x - m_nGrid * m_fMargin * 0.5f; // m_margin * (m_grid - 1) * 0.5f - m_margin * 0.5f;
			float startZ = m_vCenter.z - m_nGrid * m_fMargin * 0.5f;
			return Point{
				static_cast<int>((pos.x - startX) / m_fMargin),
				static_cast<int>((pos.z - startZ) / m_fMargin)
			};
		}

		// *@xy����C���f�b�N�X�ϊ�
		inline int PointToIndex(Point point)const { return point.y * m_nGrid + point.x; }

		// *@xy������W�ϊ�
		Vector3 PointToPos(Point point)
		{
			float startX = m_vCenter.x - (m_nGrid - 1) * m_fMargin * 0.5f;
			float startZ = m_vCenter.z - (m_nGrid - 1) * m_fMargin * 0.5f;
			return Vector3(
				startX + point.x * m_fMargin,
				m_vCenter.y,
				startZ + point.y * m_fMargin
			);
		}

		// *@���W����}�b�v�̍��W�擾
		Vector3 PosToMapPos(Vector3 pos);

		// *@�}�b�v�ް����烋�[�g���\�z
		MapRoute GetRoute(Vector3 start, Vector3 end, AIMapOperator&& op);

		//--- �Q�b�^�[�E�Z�b�^�[

		// *@���S���W
		inline void SetPos(Vector3 pos) { m_vCenter = pos; }

		// *@�}�[�W��(��)
		inline void SetMargin(float value) { m_fMargin = value; }

		// *@��Q���ݒu
		void SetObstacle(Vector3 pos, Vector3 size) { m_aObstacle.push_back(Obstacle(pos, size)); }

#if BUILD_MODE

		void ImGuiDebug();

#endif // 0
		

#if DEBUG_POINT_DRAW
	protected:
		std::shared_ptr<CSphere> m_pDebugSphere;
		std::vector<std::shared_ptr<CBox>> m_aDebugBox;
		std::vector<int> m_aDebugRoute;
		Vector3 m_vDebugStart;
		Vector3 m_vDebugEnd;
		float m_fDebugHeight = (1.0f);
		Vector2 m_vLine = Vector2(1000, -1000);

	private:
		bool IsRoute(int idx);

	public:
		// *@�|�C���g�`��
		void Draw();
#endif // DEBUG_POINT_DRAW

	};
}

#endif // !__NAV_MESH_BAKE_H__