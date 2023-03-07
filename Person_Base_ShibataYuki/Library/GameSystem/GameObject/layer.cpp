//=========================================================
// [layer.h]
//---------------------------------------------------------
//作成:2022/04/19
//---------------------------------------------------------
// レイヤーｸﾗｽ : 部品ｸﾗｽ
//=========================================================

//--- インクルード部
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
		//--- 静的メンバ変数
		std::map<int, std::string> CLayer::m_aLayerMap = 
		{	
				{ 0, NAME_TO(E_Layer::DEFAULT) },
				{ 1, NAME_TO(E_Layer::UI) },
		};

		//==========================================================
		// コンストラクタ
		//==========================================================
		CLayer::CLayer()
		{
			m_nLayer = CLayer::NumberToBit(0);
		};

		//==========================================================
		// 引数付きコンストラクタ
		//==========================================================
		CLayer::CLayer(int layer) :m_nLayer(layer)
		{
		};

		//==========================================================
		// デストラクタ
		//==========================================================
		CLayer::~CLayer()
		{
		};
		
		//==========================================================
		// セッター
		// 上限決め
		//==========================================================
		void CLayer::SetLayer(int layer)
		{
			// 拒否
			if (layer >= 32 || layer < 0)
				return;
			m_nLayer = layer;
		};

		//==========================================================
		// 静的関数
		// 登録
		//==========================================================
		bool CLayer::Regist(int no, std::string registName)
		{
			// 32以上は入れない
			if (no >= 32 || no < 0)
				return false;

			m_aLayerMap[no] = registName;
			return true;
		}
		
		//==========================================================
		// 静的関数
		// Noから名前取得
		//==========================================================
		std::string CLayer::NumberToName(int No)
		{
			if (!m_aLayerMap.count(No))
				return std::string();
			return m_aLayerMap[No];
		}
		
		//==========================================================
		// 静的関数
		// No取得。格納順から一致する文字列を取得
		//==========================================================
		int CLayer::GetNumber(std::string registName)
		{
			for (auto & layer : m_aLayerMap)
			{
				// 文字列確認
				if (layer.second == registName)
				{	// keyを返す
					return layer.first;
				}
			}
			return -1;
		}
		
		//==========================================================
		// 静的関数
		// レイヤー番号をビット変換
		//==========================================================
		int CLayer::NumberToBit(int no)
		{
			// 32ビット以上は判定しない
			if (no >= 32)
				return -1;

			//--- ビットシフトした値を返す
			int ret = 0;
			ret = 1 << no;
			return ret;
		}

		//==========================================================
		// 静的関数
		// ロード
		//==========================================================
		void CLayer::LoadSystem()
		{
			CCerealize<std::map<int, std::string>> sirial;
			sirial.OutputFile("layerList", LAYER_PATH, m_aLayerMap);
		}

		//==========================================================
		// 静的関数
		// セーブ
		//==========================================================
		void CLayer::SaveSystem()
		{
			CCerealize<std::map<int, std::string>> sirial;
			m_aLayerMap = sirial.InputFile(LAYER_PATH);
		}
		

#ifdef BUILD_MODE
		//==========================================================
		// 静的関数
		// 名前取得
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
		// 静的関数
		// ImGui表示関数
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
				// 全て表示
				for (auto & layer : m_aLayerMap)
				{
					ImGui::Text("%d", layer.first);
					ImGui::SameLine();
					ImGui::Text("%s", layer.second);
				}

				//--- 追加する情報
				ImGui::InputInt("ID", &AddNo);
				ImGui::SameLine();
				InputLayer = Debug::InputString(InputLayer, "Add Layer");

				// 追加実行
				if (ImGui::Button("Applay"))
				{
					// 初期化
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
		// 静的関数
		//==========================================================
		int CLayer::ImGuiSetLayerList(int bit)
		{
			if (ImGui::BeginListBox("LayerMask"))
			{
				auto list = m_aLayerMap;
				for (auto & layer : list)
				{
					// 推された/選択された
					// and確認で選択中
					if (ImGui::Selectable(layer.second.c_str(), bit & layer.first))
					{
						// 数字をビットに変えて、XORで変更(0->1 or 1->0)
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