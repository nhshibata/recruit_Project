// BGM/SE定義 [SoundData.hpp]
#pragma once
LPCWSTR g_pszFileBGM[MAX_BGM] = {
	L"result.mp3",		// リザルトBGM
	L"title.mp3",			// タイトルBGM
	L"自然_03.mp3",		// ゲームBGM
};

LPCWSTR g_pszFileSE[MAX_SE] = {
	L"8bit獲得7.mp3",			// アイテム
	L"8bit失敗2.mp3",			// HP0
	L"8bit消滅1.mp3",			// 撃破
	L"決定1.mp3",				// Enter
	L"振り下ろす一撃.mp3",		// 衝突
	L"太鼓.mp3",			// 成長
	L"太鼓2連打.mp3",		// ゲームオーバーBGM
};
