//=========================================================
// [rigidbody.cpp]
//---------------------------------------------------------
// 作成:2022/05/24
//---------------------------------------------------------
// 物理
//=========================================================

//--- インクルードガード
#ifndef __RIGIDBODY_COMPONENT_H__
#define __RIGIDBODY_COMPONENT_H__

//---- インクルード部
#include <GameSystem/Component/component.h>

namespace MySpace
{
	namespace Game
	{
		//--- クラス定義
		class CRigidbody : public CComponent
		{
		private:
			// シリアライズ
			friend class cereal::access;
			template<class Archive>
			void save(Archive& archive) const
			{
				archive(cereal::make_nvp("RigidbodyComponent", cereal::base_class<CBaseComponent>(this))/*,
					CEREAL_NVP(m_bGravity), CEREAL_NVP(m_bIsSleep), CEREAL_NVP(m_bCollisionDetectionMode),
					CEREAL_NVP(m_fResistance)*/
				);
			}
			template<class Archive>
			void load(Archive& archive)
			{
				archive(cereal::make_nvp("RigidbodyComponent", cereal::base_class<CBaseComponent>(this))/*,
					CEREAL_NVP(m_bGravity), CEREAL_NVP(m_bIsSleep), CEREAL_NVP(m_bCollisionDetectionMode),
					CEREAL_NVP(m_fResistance)*/
				);
			}
		private:
			//--- 軸固定構造体
			struct FixedVector3
			{
				// シリアライズ
				friend class cereal::access;
				template<class Archive>
				void serialize(Archive & archive)
				{
					archive(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(z),
							CEREAL_NVP(bX), CEREAL_NVP(bY), CEREAL_NVP(bZ)
					);
				}
				//--- メンバ変数
				float x, y, z;
				bool bX, bY, bZ;

				//--- メンバ関数
				FixedVector3()
				{
					x = y = z = 0;
					bX = bY = bZ = false;
				};
				void Fixed(bool pX, bool pY, bool pZ, Vector3 vec)
				{
					if (pX) { bX = true; x = vec.x; }
					if (pY) { bY = true; y = vec.y; }
					if (pZ) { bZ = true; z = vec.z; }
				}
				void Fixed(Vector3 vec)
				{
					if (bX) { x = vec.x; }
					if (bX) { y = vec.y; }
					if (bX) { z = vec.z; }
				}
				void Fix(Vector3 vec)
				{
					if (bX) vec.x = x; 
					if (bY) vec.y = y; 
					if (bZ) vec.z = z; 
				}
			};

		private:
			//--- ﾒﾝﾊﾞ変数
			bool m_bGravity;			// 重力フラグ
			bool m_bIsSleep;			//
			//bool m_bCollisionDetectionMode;		// 当たり判定を行うか
			float m_fResistance;		// 抵抗
			float m_fGravity;			// 重力
			float m_fMass;				// 質量

			Vector3 m_vTargetPos;		// 目標座標
			Vector3 m_vVel;				// 移動量
			Vector3 m_vForce;			// 力
			Vector3 m_vAccel;			// 加速
			FixedVector3 m_pFreezPos;	// 座標固定
			FixedVector3 m_pFreezRot;	// 回転固定

		public:
			//--- メンバ関数
			CRigidbody();
			CRigidbody(std::shared_ptr<CGameObject> owner);
			~CRigidbody();
			
			// *@生成時呼び出し
			virtual void Awake() {};
			void FixedUpdate();

			void OnCollisionEnter(CGameObject * obj);

			//--- ゲッター・セッター
			_NODISCARD inline Vector3 GetVel() { return m_vVel; };
			_NODISCARD inline Vector3 GetAccel() { return m_vAccel; };

			inline void SetGravity(const bool value) { m_bGravity = value; }
			inline void SetTargetPos(Vector3 value) { m_vTargetPos = value; };
			inline void SetVel(Vector3 value) { m_vVel = value; };
			inline void SetAccel(Vector3 value) { m_vAccel = value; };
			// 位置・角度固定
			void SetFreezPos(const bool x, const bool y, const bool z);
			void SetFreezRot(const bool x, const bool y, const bool z);
			inline void SetForce(Vector3 value) { m_vForce = value; }

			_NODISCARD inline bool UseGravity() { return m_bGravity; }
			_NODISCARD inline bool IsSleeping() { return m_bIsSleep; }
			inline void AddForce(Vector3 add) { m_vForce += add; }

#ifdef BUILD_MODE

			void ImGuiDebug();

#endif // BUILD_MODE

		};
	}
}
//CEREAL_REGISTER_TYPE(MySpace::Game::CRigidbody)

#endif // !__RIGIDBODY_H__
