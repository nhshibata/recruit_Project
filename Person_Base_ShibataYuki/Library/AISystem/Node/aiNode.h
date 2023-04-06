//========================================================
// [aiNode.h]
//-------------------
// 作成:2022/11/22
//-------------------
// AISystemに管理されるノード。の基底クラス
// 生成、解放もAISystemがやらなければならない
//========================================================

#ifndef __AI_NODE_H__
#define __AI_NODE_H__

//--- インクルード部
#include <memory>
#include <CoreSystem/Util/cerealCommon.h>

//--- 定数定義
// AID: AIDefine
#define AID_DEFINITION_ACTION					CAINode::EStatus Execute(AI::CAISystem* sys)
#define AID_IMPLEMENTATION_ACTION(type)			CAINode::EStatus type##::Execute(AI::CAISystem* sys)
#define AID_IMPLEMENTATION_AWAKE				void OnAwake(AI::CAISystem* sys)
#define AID_IMPLEMENTATION_START				void OnStart(AI::CAISystem* sys)

namespace AI
{
	//--- 前方宣言
	class CAISystem;

	class CAINode
	{
		friend class CAISystem;

#pragma region Serialize
	private:
		//--- シリアライズ
		friend class cereal::access;
		template<class Archive>
		void save(Archive& archive) const
		{
			archive(CEREAL_NVP(m_eStatus), CEREAL_NVP(m_nIndex)
			);
		}
		template<class Archive>
		void load(Archive& archive)
		{
			archive(CEREAL_NVP(m_eStatus), CEREAL_NVP(m_nIndex)
			);
		}
#pragma endregion

	public:
		//--- 列挙体
		// *@ノード状態
		enum class EStatus : int
		{
			NONE_ACTIVE = 0,	// 実行状態でない
			SUCCES,				// 成功
			FAILURE,			// 失敗
			RUNNING,			// 実行中
			//COMPLETED,			// 完了
			MAX,
		};
		//--- エイリアス
		using Ptr = std::shared_ptr<CAINode>;

	private:
		//--- メンバ変数
		EStatus m_eStatus = EStatus::NONE_ACTIVE;
		int m_nIndex;

	protected:
		//--- メンバ関数
		// *@状態設定
		// *@設定した状態を返す
		inline CAINode::EStatus SetStatus(const EStatus e) { m_eStatus = e; return m_eStatus; }
		inline void SetIndex(const int idx) { m_nIndex = idx; }
	public:
		// *@起動時処理
		virtual void OnAwake(CAISystem* sys) {};

		// *@開始時処理
		virtual void OnStart(CAISystem* sys) {};

		//// *@更新処理
		//virtual void OnUpdate(CAISystem* sys) = 0;

		//// *@終了処理
		//virtual void OnEnd(CAISystem* sys) = 0;

		// *@ノード実行関数
		virtual CAINode::EStatus Execute(CAISystem* sys) = 0;

		//--- ゲッター・セッター
		// *@登録インデックス取得
		_NODISCARD inline int GetIndex()const { return m_nIndex; }

		// *@状態取得
		_NODISCARD CAINode::EStatus GetStatus()const { return m_eStatus; }

		// *@ｱｸﾃｨﾌﾞ状態確認
		_NODISCARD bool IsActive()const { return m_eStatus != EStatus::NONE_ACTIVE; }
	};
}
#endif // DEBUG
