//=========================================================
//�쐬:2022/04/19 (�Ηj��)
// ���C���[�׽ : ���i�׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __LAYER_H__
#define __LAYER_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/cerealCommon.h>

namespace MySpace
{
	namespace Game
	{
		class CLayer
		{
		private:
			//--- �V���A���C�Y
			friend class cereal::access;
			template<class Archive>
			void save(Archive & archive) const
			{
				archive(CEREAL_NVP(m_nLayer));
			}
			template<class Archive>
			void load(Archive & archive)
			{
				archive(CEREAL_NVP(m_nLayer));
			}
		public:
			//--- �񋓑̒�`
			enum class E_Layer	// ���C���[�𑝂₷
			{
				SKY = 0, 
				BG = 1,			// �w�i
				DEFAULT = 2,	// �f�t�H���g
				MODEL = 30,			// UI
				UI = 50,			// UI
				FOG = 99,		// �O�i(�t�F�[�h��)
			};
		private:
			int m_nLayer;
		public:
			CLayer() :m_nLayer(static_cast<int>(E_Layer::DEFAULT)) {};
			CLayer(E_Layer layer) :m_nLayer((int)layer) {};
			~CLayer() {};
			inline int* GetLayer() { return &m_nLayer; };
			inline void SetLayer(int layer) { m_nLayer = layer; };
			inline void SetLayer(E_Layer layer) { m_nLayer = static_cast<int>(layer); };
		};
	}
}

#endif // !__LAYER_H__
