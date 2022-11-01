

// --- インクルード部 ---
#include <CoreSystem/Time/fps.h>

#pragma comment(lib, "winmm.lib")	// winmm.libを使用する
// --- 定数定義 ---

using namespace MySpace::System;

// コンストラクタ
CFps::CFps()
	:m_nSlowFramPerSec(60), m_dwExecLastTime(m_dwLastTime), m_bSlow(false), m_bUpdate(true)
{
}
// 引数付きコンストラクタ
CFps::CFps(int nSlowfps)
	: m_nSlowFramPerSec(60), m_dwExecLastTime(m_dwLastTime), m_bSlow(false), m_bUpdate(true)
{
	SetSlow(nSlowfps);
}
// デストラクタ
CFps::~CFps()
{
	Uninit();
}
void CFps::Init()
{
	// 精度を上げる
	timeBeginPeriod(1);

	// 現在時刻

}
void CFps::Uninit()
{
	// 精度を戻す
	timeEndPeriod(1);
}
// 更新
void CFps::Update()
{
	// 現在時刻の取得
	m_dwCurrentTime = timeGetTime();
	// 現在時間と前回の更新時間の差を取る 1fの時間を求める
	m_dwDeltaTime = (m_dwCurrentTime - m_dwExecLastTime);
	if (m_dwDeltaTime > 100) {
		m_dwDeltaTime = 100;
	}
	// フラグが立っていなければ設定されたスロー時間を渡す
	if (!m_bSlow)
	{
		if (m_nSlowFramPerSec < FPS)
		{
			m_nSlowFramPerSec = (int)(m_nSlowFramPerSec * 0.9f + FPS * 0.1f);
		}
	}
	// ヒットストップ回数が0より上の時
	if (m_nHitStopFrame > 0)
	{
		--m_nHitStopFrame;		// ヒットストップ回数を減らす
		m_bUpdate = false;		// 更新しない判定を返す
		return;
	}

	// 設定された速度で更新
	if ((m_dwCurrentTime - m_dwExecLastTime) >= (DWORD)(1000 / m_nSlowFramPerSec))
	{
		// スローならば回数を減らす
		if (m_bSlow)
		{
			--m_dwSlowTime;			// 回数を減らす
			if (m_dwSlowTime <= 0)
			{
				m_dwSlowTime = 0;
				m_bSlow = false;
			}
		}
		

		// 最終更新時間を更新
		m_dwExecLastTime = m_dwCurrentTime;
		m_bUpdate = true;
		return;
	}

	// 更新しない
	m_bUpdate = false;
}
// 更新してもいいか確認
bool CFps::IsUpdate()
{
	return m_bUpdate;
}
// スロー開始
void CFps::StartSlow(const int nSlowfps)
{
	m_bSlow = true;
	SetSlow(nSlowfps);
}
// スロー終了
void CFps::SlowEnd()
{
	m_bSlow = false;
	// メインのフレームと合わせる
	m_dwExecLastTime = m_dwLastTime;
}
// スロー時間設定
void CFps::SetSlow(const int nSlowfps)
{
	// 60より上なら修正
	if (nSlowfps > FPS)		
	{
		m_nSlowFramPerSec = FPS;
		return;
	}
	m_nSlowFramPerSec = nSlowfps;
}
// スロー時間設定
void CFps::SetSlow(const int nSlowfps, const int nTime)
{
	// 秒で決めたいため、60をかける
	m_dwSlowTime = nTime * 60;

	// 60より上なら修正
	if (nSlowfps > FPS)		
	{
		m_nSlowFramPerSec = FPS;
		return;
	}
	m_nSlowFramPerSec = nSlowfps;
}