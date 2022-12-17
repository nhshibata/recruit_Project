//=========================================================
// [screen.h]
// 作成:2022/07/03
// 作成:2022/08/19 座標変換追加
//=========================================================

//--- インクルードガード
#ifndef __SCREEN_H__
#define __SCREEN_H__

//--- インクルード部
#include <CoreSystem/Math/myVector.h>

using namespace MySpace::MyMath;

namespace MySpace
{
	namespace System
	{
		class CScreen
		{
		private:
			//--- メンバ変数
			static float m_fWidth;
			static float m_fHeight;
			static Matrix4x4 m_mtxScreen;		// スクリーン座標変換用
			
		public:
			//--- メンバ関数

			//--- ゲッター・セッター
			static inline float GetWidth() { return m_fWidth; };
			static inline float GetHeight() { return m_fHeight; };
			static inline float GetHalfWidth() { return m_fWidth/2; };
			static inline float GetHalfHeight() { return m_fHeight/2; };
			static inline Vector2 GetSize() { return Vector2(m_fWidth, m_fHeight); };
			static inline void SetSize(Vector2 size) { m_fWidth = size.x; m_fHeight = size.y; SetScreenMatrix(m_fWidth, m_fHeight); };
			static inline void SetSize(float x, float y) { m_fWidth = x; m_fHeight = y; SetScreenMatrix(m_fWidth, m_fHeight); };
			
			// *@スクリーン行列更新
			static void SetScreenMatrix(float width,float height)
			{
				m_mtxScreen = Matrix4x4(
					width / 2, 0.0f, 0.0f, 0.0f,
					0.0f, -height / 2, 0.0f, 0.0f,
					width / 2, 0.0f, 1.0f, 0.0f,
					width / 2, height / 2, 0.0f, 1.0f
				);
			}

			// *@3D座標からスクリーン座標へ
			static Vector2 ConvertWorldToScreen(Vector3 pos)
			{
				//Vector2 ret;
				auto vector = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
				//DirectX::XMStoreFloat3(&ret, DirectX::XMVector3Transform(vector, DirectX::XMLoadFloat4x4(&m_mtxScreen)));
				auto xy = DirectX::XMVector3Transform(vector, DirectX::XMLoadFloat4x4(&m_mtxScreen));

				return Vector2(xy.m128_f32[0], xy.m128_f32[1]);
			};
			// *@画面内かどうか
			static bool ScreenJudg(Vector3 pos)
			{
				auto scrPos = ConvertWorldToScreen(pos);
				if (m_fWidth / 2 >= scrPos.x && -m_fWidth / 2 <= scrPos.x)
				{
					if (m_fHeight / 2 >= scrPos.y && -m_fHeight / 2 <= scrPos.y)
						return true;
				}
				return false;
			}
			// *@画面内かどうか
			static bool ScreenJudg(Vector2 pos)
			{
				if (m_fWidth / 2 >= pos.x && -m_fWidth / 2 <= pos.x)
				{
					if (m_fHeight / 2 >= pos.y && -m_fHeight / 2 <= pos.y)
						return false;
				}
				return true;
			}
			// *@画面内かどうか
			static bool ScreenJudg(Vector2 pos, Vector2 size)
			{
				if (m_fWidth / 2 >= pos.x + size.x && -m_fWidth / 2 <= pos.x - size.x)
				{
					if (m_fHeight / 2 >= pos.y + size .y && -m_fHeight / 2 <= pos.y - size.y)
						return false;
				}
				return true;
			}
			// *@Vecor2
			// *@画面端の場合、反転した座標を返す
			static Vector2 ScreenReverse(Vector2 pos)
			{
				Vector2 ret = pos;
				if (ret.x > m_fWidth / 2)
					ret.x -= m_fWidth / 2;
				else if (ret.x < -m_fWidth / 2)
					ret.x += m_fWidth / 2;

				if (ret.y > m_fHeight / 2)
					ret.y -= m_fHeight / 2;
				else if (ret.y < -m_fHeight / 2)
					ret.y += m_fHeight / 2;
				return ret;
			}

			// *@画面からの距離(-の値)
			static Vector2 DistanceFromScreen(Vector3 pos)
			{
				Vector2 ret;
				auto scrPos = ConvertWorldToScreen(pos);
				if (scrPos.x > 0)
					ret.x = m_fWidth / 2 - scrPos.x;
				else
					ret.x = -m_fWidth / 2 - scrPos.x;
				
				if (scrPos.y > 0)
					ret.y = m_fHeight / 2 - scrPos.y;
				else
					ret.y = -m_fHeight / 2 - scrPos.y;

				return ret;
			}
		};
	}
}
using namespace MySpace::System;

#endif // !__SCREEN_H__
