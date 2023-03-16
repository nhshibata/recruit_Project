//=========================================================
// [imageResourceManager.cpp] 
// 作成:2022/06/19
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/Manager/imageResourceManager.h>
#include <tchar.h>

using namespace MySpace::Graphics;

//==========================================================
// コンストラクタ
//==========================================================
CImageResourceManager::CImageResourceManager()
{
}

//==========================================================
// デスクトラクタ
//==========================================================
CImageResourceManager::~CImageResourceManager()
{
	UnloadAll();
}

//==========================================================
// データ読み込み
//==========================================================
bool CImageResourceManager::Load(std::string name)
{
	//--- 読み込み済みか確認
	if(m_aResourceMap.count(name))
		return true;

	// メモリ確保
	std::shared_ptr<CImageResource> tex = std::make_shared<CImageResource>();

	// 読み込み
	if (!tex->Load(name)) 
	{
		MessageBox(NULL, _T(name.c_str()), _T("texture load error"), MB_OK);
		return false;
	}

	// 格納
	m_aResourceMap.insert(IMAGE_PAIR(name, tex));
	return true;
}

//==========================================================
// 指定されたﾃﾞｰﾀの解放
//==========================================================
bool CImageResourceManager::Unload(std::string name)
{
	auto it = m_aResourceMap.find(name);
	if (it != m_aResourceMap.end())
	{
		m_aResourceMap[name].reset();	// 所有権の破棄
		m_aResourceMap.erase(name);		// リストからの除外
		return true;
	}
	return false;
}

//==========================================================
// 全開放
//==========================================================
void CImageResourceManager::UnloadAll()
{
	for (auto it = m_aResourceMap.begin(); it != m_aResourceMap.end(); ++it)
	{
		it->second.reset();
	}
	m_aResourceMap.clear();
}

//==========================================================
// 画像取得
//==========================================================
ImageSharedPtr CImageResourceManager::GetResource(std::string name)
{
	//--- 存在しない
	if (m_aResourceMap.count(name))
	{
		if (!Load(name))
		{
			MessageBox(NULL, _T(name.c_str()), _T("error"), MB_OK);

			// 読み込み失敗
			return std::shared_ptr<CImageResource>();
		}
	}

	return m_aResourceMap[name];
}

//==========================================================
// シーン解放時確認
//==========================================================
int CImageResourceManager::SceneUnload()
{
#if _DEBUG	// 確認
	int cnt = static_cast<int>(m_aResourceMap.size());
	for (auto & image : m_aResourceMap)
	{
		// 所持権が自身しか持っていなければ解放
		if (image.second.use_count() == 1)
		{
			image.second->Unload();
			image.second.reset();
			--cnt;
		}
	}
	return cnt;
#else
	for (auto & image : m_aResourceMap)
	{
		// 所持権が自身しか持っていなければ解放
		if (image.second.use_count() == 1)
		{
			image.second->Unload();
			image.second.reset();
		}
	}
#endif // _DEBUG

	return 0;
}