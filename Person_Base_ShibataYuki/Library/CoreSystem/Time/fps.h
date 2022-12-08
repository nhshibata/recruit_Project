//=========================================================
// [fps.h]
// fps�A���Ԑ���
//----------------------------
// �쐬:2022/04/21
// �X�V:2022/11/12 TimeScale�ǉ�
//=========================================================

//--- �C���N���[�h�K�[�h
#ifndef __FPS_H__
#define __FPS_H__

//--- �C���N���[�h��
#define NOMINMAX
#include <Windows.h>
#include <CoreSystem/Singleton.h>
#include <DebugSystem/debug.h>

//--- �萔��`
#define FPS			(60)

namespace MySpace
{
	namespace System
	{
		//--- �N���X��`
		class CFps : public CSingleton<CFps>
		{
			friend class CSingleton<CFps>;
		private:
			struct STFixedTime 
			{
				DWORD m_dwFixedExecLastTime = 0;		// �X�V�Ō㎞��
				DWORD m_dwFixedTime	= 20;				// �X�V�Ԋu
				bool m_bUpdate = true;
			};
			enum ESlow
			{
				SLOW_NONE	= 0,
				SLOW_ON,
				SLOW_OFF,
			};
		private:
			//--- �����o�ϐ�
			//CFps* m_pInstance;
			DWORD m_dwCurrentTime;		// ���ݎ���
			DWORD m_dwExecLastTime;		// �ÓI�łȂ��Ō㎞��
			DWORD m_dwDeltaTime;		// �f���^�^�C��
			int m_nHitStopFrame;		// �q�b�g�X�g�b�v�񐔊i�[
			int m_dwSlowTime = -1;

			int m_nSlowFramPerSec;			// ��b�̕�����
			float m_fTimeScale = 1.0f;		// ����
			ESlow m_eSlow;					// �X���[�J�n�t���O
			bool m_bUpdate;

			STFixedTime m_FixedData;
			
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
			// *@�X�V���Ă��������m�F
			_NODISCARD inline bool IsUpdate()			{ return m_bUpdate; }
			// *@�Œ莞�ԍX�V�m�F
			_NODISCARD inline bool IsFixedUpdate()		{ return m_FixedData.m_bUpdate; }

			// *@�X���[�J�n
			void StartSlow(const int nSlowfps);			
			// *@�X���[�I��
			inline void SlowEnd()						{ m_eSlow = ESlow::SLOW_NONE; }
			// *@�X���[�m�F
			_NODISCARD inline bool IsSlow()				{ return m_eSlow == ESlow::SLOW_ON; };
			
			//--- �Q�b�^�[�E�Z�b�^�[
			_NODISCARD inline DWORD GetTime()			{ return m_dwCurrentTime; }
			_NODISCARD inline float GetTimeScale()		{ return m_fTimeScale; }
			_NODISCARD inline float DeltaTime()			{ return (static_cast<float>(m_dwDeltaTime) / 1000) * m_fTimeScale; }
			_NODISCARD inline float UnScaleDeltaTime()	{ return (static_cast<float>(m_dwDeltaTime) / 1000); }

			inline void SetCurrentTime(const DWORD currentTime) { m_dwCurrentTime = currentTime; };
			//inline void SetLastTime(const DWORD lastTime)		{ m_dwFixedExecLastTime = lastTime; };
			inline void SetHitStop(const int cnt)				{ if (m_nHitStopFrame <= 0) { m_nHitStopFrame = cnt; } };
			inline void SetTimeScale(float time)				{ if (time > 0 && time < 2.0f)m_fTimeScale = time; }
			void SetSlow(const int nSlowfps);
			void SetSlow(const int nSlowfps, const int nTime);

			// �V���O���g��
			//static void Start() { m_pInstance = new CFps(); };
			//static void End() { delete m_pInstance; };
			//static CFps* Get() { return m_pInstance; };

#if BUILD_MODE
			void ImGuiDebug();
#endif // BUILD_MODE

		};
	}
}
#endif