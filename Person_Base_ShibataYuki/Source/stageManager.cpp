//========================================================
// [stageManager.cpp]
//------------------------
// 作成:2023/01/02
//------------------------
//
//========================================================

//--- インクルード部
#include "stageManager.h"
#include "stageBuild.h"
#include "spellDefine.h"

using namespace Spell;

//========================================================
// コンストラクタ
//========================================================
CStageManager::CStageManager()
	:m_pCSV(nullptr), m_vCenter(0, 0, 0), m_fMargin(1)
{

}

//========================================================
// 引き数付きコンストラクタ
//========================================================
CStageManager::CStageManager(CGameObject::Ptr ptr)
	: CComponent(ptr),
	m_pCSV(nullptr), m_vCenter(0,0,0), m_fMargin(1)
{
}

//========================================================
// コンストラクタ
//========================================================
CStageManager::~CStageManager()
{
	if(m_pCSV)
		delete m_pCSV;
}

//========================================================
// 初期化
//========================================================
void CStageManager::Awake()
{
	

}

//========================================================
// 初期化
//========================================================
void CStageManager::StartBuild(std::string file)
{
	//--- ファイル読み込み
	if (m_pCSV)
		delete m_pCSV;

	m_pCSV = new CResourceCSV();
	m_pCSV->Load(file);

	//--- ステージ構築
	CStageBuild build(PointToPos(GridPoint(0, 0)), 1);
	build.Build(m_pCSV);
}

//========================================================
// stageData情報取得
//========================================================
int CStageManager::GetData(const Vector3 pos)
{
	auto grid = PosToPoint(pos);
	return m_pCSV->GetInt(grid.x, grid.y);
}

//========================================================
// stageData情報取得
//========================================================
int CStageManager::GetData(const int idx)
{
	auto grid = IndexToPoint(idx);
	return m_pCSV->GetInt(grid.x, grid.y);
}

//========================================================
// indexから二次元配列へ変換
//========================================================
CStageManager::GridPoint CStageManager::IndexToPoint(int index)
{
	return GridPoint{ index % m_pCSV->GetWidth() , index / m_pCSV->GetHeight() };
}

//========================================================
// 座標から二次元配列へ変換
//========================================================
CStageManager::GridPoint CStageManager::PosToPoint(const Vector3 pos)
{
	float startX = m_vCenter.x - m_pCSV->GetWidth() * m_fMargin * 0.5f;
	float startZ = m_vCenter.z - m_pCSV->GetHeight() * m_fMargin * 0.5f;
	return GridPoint(
		static_cast<int>((pos.x - startX) / m_fMargin),
		static_cast<int>((pos.z - startZ) / m_fMargin)
	);
}

//========================================================
// 座標からインデックスへ
//========================================================
int CStageManager::PointToIndex(GridPoint point)const 
{
	return point.y * m_pCSV->GetHeight() + point.x;
}

//========================================================
// 二次元配列から座標へ変換
//========================================================
Vector3 CStageManager::PointToPos(GridPoint point)
{
	float startX = m_vCenter.x - (m_pCSV->GetWidth() - 1) * m_fMargin * 0.5f;
	float startZ = m_vCenter.z - (m_pCSV->GetHeight() - 1) * m_fMargin * 0.5f;
	return Vector3(
		startX + point.x * m_fMargin,
		m_vCenter.y,
		startZ + point.y * m_fMargin
	);
}