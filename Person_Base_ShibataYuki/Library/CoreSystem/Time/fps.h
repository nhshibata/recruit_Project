//=========================================================
// [fps.h]
// fps、時間制御
//----------------------------
// 作成:2022/04/21
// 更新:2022/11/12 TimeScale追加
//=========================================================

//--- インクルードガード
#ifndef __FPS_H__
#define __FPS_H__

//--- インクルード部
#define NOMINMAX
#include <Windows.h>
#include <DebugSystem/debug.h>
#include <memory>

//--- 定数定義
#define FPS			(60)

namespace MySpace
{
	namespace System
	{
		//--- クラス定義
		class CFps
		{
		private:
			struct STFixedTime 
			{
				DWORD m_dwFixedExecLastTime = 0;		// 更新最後時間
				DWORD m_dwFixedTime	= 20;				// 更新間隔
				bool m_bUpdate = true;
			};
			enum ESlow
			{
				SLOW_NONE	= 0,
				SLOW_ON,
				SLOW_OFF,
			};
		private:
			//--- メンバ変数
			//CFps* m_pInstance;
			DWORD m_dwCurrentTime;		// 現在時間
			DWORD m_dwExecLastTime;		// 静的でない最後時間
			DWORD m_dwDeltaTime;		// デルタタイム
			int m_nHitStopFrame;		// ヒットストップ回数格納
			int m_dwSlowTime = -1;

			int m_nSlowFramPerSec;			// 一秒の分割数
			float m_fTimeScale = 1.0f;		// 時間
			ESlow m_eSlow;					// スロー開始フラグ
			bool m_bUpdate;

			STFixedTime m_FixedData;

		private:
			//--- メンバ関数
			CFps();
			~CFps();
		public:

//#pragma warning(push)
//#pragma warning(disable:4789)
			// *@シングルトンの取得
			static CFps& Get();
//#pragma warning(pop)   

			void Init();
			void Uninit();
			void Update();

			// *@更新してもいいか確認
			_NODISCARD inline bool IsUpdate()			{ return m_bUpdate; }
			// *@固定時間更新確認
			_NODISCARD inline bool IsFixedUpdate()		{ return m_FixedData.m_bUpdate; }

			// *@スロー開始
			void StartSlow(const int nSlowfps);			
			// *@スロー終了
			inline void SlowEnd()						{ m_eSlow = ESlow::SLOW_NONE; }
			// *@スロー確認
			_NODISCARD inline bool IsSlow()				{ return m_eSlow == ESlow::SLOW_ON; };
			
			//--- ゲッター・セッター
			_NODISCARD inline DWORD GetTime()			{ return m_dwCurrentTime; }
			_NODISCARD inline float GetTimeScale()		{ return m_fTimeScale; }
			_NODISCARD inline float DeltaTime()			{ return static_cast<float>(m_dwDeltaTime) / 1000 * m_fTimeScale; }
			_NODISCARD inline float UnScaleDeltaTime()	{ return static_cast<float>(m_dwDeltaTime) / 1000; }

			inline void SetCurrentTime(const DWORD currentTime) { m_dwCurrentTime = currentTime; };
			//inline void SetLastTime(const DWORD lastTime)		{ m_dwFixedExecLastTime = lastTime; };
			inline void SetHitStop(const int cnt)				{ if (m_nHitStopFrame <= 0) { m_nHitStopFrame = cnt; } };
			inline void SetTimeScale(float time)				{ if (time > 0 && time < 2.0f)m_fTimeScale = time; }
			void SetSlow(const int nSlowfps);
			void SetSlow(const int nSlowfps, const int nTime);
			
#if BUILD_MODE
		private:
			DWORD m_dwFPSLastTime;
			DWORD m_dwFrameCount;
			int m_nCountFPS;
			int m_nDebugFPS = FPS;
			DWORD m_dwDebugSlow;
		public:
			inline int GetFPSCount() { return m_nCountFPS; }
			void ImGuiDebug();
#endif // BUILD_MODE

		};
	}
}
#endif