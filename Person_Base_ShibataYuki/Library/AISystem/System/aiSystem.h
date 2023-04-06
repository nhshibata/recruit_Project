//========================================================
// [AISystem.h]
//------------------
// 作成:2022/11/22
//------------------
// 基本的なビヘイビア
// ノードの追加順で優先度が決まる
// ノードのノードだと子ノードが優先される
// NodeにはConditionとなる条件を追加する
//（複数ある場合、全ての条件を満たしていなければノードは実行されない）
//========================================================

#ifndef __AI_SYSTEM_H__
#define __AI_SYSTEM_H__

//--- インクルード部
#include <map>
#include <memory>
#include <AISystem/Node/aiNode.h>
#include <GameSYstem/Component/component.h>

//--- 前方宣言
namespace MySpace
{
	namespace Game
	{
		class CGameObject;
	}
}

namespace AI
{
	class CAINode;
}
	
namespace AI
{
	using MySpace::Game::CGameObject;
	
	class CAISystem : public CComponent
	{
#pragma region Serialize
	private:
		//--- シリアライズ
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(cereal::make_nvp("AISystemComponent", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_pRoot), CEREAL_NVP(m_Nodes)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(cereal::make_nvp("AISystemComponent", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_pRoot), CEREAL_NVP(m_Nodes)
			);
		}
#pragma endregion
	private:
		//--- エイリアス
		using NodePtr = std::shared_ptr<CAINode>;
		using NodeMap = std::map<int, NodePtr>;
		
	protected:
		//--- メンバ変数
		NodePtr m_pRoot;			// 先頭ノード
		NodeMap m_Nodes;			// ノードを一括で管理する
		void* m_pData;				// (解放の責任を持つ)

	public:
		//--- メンバ関数
		CAISystem();
		CAISystem(std::shared_ptr<CGameObject> ptr);
		~CAISystem();
		
		// *@生成時呼び出し
		virtual void Awake() {};
		// *@初期化
		virtual void Init();

		void Update();

		// *@ノードの更新
		// *@戻り値はノードの結果を返す(値は列挙体であるCAINode::EStatus基準)
		virtual int NodeUpdate();

		// *@所持ﾎﾟｲﾝﾀ変数をvoid*からT型へcastする
		template<class T>
		inline T* ConvertData()
		{
			T* ret = reinterpret_cast<T*>(m_pData);
			return ret;
		}

		//--- ノード関連
		// *@ ノード追加
		CAISystem::NodePtr AddNode(NodePtr node);

		// *@ノード生成関数
		// *@内部でID番号生成を行い、振り分ける
		template<class T>
		_NODISCARD std::shared_ptr<T> MakeNode()
		{
			int idx = -1;
			do{
				idx = rand() % 1000;
				if (!m_Nodes.count(idx))
					break;
			} while (true);

			auto node = std::make_shared<T>();
			node->SetIndex(idx);
			AddNode(node);
			return node;
		}

		//--- ゲッター・セッター

		// *@ノード取得
		// *@引き数:振り分られた一意なインデックス番号
		// *@登録されていないインデックスが渡された場合、空のSPを返す
		inline NodePtr GetNode(int idx) { if (!m_Nodes.count(idx)) { return NodePtr(); } return m_Nodes[idx]; }

		// *@登録したインデックスからSP取得
		inline NodePtr GetSmartPtr(int idx) { if (!m_Nodes.count(idx))NodePtr(); return m_Nodes[idx]; }

		// *@型からノードを取得
		template<class T>
		_NODISCARD std::shared_ptr<T> GetNode()
		{
			std::shared_ptr<T> ret = std::shared_ptr<T>();
			for (auto & node : m_Nodes)
			{
				ret = std::dynamic_pointer_cast<T>(node);
				if (ret)
					return ret;
			}
			return std::shared_ptr<T>();
		}

		// *@実行ノードの設定
		inline void SetRoot(NodePtr node) { m_pRoot = node; }

		inline void SetData(void* data) { m_pData = data; }

#if _DEBUG
	public:
		int m_nNodeCnt = 0;
#endif // _DEBUG

	};
}

CEREAL_REGISTER_TYPE(AI::CAISystem)

#endif // !__AI_SYSTEM_H__
