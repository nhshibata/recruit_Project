//=========================================================
// 作成:2022/04/19
// 更新:2022/05/11 親子関係の実装。ローカル座標用のｸﾗｽ作成
// トランスフォーム : 部品ｸﾗｽ
//=========================================================

//--- インクルードガード
#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

//--- インクルード部
#include <GameSystem/Component/component.h>
#include <GameSystem/GameObject/gameObject.h>
#include <vector>

#pragma region ForwardDeclaration
namespace MySpace
{
	using namespace MySpace::MyMath;

	namespace Game
	{
		class CGameObject;
	}
}
#pragma endregion

namespace MySpace
{
	namespace Game
	{
		using MySpace::Game::CGameObject;

		//--- クラス定義
		class CTransform : public CComponent
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("transform", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_vPos), CEREAL_NVP(m_vOldPos), CEREAL_NVP(m_vRot),
					CEREAL_NVP(m_vDestRot), CEREAL_NVP(m_vScale),
					/*CEREAL_NVP(m_mWorldMtx),CEREAL_NVP(m_mLocalMtx),*/CEREAL_NVP(m_Rot)
					/*CEREAL_NVP(m_pChilds), CEREAL_NVP(m_pParent),*/
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("transform", cereal::base_class<CComponent>(this)),
					CEREAL_NVP(m_vPos), CEREAL_NVP(m_vOldPos), CEREAL_NVP(m_vRot),
					CEREAL_NVP(m_vDestRot), CEREAL_NVP(m_vScale),
					/*CEREAL_NVP(m_mWorldMtx),CEREAL_NVP(m_mLocalMtx),*/ CEREAL_NVP(m_Rot)
					/*CEREAL_NVP(m_pChilds), CEREAL_NVP(m_pParent),*/
				);
			}
		private:
			//--- メンバ変数
			Vector3 m_vPos;			// 座標
			Vector3 m_vOldPos;		// 過去座標
			Vector3 m_vRot;			// 角度
			Vector3 m_vDestRot;		// 目標角度
			Vector3 m_vScale;		// 大きさ
			Matrix4x4 m_mWorldMtx;	// ワールドマトリックス
			Matrix4x4 m_mLocalMtx;	// ローカルマトリックス
			Quaternion m_Rot;		// クォータニオン

		protected:
			std::weak_ptr<CTransform> m_pParent;					// 親オブジェクト
			std::vector<std::weak_ptr<CTransform>> m_pChilds;		// 子要素リスト
		
		private:
			//--- メンバ関数
			void UpdateChildMatrix(CTransform* ptr, Matrix4x4 mtx);

		public:
			CTransform();
			CTransform(std::shared_ptr<CGameObject> owner);
			~CTransform();

			// *@生成時呼び出し
			virtual void Awake() {};
			void Init();
			virtual void Uninit();
			void Update();
			void UpdateWorldMatrix();

			//--- ゲッター・セッター
			_NODISCARD inline Vector3 GetPos()		{ return m_vPos; };
			_NODISCARD inline Vector3 GetRot()		{ return m_vRot; };
			_NODISCARD inline Vector3 GetScale()	{ return m_vScale; };
			_NODISCARD inline Vector3 GetOldPos()	{ return m_vOldPos; };
			
			Quaternion GetWorldQuaternion();
			Quaternion GetLocalQuaternion();
			_NODISCARD inline XMFLOAT4X4 GetWorldMatrix() { return m_mWorldMtx; }
			_NODISCARD inline XMFLOAT4X4 GetLocalMatrix() { return m_mLocalMtx; }

			inline void SetPos(Vector3 value)					{ m_vPos = value; };
			inline void SetRot(Vector3 value)					{ m_vRot = value; };
			inline void SetDestRot(Vector3 value)				{ m_vDestRot = value; };
			inline void SetScale(Vector3 value)					{ m_vScale = value; };
			inline void SetWorldQuaternion(const Quaternion & rotation);
			inline void SetLocalQuaternion(const Quaternion & rotation);
			void SetWorldMatrix(Vector3 translate, Vector3 rot, Vector3 scale);
			inline void SetWorldMatrix(const XMFLOAT4X4 value)	{ m_mWorldMtx = value; }
			inline void SetLocalMatrix(const XMFLOAT4X4 value)	{ m_mLocalMtx = value; }

			//--- 親子関係
			// *@親オブジェクト取得
			_NODISCARD inline std::weak_ptr<CTransform> GetParent()		{ return m_pParent; };
			// *@子オブジェクト取得
			_NODISCARD inline 
				std::vector<std::weak_ptr<CTransform>> GetChilds()		{ return m_pChilds; }
			// *@子オブジェクト取得
			_NODISCARD std::weak_ptr<CTransform> GetChild(int no);
			// *@要素数を返す
			_NODISCARD inline int GetChildCount()						{ return static_cast<int>(m_pChilds.size()); };

			// *@親要素追加
			inline void SetParent(std::weak_ptr<CTransform> parent)		{ m_pParent = parent; }
			// *@子要素追加
			void AddChild(std::weak_ptr<CTransform> child);
			// *@子要素除外
			void RemoveChild(std::weak_ptr<CTransform> child)
			{
				for (auto it = m_pChilds.begin(); it != m_pChilds.end(); ++it)
				{
					if ((*it).lock() == child.lock())
					{
						m_pChilds.erase(it); 
						break;
					}
				}
			}
			// *@親子関係解消
			void ParentDissolved()
			{
				if (!m_pParent.lock())
					return;
				m_pParent.lock()->RemoveChild(BaseToDerived<CTransform>());
				m_pParent.reset();
			}

			//--- オブジェクト向き取得
			// *@前方
			inline Vector3 forward()
			{
				DirectX::XMFLOAT4X4 mtx;
				DirectX::XMStoreFloat4x4(&mtx,
					DirectX::XMMatrixRotationRollPitchYaw(
						DirectX::XMConvertToRadians(m_vRot.x),
						DirectX::XMConvertToRadians(m_vRot.y),
						DirectX::XMConvertToRadians(m_vRot.z)
					));

				return Vector3(mtx._31, mtx._32, mtx._33).Normalize();
			}
			// *@右方
			inline Vector3 right()
			{
				DirectX::XMFLOAT4X4 mtx;
				DirectX::XMStoreFloat4x4(&mtx,
					DirectX::XMMatrixRotationRollPitchYaw(
						DirectX::XMConvertToRadians(m_vRot.y + 90.0f),
						DirectX::XMConvertToRadians(m_vRot.x),
						DirectX::XMConvertToRadians(m_vRot.z)
					));

				return Vector3(mtx._31, mtx._32, mtx._33).Normalize();
			}
			// *@ 上方
			inline Vector3 up()
			{
				return Vector3::Normalize(Vector3::Cross(forward(), right()));
			}

#ifdef BUILD_MODE

			virtual void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}
CEREAL_REGISTER_TYPE(MySpace::Game::CTransform)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(MySpace::CComponent, MySpace::CTransform)

#endif // !__TRANSFORM_H__

