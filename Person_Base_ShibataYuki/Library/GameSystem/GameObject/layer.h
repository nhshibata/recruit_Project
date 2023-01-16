//=========================================================
// [layer.h]
//---------------------------------------------------------
//�쐬:2022/04/19
//---------------------------------------------------------
// ���C���[�׽ : ���i�׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __LAYER_H__
#define __LAYER_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/cerealCommon.h>
#include <string>

#ifndef NAME_TO
#define NAME_TO(name)		#name
#endif // NAME_TO
#define MAP_SET(key, value)	static_cast<int>(key), NAME_TO(value)

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
			enum class E_Layer : int	// ���C���[�𑝂₷
			{
				SKY = 0, 
				BG = 1,			// �w�i
				DEFAULT = 10,	// �f�t�H���g
				MODEL = 30,
				UI = 50,		// UI
				FOG = 99,		// �O�i(�t�F�[�h��)
			};

		private:
			//--- �����o�ϐ�
			int m_nLayer;

			// �o�^�p�ÓI�����o�ϐ�
			static std::map<int, std::string> m_aNoList;

		public:
			//--- �����o�֐�
			CLayer() :m_nLayer(static_cast<int>(E_Layer::DEFAULT)) {};
			CLayer(E_Layer layer) :m_nLayer((int)layer) {};
			~CLayer() {};

			//--- �Q�b�^�[�E�Z�b�^�[
			inline int* GetLayer() { return &m_nLayer; };
			inline void SetLayer(int layer) { m_nLayer = layer; };
			inline void SetLayer(E_Layer layer) { m_nLayer = static_cast<int>(layer); };

			static void Regist(int no, std::string registName)
			{
				if(!m_aNoList.count(no))
					m_aNoList[no] = registName;
			}

		};


	}
}

#endif // !__LAYER_H__
