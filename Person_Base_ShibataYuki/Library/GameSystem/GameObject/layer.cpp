//=========================================================
// [layer.h]
//---------------------------------------------------------
//�쐬:2022/04/19
//---------------------------------------------------------
// ���C���[�׽ : ���i�׽
//=========================================================

//--- �C���N���[�h��
#include <GameSystem/GameObject/layer.h>
#include <CoreSystem/File/cerealize.h>
#include <DebugSystem/imGuiPackage.h>
#include <Application/screen.h>

using namespace MySpace::System;
using namespace MySpace::Debug;

namespace MySpace
{
	namespace Game
	{
		//--- �ÓI�����o�ϐ�
		std::map<int, std::string> CLayer::m_aLayerMap = 
		{	
				{ 0, NAME_TO(E_Layer::DEFAULT) },
				{ 1, NAME_TO(E_Layer::UI) },
		};

		//==========================================================
		// �R���X�g���N�^
		//==========================================================
		CLayer::CLayer()
		{
			m_nLayer = CLayer::NumberToBit(0);
		};

		//==========================================================
		// �����t���R���X�g���N�^
		//==========================================================
		CLayer::CLayer(int layer) :m_nLayer(layer)
		{
		};

		//==========================================================
		// �f�X�g���N�^
		//==========================================================
		CLayer::~CLayer()
		{
		};
		
		//==========================================================
		// �Z�b�^�[
		// �������
		//==========================================================
		void CLayer::SetLayer(int layer)
		{
			// ����
			if (layer >= 32 || layer < 0)
				return;
			m_nLayer = layer;
		};

		//==========================================================
		// �ÓI�֐�
		// �o�^
		//==========================================================
		bool CLayer::Regist(int no, std::string registName)
		{
			// 32�ȏ�͓���Ȃ�
			if (no >= 32 || no < 0)
				return false;

			m_aLayerMap[no] = registName;
			return true;
		}
		
		//==========================================================
		// �ÓI�֐�
		// No���疼�O�擾
		//==========================================================
		std::string CLayer::NumberToName(int No)
		{
			if (!m_aLayerMap.count(No))
				return std::string();
			return m_aLayerMap[No];
		}
		
		//==========================================================
		// �ÓI�֐�
		// No�擾�B�i�[�������v���镶������擾
		//==========================================================
		int CLayer::GetNumber(std::string registName)
		{
			for (auto & layer : m_aLayerMap)
			{
				// ������m�F
				if (layer.second == registName)
				{	// key��Ԃ�
					return layer.first;
				}
			}
			return -1;
		}
		
		//==========================================================
		// �ÓI�֐�
		// ���C���[�ԍ����r�b�g�ϊ�
		//==========================================================
		int CLayer::NumberToBit(int no)
		{
			// 32�r�b�g�ȏ�͔��肵�Ȃ�
			if (no >= 32)
				return -1;

			//--- �r�b�g�V�t�g�����l��Ԃ�
			int ret = 0;
			ret = 1 << no;
			return ret;
		}

		//==========================================================
		// �ÓI�֐�
		// ���[�h
		//==========================================================
		void CLayer::LoadSystem()
		{
			CCerealize<std::map<int, std::string>> sirial;
			sirial.OutputFile("layerList", LAYER_PATH, m_aLayerMap);
		}

		//==========================================================
		// �ÓI�֐�
		// �Z�[�u
		//==========================================================
		void CLayer::SaveSystem()
		{
			CCerealize<std::map<int, std::string>> sirial;
			m_aLayerMap = sirial.InputFile(LAYER_PATH);
		}
		

#ifdef BUILD_MODE
		//==========================================================
		// �ÓI�֐�
		// ���O�擾
		//==========================================================
		std::vector<std::string> CLayer::GetNameList()
		{
			std::vector<std::string> ret;
			for (auto & layer : m_aLayerMap)
			{
				ret.push_back(std::to_string(layer.first) + ":" + layer.second);
			}
			return ret;
		}

		//==========================================================
		// �ÓI�֐�
		// ImGui�\���֐�
		//==========================================================
		void CLayer::ImGuiLayer(bool& disp)
		{
			static std::string InputLayer;
			static int AddNo = 0;

			if (!disp)
				return;

			ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_::ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(CScreen::GetWidth() / 8, CScreen::GetHeight() / 8), ImGuiCond_::ImGuiCond_Once);
			if (ImGui::Begin("Layer Window", &disp))
			{
				// �S�ĕ\��
				for (auto & layer : m_aLayerMap)
				{
					ImGui::Text("%d", layer.first);
					ImGui::SameLine();
					ImGui::Text("%s", layer.second);
				}

				//--- �ǉ�������
				ImGui::InputInt("ID", &AddNo);
				ImGui::SameLine();
				InputLayer = Debug::InputString(InputLayer, "Add Layer");

				// �ǉ����s
				if (ImGui::Button("Applay"))
				{
					// ������
					AddNo = 0;
					InputLayer.clear();
					disp = false;
					if (CLayer::Regist(AddNo, InputLayer))
					{
						ImGui::Text("range error");
					}
				}

			}
		}

		//==========================================================
		// �ÓI�֐�
		//==========================================================
		int CLayer::ImGuiSetLayerList(int bit)
		{
			if (ImGui::BeginListBox("LayerMask"))
			{
				auto list = m_aLayerMap;
				for (auto & layer : list)
				{
					// �����ꂽ/�I�����ꂽ
					// and�m�F�őI��
					if (ImGui::Selectable(layer.second.c_str(), bit & layer.first))
					{
						// �������r�b�g�ɕς��āAXOR�ŕύX(0->1 or 1->0)
						bit ^= CLayer::NumberToBit(layer.first);
					}
				}
				ImGui::EndListBox();
			}
			return bit;
		}

#endif // BUILD_MODE

	}
}