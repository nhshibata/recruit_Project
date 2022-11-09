//=========================================================
// �쐬:2022/04/21
// fps�A���Ԑ���
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __FPS_H__
#define __FPS_H__

#define NOMINMAX
#include <Windows.h>
#include <CoreSystem/Singleton.h>
#include <DebugSystem/debug.h>

//--- �萔��`
#define FPS			(60)
#define MAIN_FPS	CFps::Get()

namespace MySpace
{
	namespace System
	{
		//--- �N���X��`
		class CFps : public CSingleton<CFps>
		{
			friend class CSingleton<CFps>;
		private:
			//--- �ÓI�����o�ϐ�
			CFps* m_pInstance;
			DWORD m_dwCurrentTime;	// ���ݎ���
			DWORD m_dwDeltaTime;		// �f���^�^�C��
			DWORD m_dwLastTime;		// �X�V�Ō㎞��
			int m_nHitStopFrame;		// �q�b�g�X�g�b�v�񐔊i�[
			int m_dwSlowTime;

			//--- �����o�ϐ�
			int m_nSlowFramPerSec;			// ��b�̕�����
			DWORD m_dwExecLastTime;			// �ÓI�łȂ��Ō㎞��
			bool m_bSlow;					// �X���[�J�n�t���O
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

			// fps���C��
			inline DWORD GetTime() { return m_dwCurrentTime; }
			inline float DeltaTime() { return static_cast<float>(m_dwDeltaTime) / 1000; }
			inline void SetCurrentTime(const DWORD currentTime) { m_dwCurrentTime = currentTime; };
			inline void SetLastTime(const DWORD lastTime) { m_dwLastTime = lastTime; };
			inline void SetHitStop(const int cnt) { if (m_nHitStopFrame <= 0) { m_nHitStopFrame = cnt; } };

			// �V���O���g��
			//static void Start() { m_pInstance = new CFps(); };
			//static void End() { delete m_pInstance; };
			//static CFps* Get() { return m_pInstance; };

		};
	}
}
#endif