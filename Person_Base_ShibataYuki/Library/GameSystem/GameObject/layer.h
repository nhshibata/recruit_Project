//=========================================================
// [layer.h]
//---------------------------------------------------------
// �쐬:2022/04/19
// �X�V:2023/02/14 �r�b�g�ɑΉ��ł���悤�ύX
//---------------------------------------------------------
// ���C���[�׽ : ���i�׽
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __LAYER_H__
#define __LAYER_H__

//--- �C���N���[�h��
#include <CoreSystem/Util/cerealCommon.h>
#include <string>

//--- �萔��`
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
		private:
			//--- �����o�ϐ�
			int m_nLayer;

			// �o�^�p�ÓI�����o�ϐ�
			static std::map<int, std::string> m_aLayerMap;

		public:
			//--- �����o�֐�
			CLayer();
			CLayer(int layer);
			~CLayer();

			//--- �Q�b�^�[�E�Z�b�^�[
			inline int GetLayer() { return m_nLayer; }
			inline std::string GetName() { return NumberToName(m_nLayer); }
			void SetLayer(int layer);
			void SetLayer(std::string layer);

			// *@�ÓI�����o�֐�
			// *@�V�K�o�^
			static bool Regist(int no, std::string registName);
			// *@�ÓI�����o�֐�
			// *@���O�𐔎���
			static int GetNumber(std::string name);
			// *@�ÓI�����o�֐�
			// *@�����𖼑O��
			static std::string NumberToName(int No);
			// *@�ÓI�����o�֐�
			// *@�������i���ϊ�
			static int NumberToBit(int no);
			// *@�ÓI�����o�֐�
			// *@�t�@�C���ǂݍ���
			static void LoadSystem();
			// *@�ÓI�����o�֐�
			// *@�t�@�C�������o��
			static void SaveSystem();

#ifdef BUILD_MODE
			static std::vector<std::string> GetNameList(bool bId = false);
			static void ImGuiLayer(bool& disp);
			static int ImGuiSetLayerList(int bit);
#endif // BUILD_MODE


		};


	}
}

#endif // !__LAYER_H__
