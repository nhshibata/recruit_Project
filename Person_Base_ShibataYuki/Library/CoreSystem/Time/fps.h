//=========================================================
// 作成:2022/04/21
// fps、時間制御
//=========================================================

//--- インクルードガード
#ifndef __FPS_H__
#define __FPS_H__

#define NOMINMAX
#include <Windows.h>
#include <CoreSystem/Singleton.h>
#include <DebugSystem/debug.h>

//--- 定数定義
#define FPS			(60)
#define MAIN_FPS	CFps::Get()

namespace MySpace
{
	namespace System
	{
		//--- クラス定義
		class CFps : public CSingleton<CFps>
		{
			friend class CSingleton<CFps>;
		private:
			//--- 静的メンバ変数
			CFps* m_pInstance;
			DWORD m_dwCurrentTime;	// 現在時間
			DWORD m_dwDeltaTime;		// デルタタイム
			DWORD m_dwLastTime;		// 更新最後時間
			int m_nHitStopFrame;		// ヒットストップ回数格納
			int m_dwSlowTime;

			//--- メンバ変数
			int m_nSlowFramPerSec;			// 一秒の分割数
			DWORD m_dwExecLastTime;			// 静的でない最後時間
			bool m_bSlow;					// スロー開始フラグ
			bool m_bUpdate;
			
#if BUILD_MODE
			DWORD m_dwFPSLastTime;
			DWORD m_dwFrameCount;
			int m_nCountFPS;
		public:
			inline int GetFPSCount() { return m_nCountFPS; }
#endif // BUILD_MODE

		public:
			CFps();
			CFps(const int nSlowfps);
			~CFps();

			void Init();
			void Uninit();
			void Update();
			bool IsUpdate();
			void StartSlow(const int nSlowfps);
			void SlowEnd();
			void SetSlow(const int nSlowfps);
			void SetSlow(const int nSlowfps, const int nTime);
			
			inline bool IsSlow() { return m_bSlow; };

			// fpsメイン
			inline DWORD GetTime() { return m_dwCurrentTime; }
			inline float DeltaTime() { return static_cast<float>(m_dwDeltaTime) / 1000; }
			inline void SetCurrentTime(const DWORD currentTime) { m_dwCurrentTime = currentTime; };
			inline void SetLastTime(const DWORD lastTime) { m_dwLastTime = lastTime; };
			inline void SetHitStop(const int cnt) { if (m_nHitStopFrame <= 0) { m_nHitStopFrame = cnt; } };

			// シングルトン
			//static void Start() { m_pInstance = new CFps(); };
			//static void End() { delete m_pInstance; };
			//static CFps* Get() { return m_pInstance; };

		};
	}
}
#endif