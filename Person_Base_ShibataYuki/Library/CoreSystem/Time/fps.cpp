//=========================================================
// [fps.h]
//---------------------------------------------------------
// 作成:2022/04/21
//---------------------------------------------------------
// fps、時間制御
//=========================================================

// --- インクルード部 ---
#include <CoreSystem/Time/fps.h>
#include <DebugSystem/imGuiPackage.h>

using namespace MySpace::System;

//==========================================================
// コンストラクタ
//==========================================================
CFps::CFps()
	:m_nSlowFramPerSec(60), m_dwExecLastTime(0), m_eSlow(ESlow::SLOW_NONE), m_bUpdate(true), m_dwCurrentTime(0),
	m_dwDeltaTime(0), m_dwSlowTime(0), m_fTimeScale(1.0f),m_nHitStopFrame(0)
{
	m_FixedData.m_bUpdate = true;
	m_FixedData.m_dwFixedExecLastTime = 0;
	m_FixedData.m_dwFixedTime = 20;

#ifdef BUILD_MODE
	m_dwFPSLastTime = 0;
	m_dwFrameCount = 0;
	m_nCountFPS = 0;
	m_dwDebugSlow = 0;
	m_nDebugFPS = FPS;
#endif // DEBUG

	SetSlow(FPS);
}

//==========================================================
// デストラクタ
//==========================================================
CFps::~CFps()
{
	
}

//==========================================================
// 初期化
//==========================================================
void CFps::Init()
{
	// 精度を上げる
	timeBeginPeriod(1);

#if BUILD_MODE
	// 現在時刻
	m_dwExecLastTime = m_dwFPSLastTime = timeGetTime();
	m_dwCurrentTime = m_dwFrameCount = 0;
#endif // BUILD_MODE
	srand(unsigned int(::GetTickCount64()) );
}

//==========================================================
// 終了処理
//==========================================================
void CFps::Uninit()
{
	// 精度を戻す
	timeEndPeriod(1);
}

//==========================================================
// 更新
//==========================================================
void CFps::Update()
{
	// 更新フラグ初期化
	m_bUpdate = false;
	// 現在時刻の取得
	m_dwCurrentTime = timeGetTime();

#if BUILD_MODE
	if ((m_dwCurrentTime - m_dwFPSLastTime) >= 500)
	{	// 0.5秒ごとに実行
		m_nCountFPS = m_dwFrameCount * 1000 /(m_dwCurrentTime - m_dwFPSLastTime);
		m_dwFPSLastTime = m_dwCurrentTime;
		m_dwFrameCount = 0;
	}
	++m_dwFrameCount;
#endif // BUILD_MODE

	//--- deltaTime
	// 現在時間と前回の更新時間の差を取る 1fの時間を求める
	m_dwDeltaTime = (m_dwCurrentTime - m_dwExecLastTime);
	// 外部からの介入があった際のため、上限を決める
	if (m_dwDeltaTime > 50) 
		m_dwDeltaTime = 50;

	//--- 固定時間
	m_FixedData.m_bUpdate = false;
	if (auto diffTime = float(m_dwCurrentTime - m_FixedData.m_dwFixedExecLastTime) * m_fTimeScale;  
		(DWORD)diffTime >= m_FixedData.m_dwFixedTime)
	{
		m_FixedData.m_dwFixedExecLastTime = m_dwCurrentTime;
		m_FixedData.m_bUpdate = true;
	}

	// 設定された速度で更新
	if (m_dwDeltaTime >= (DWORD)(1000 / m_nSlowFramPerSec))
	//if ((m_dwCurrentTime - m_dwExecLastTime) >= (DWORD)(1000 / m_nSlowFramPerSec))
	{
		// 最終更新時間を更新
		m_dwExecLastTime = m_dwCurrentTime;
		m_bUpdate = true;

		// スローならば回数を減らす
		if (m_eSlow == ESlow::SLOW_ON && m_dwSlowTime != -1)
		{
			--m_dwSlowTime;			// 回数を減らす
			if (m_dwSlowTime <= 0)
			{
				m_dwSlowTime = -1;
				m_eSlow = ESlow::SLOW_OFF;
			}
		}

		//--- スロー
		if (m_eSlow == ESlow::SLOW_OFF)
		{
			// 徐々に戻す
			if (m_nSlowFramPerSec < FPS)
			{
				int fps = static_cast<int>(m_nSlowFramPerSec * 0.9f + FPS * 0.1f);
				m_nSlowFramPerSec = fps;
				if (FPS - fps < 0.2f)
				{
					m_eSlow = ESlow::SLOW_NONE;
					m_nSlowFramPerSec = FPS;
				}
			}
		}

		//--- ヒットストップ
		// ヒットストップ回数が0より上の時
		if (m_nHitStopFrame > 0)
		{
			--m_nHitStopFrame;		// ヒットストップ回数を減らす
			m_bUpdate = false;		// 更新しない
		}
	}
}

//==========================================================
// スロー開始
//==========================================================
void CFps::StartSlow(const int nSlowfps)
{
	m_eSlow = ESlow::SLOW_ON;
	SetSlow(nSlowfps);
}

//==========================================================
// スロー時間設定
//==========================================================
void CFps::SetSlow(const int nSlowfps)
{
	// 60より上なら修正
	if (nSlowfps > FPS)
	{
		m_nSlowFramPerSec = FPS;
		return;
	}
	else if (nSlowfps < 1)
		return;
	m_nSlowFramPerSec = nSlowfps;
}

//==========================================================
// スロー時間設定
//==========================================================
void CFps::SetSlow(const int nSlowfps, const int nTime)
{
	// 秒で決めたいため、60をかける
	m_dwSlowTime = nTime * 60;
	SetSlow(nSlowfps);
}

//==========================================================
// シングルトン取得
//==========================================================
CFps* CFps::Get()
{
	static CFps pInstance;
	return &pInstance;
}


#if BUILD_MODE

void CFps::ImGuiDebug()
{
	//--- 情報表示
	ImGui::Checkbox(u8"UpdateON", &m_bUpdate);
	
	Debug::SetTextAndAligned("DeltaTime:");
	ImGui::Text("%.5f", CFps::Get()->DeltaTime());

	Debug::SetTextAndAligned("UnScaleDeltaTime:");
	ImGui::Text("%.5f", CFps::Get()->UnScaleDeltaTime());

	Debug::SetTextAndAligned("TimeScale:");
	ImGui::Text("%.5f", CFps::Get()->GetTimeScale());
	
	Debug::SetTextAndAligned("Count:");
	ImGui::Text("%d", CFps::Get()->GetFPSCount());

	//--- 設定
	/*if (ImGui::Button("FPS Set"))
		SetSlow(m_nDebugFPS);*/
	
	Debug::SetTextAndAligned(u8"FPS 分割数");
	ImGui::DragInt("##fps", &m_nDebugFPS, 1, 1, 60);

	Debug::SetTextAndAligned("TimeScale");
	if(ImGui::DragFloat("##TimeScale", &m_fTimeScale))
		SetTimeScale(m_fTimeScale);

	Debug::SetTextAndAligned("HitStop Num");
	ImGui::DragInt("##HitStopNum", &m_nHitStopFrame);
	
	Debug::SetTextAndAligned("Slow Time");
	ImGui::DragInt("##SlowTime", (int*)&m_dwDebugSlow);
	
	if (ImGui::Button("Slow OK?"))
	{
		m_dwSlowTime = m_dwDebugSlow;
	}
	ImGui::SameLine();
	if (ImGui::Button("Slow ON/OFF"))
	{
		m_eSlow = (ESlow)(m_eSlow ^ ESlow::SLOW_ON);
	}

	//--- 固定時間
	ImGui::Checkbox("Fixed Update", &m_FixedData.m_bUpdate);

	int newFixedTime = (int)m_FixedData.m_dwFixedTime;
	Debug::SetTextAndAligned("Fixed Time");
	if (ImGui::DragInt("##FixedTime", &newFixedTime))
	{
		m_FixedData.m_dwFixedTime = newFixedTime;
	}
	
}

#endif // BUILD_MODE
