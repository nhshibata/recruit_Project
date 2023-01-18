//==========================================================
// [Tween.h]
// 2022/06/07 作成
//----------------------------------------------------------
// 作ったはいいものの、使うか不明
// Transformに持たせるか、Componentの更新後に呼び出すべきか
//==========================================================

//--- インクルードガード
#ifndef __TWEEN_H__
#define __TWEEN_H__

//--- インクルード部
#include <GameSystem/Component/Transform/transform.h>
#include <CoreSystem/Math/MyMath.h>
#include <memory>

//namespace MySpace
//{
//	namespace TWeen
//	{
//		class CTween;
//	}
//}

namespace MySpace
{
	namespace TWeen
	{
		using namespace MySpace::MyMath;

		//--- クラス定義
		class CTween
		{
		private:
			//TODO: 終了時関数ポインタ作る?

		protected:
			std::weak_ptr<CTransform> m_pTarget;
			bool m_bEnd;
			float m_fMoveTime;
			float m_fWaitTime;

		protected:
			void SetEnd(bool flg) { m_bEnd = flg; }

		public:
			CTween() :m_fMoveTime(0), m_fWaitTime(0), m_bEnd(false){};
			CTween(std::weak_ptr<CTransform> ptr) :m_pTarget(ptr), m_fMoveTime(0), m_fWaitTime(0), m_bEnd(false){

			};
			virtual ~CTween() {};

			virtual void Init() {};
			virtual bool Update()
			{
				if (m_fWaitTime > 0) { --m_fWaitTime; return false; }
				if (!m_pTarget.lock()) { SetEnd(true); return false; }
				return true; 
			};

			void Destroy() { m_pTarget.reset(); };
			bool IsEnd() { return m_bEnd; };
			CTween* SetDelay(float time) { m_fMoveTime = time; return this; }
			CTween* SetMoveTime(float time) { m_fMoveTime = time; return this; }
		};

		class CUniformMove : public CTween
		{
			//--- メンバ変数
			Vector3 m_vInitPos;
			Vector3 m_vDestPos;
			Vector3 m_vVel;

		public:
			//--- メンバ関数
			CUniformMove() {};
			CUniformMove(std::weak_ptr<CTransform> ptr) : CTween(ptr) { }
			~CUniformMove() {};
			virtual bool Update()
			{
				if (!CTween::Update())
					return false;
				//pos = (pos + m_vVel);
				if (IsOver() == 3)
					SetEnd(true);
				return true;
			}
			void SetTargetPos(Vector3 targetPos, float time)
			{
				SetEnd(false);
				Vector3 pos = m_pTarget.lock()->GetPos();
				m_vInitPos = pos;
				m_vDestPos = targetPos;
				m_vVel = targetPos - pos / time;
				//m_fMoveTime = time;
			}
			int IsOver()
			{
				Vector3 pos = m_pTarget.lock()->GetPos();
				int ret = 0;
				if (m_vDestPos.x - m_vInitPos.x < 0)
				{
					if (m_vDestPos.x < pos.x)
						pos = (pos.x + m_vVel.x);
					else
						++ret;
				}
				else
					if (m_vDestPos.x > pos.x)
						pos = (pos.x + m_vVel.x);
					else
						++ret;
				
				if (m_vDestPos.y - m_vInitPos.y < 0)
				{
					if (m_vDestPos.y < pos.y)
						pos = (pos.y + m_vVel.y);
					else
						++ret;
				}
				else
					if (m_vDestPos.y > pos.y)
						pos = (pos.y + m_vVel.y);
					else
						++ret;
				
				if (m_vDestPos.z - m_vInitPos.z < 0)
				{
					if (m_vDestPos.z < pos.z)
						pos = (pos.z + m_vVel.z);
					else
						++ret;
				}
				else
					if (m_vDestPos.z > pos.z)
						pos = (pos.z + m_vVel.z);
					else
						++ret;
				m_pTarget.lock()->SetPos(pos);
				return ret;
			}
		};
		class CDoMove : public CTween
		{
		protected:
			Vector3 m_vInitPos;
			Vector3 m_vVel;
			Vector3 m_vAccel;
			float m_fCnt = 0;

		public:
			//CDoMove() {};
			CDoMove(std::weak_ptr<CTransform> ptr) : CTween(ptr)
			{
				m_vInitPos = m_pTarget.lock()->GetPos();
			};
			~CDoMove() {};

			//void Init();
			virtual bool Update()
			{
				if (!CTween::Update())
					return false;

				auto pos = m_pTarget.lock()->GetPos();
				m_fCnt += 1.0f / 60;
				pos.x = m_vInitPos.x + m_vVel.x * m_fCnt + (1.0f / 2 * (m_vAccel.x * m_fCnt * m_fCnt));
				pos.y = m_vInitPos.y + m_vVel.y * m_fCnt + (1.0f / 2 * (m_vAccel.y * m_fCnt * m_fCnt));
				pos.z = m_vInitPos.z + m_vVel.z * m_fCnt + (1.0f / 2 * (m_vAccel.z * m_fCnt * m_fCnt));
				m_vVel.x = m_vVel.x + m_vAccel.x * m_fCnt;
				m_vVel.y = m_vVel.y + m_vAccel.y * m_fCnt;
				m_vVel.z = m_vVel.z + m_vAccel.z * m_fCnt;
				m_pTarget.lock()->SetPos(pos);
				return true;
			}

			void SetVel(Vector3 value) { m_vVel = value; SetEnd(false); }
			void SetAccel(Vector3 value) { m_vAccel = value; SetEnd(false); }
		};

		class CDoJump : public CDoMove
		{
		public:
			CDoJump(std::weak_ptr<CTransform> ptr) : CDoMove(ptr)
			{
				SetVel({ 0,1,0 });
				SetAccel({ 0, 0.01f, 0 });
			};
			CDoJump(std::weak_ptr<CTransform> ptr, Vector3 vel, Vector3 acc) : CDoMove(ptr)
			{
				SetVel(vel);
				SetAccel(acc);
			};
			virtual bool Update()
			{
				if (!CDoMove::Update())
					return false;

				auto pos = m_pTarget.lock()->GetPos();
				if (pos.y < m_vInitPos.y)
				{
					SetEnd(true);
					//Destroy();
				}
			}
		};

		// *@色変更
		// *@SetColor関数を呼び出さなければいけない
		class CDoColor : public CTween
		{
		private:
			Color* m_pColor;
			Color m_vDestColor;

		public:
			CDoColor()
				:m_pColor(nullptr)
			{};
			CDoColor(std::weak_ptr<CTransform> ptr):CTween(ptr) ,m_pColor(nullptr)
			{};
			~CDoColor() {};

			bool Update() 
			{
				if (!CTween::Update() || !m_pColor)
					return false;

				Color color = *m_pColor;
				//m_pColor->a += (m_vDestColor.a - m_pColor->a) / m_fMoveTime;
				color = (m_vDestColor - color) / m_fMoveTime;

				// 差の確認,終了
				if (color.Check(Color(0, 0, 0, 0)))
					SetEnd(true);
				*m_pColor += color;
				return true;
			};
			void SetColor(Color* pColor, Color dest, float time = 2.f)
			{
				SetEnd(false);
				m_pColor = pColor;
				m_vDestColor = dest;
				m_fMoveTime = time;
			}
		};

	}
}

namespace MySpace
{
	namespace Game
	{
		//using MySpace::Tween::CTween;

		class CTweenManager
		{
		private:
			//--- メンバ変数
			std::vector<std::shared_ptr<MySpace::TWeen::CTween>> m_aTweenList;

		public:
			//--- メンバ関数
			CTweenManager() = default;
			~CTweenManager() = default;

			// *@更新
			// *@なにかしら終了していれば除外される
			void Update()
			{
				for (int cnt = 0; cnt < static_cast<int>(m_aTweenList.size()); ++cnt)
				{
					// 終了
					if (m_aTweenList[cnt]->IsEnd())
					{
						if (cnt + 1 < static_cast<int>(m_aTweenList.size()))
						{
							m_aTweenList[cnt] = m_aTweenList.back();
							m_aTweenList.pop_back();
						}
					}
					
					m_aTweenList[cnt]->Update();
				}
			}

			// *@型指定による生成
			template <class T>
			std::shared_ptr<T> CreateTween(std::weak_ptr<CTransform> ptr)
			{
				m_aTweenList.push_back(std::make_shared<T>(ptr));
				return m_aTweenList.back();
			}
		};
	}
}


#endif // !__TWEEN_H__
