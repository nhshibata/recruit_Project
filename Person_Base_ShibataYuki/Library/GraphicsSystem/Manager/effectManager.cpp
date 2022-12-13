//==============================================
// [effect.cpp] 
//-----------------------
// 2021/12/13 作成 柴田勇貴
// とりあえず描画確認
// 描画場所などの変更予定
// 2022/01/25 再生フレーム指定したい場合要変更
//==============================================

//--- インクルード部
#include <GraphicsSystem/Manager/effectManager.h>
#include <CoreSystem/Input/input.h>
#include <GraphicsSystem/DirectX/DXDevice.h>
#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Light/directionalLight.h>
#include <uchar.h>

// --- 定数定義 ---
#define KARI_FILE	u"data/effect/Laser01.efkefc"

using namespace MySpace::System;
using namespace MySpace::Game;
using namespace MySpace::Graphics;

// --- 静的メンバ変数 ---
//EffekseerRendererDX11::RendererRef CEffekseer::m_renderer;
//Effekseer::ManagerRef CEffekseer::m_manager;
//Effekseer::Handle  CEffekseer::m_handle;

// コンストラクタ
CEffekseer::CEffekseer()
{
}
// デストラクタ
CEffekseer::~CEffekseer()
{
}
// シーン毎に一度だけ呼び出す
void CEffekseer::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
	HRESULT hr = S_OK;

	//hr = Load(device, context);
		// DirectXで画面にオブジェクトが表示されるまで
	// ①オブジェクトの頂点情報(面の構成情報)
	// ②頂点レイアウト(InputLayout)
	//   GPUに頂点構造を教えるためのデータ
	// ③頂点シェーダ(VertexShader)
	//   CPUからGPUに頂点データを送り、3D空間上の点を
	//   2D画面上の表示位置に変換する
	// ④ピクセルシェーダ(PixelShader)
	//   頂点シェーダで変換した表示位置に、
	//   どのような色が表示されるか指定する

	// 円柱オブジェクト作成
	struct CylinderVertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};
	const static int CylinderVtxCount = 16;
	CylinderVertex cylinderVtx[(CylinderVtxCount + 1) * 2];
	for (int i = 0; i <= CylinderVtxCount; ++i)
	{
		float rad = i * DirectX::XM_PI * 2.f / CylinderVtxCount;
		float u = static_cast<float>(i * 3.0f) / CylinderVtxCount;
		DirectX::XMFLOAT3 normal(sinf(rad), 0.0f, -cosf(rad));
		int idx = i * 2;
		// 上段
		cylinderVtx[idx + 0].normal = normal;
		cylinderVtx[idx + 0].pos = normal;
		cylinderVtx[idx + 0].pos.y = 1.0f;
		cylinderVtx[idx + 0].uv = DirectX::XMFLOAT2(u, 0.0f);
		// 下段
		cylinderVtx[idx + 1].normal = normal;
		cylinderVtx[idx + 1].pos = normal;
		cylinderVtx[idx + 1].pos.y = -1.0f;
		cylinderVtx[idx + 1].uv = DirectX::XMFLOAT2(u, 1.0f);
	}
	CylinderVertex polyVtx[4] = {
		{ DirectX::XMFLOAT3(-1, 1,0), DirectX::XMFLOAT3(0,0,1), DirectX::XMFLOAT2(0, 0) },
		{ DirectX::XMFLOAT3(1, 1,0), DirectX::XMFLOAT3(0,0,1), DirectX::XMFLOAT2(1, 0) },
		{ DirectX::XMFLOAT3(-1,-1,0), DirectX::XMFLOAT3(0,0,1), DirectX::XMFLOAT2(0, 1) },
		{ DirectX::XMFLOAT3(1,-1,0), DirectX::XMFLOAT3(0,0,1), DirectX::XMFLOAT2(1, 1) },
	};

	// 定数バッファ作成
	CCamera* pCamera = CCamera::GetMain();
	if (!pCamera) { hr = E_FAIL; return; }

	DirectX::XMStoreFloat4x4(&m_vsMatrix.world, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_vsMatrix.view, pCamera->GetLookAtMatrix());
	DirectX::XMStoreFloat4x4(&m_vsMatrix.proj, pCamera->GetProjectionMatrix());

	//// シェーダ読み込み
	//HRESULT hr = S_OK;
	//m_pSampleVS = new VertexShader();
	//hr = m_pSampleVS->Load("Assets/SampleVS.cso");
	//if (FAILED(hr)) { return hr; }
	//m_pSamplePS = new PixelShader();
	//hr = m_pSamplePS->Load("Assets/SamplePS.cso");
	//if (FAILED(hr)) { return hr; }

	// テクスチャの読み込み
	//m_pTexture = new TextureFromFile();
	//hr = m_pTexture->Load(
	//	"Assets/stoneRoad.jpg");
	//if (FAILED(hr)) { return hr; }
	//m_pDisolveTex = new TextureFromFile();
	//hr = m_pDisolveTex->Load(
	//	"Assets/stoneRoadN.png");
	//if (FAILED(hr)) { return hr; }

	// Effekseer初期化
	m_renderer = EffekseerRendererDX11::Renderer::Create(device, context, 8000);
	m_manager = Effekseer::Manager::Create(8000);
	// 描画モジュールの設定
	m_manager->SetSpriteRenderer(m_renderer->CreateSpriteRenderer());
	m_manager->SetRibbonRenderer(m_renderer->CreateRibbonRenderer());
	m_manager->SetRingRenderer(m_renderer->CreateRingRenderer());
	m_manager->SetTrackRenderer(m_renderer->CreateTrackRenderer());
	m_manager->SetModelRenderer(m_renderer->CreateModelRenderer());
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	m_manager->SetTextureLoader(m_renderer->CreateTextureLoader());
	m_manager->SetModelLoader(m_renderer->CreateModelLoader());
	m_manager->SetMaterialLoader(m_renderer->CreateMaterialLoader());
	m_manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
	// 行列
	Effekseer::Vector3D eye = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);
	// 投影行列を設定
	m_renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovRH(
		90.0f / 180.0f * 3.14f, 16.0f / 9.0f, 1.0f, 500.0f));
	// カメラ行列を設定
	m_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(eye, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
	// 2バイト文字の取得
	//m_effect = Effekseer::Effect::Create(m_manager, u"data/Assets/wind.efkefc");
	//m_effect = Effekseer::Effect::Create(m_manager, u"data/Assets/ice.efk");

	Load(KARI_FILE);

	m_nTimer = 0;
}
void CEffekseer::Uninit()
{
	auto it = m_aResourceMap.begin();
	for (; it != m_aResourceMap.end(); ++it)
	{
		(*it).second.Reset();
	}
	m_aResourceMap.clear();
	
	// ShutDown the manager
	// マネージャーの破棄
	//if (m_manager.Get())
	//{
	//	m_manager.Get()->Release();
	//	//m_manager.Reset();
	//}

	// ShutDown the sound
	// サウンドの破棄
	//m_sound.Reset();

	// ShutDown the renderer
	// レンダラーの破棄
	/*if (m_renderer.Get())
	{
		m_renderer.Get()->Release();
	}*/
}

void CEffekseer::Update()
{
	// エフェクトの再生 	
	//m_handle = m_manager->Play(m_effect[rand()%3][0], m_vPos.x, m_vPos.y, m_vPos.z);

	if (CInput::GetKeyTrigger(VK_Q))
	{
		Play(u"laser", DirectX::XMFLOAT3(0, 0, 0));
		// stop effects
		//m_manager->StopEffect(m_handle);
	}

	CCamera* pCamera = CCamera::GetMain();
	if (!pCamera)return;

	// 行列
	DirectX::XMFLOAT3 cameraPos = pCamera->GetPos();
	DirectX::XMFLOAT3 cameraLook = pCamera->GetTarget();
	DirectX::XMFLOAT3 cameraUp = pCamera->GetUpVector();
	Effekseer::Vector3D eye = ::Effekseer::Vector3D(cameraPos.x, cameraPos.y, cameraPos.z);
	Effekseer::Vector3D look = ::Effekseer::Vector3D(cameraLook.x, cameraLook.y, cameraLook.z);
	Effekseer::Vector3D up = ::Effekseer::Vector3D(cameraUp.x, cameraUp.y, cameraUp.z);
	// 投影行列を設定
	m_renderer->SetProjectionMatrix(::Effekseer::Matrix44().PerspectiveFovLH(
		pCamera->GetFOV(), pCamera->GetAspect(), pCamera->GetNearClip(), pCamera->GetFarZ()));
	// カメラ行列を設定
	m_renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(eye, look, up));

	// ライトの移動
	//static float rad = 0.0f;

	//m_psLight = DirectX::XMFLOAT4(sinf(rad), 0, cosf(rad), 0);
	//rad += 0.01f;
	//m_pLightCB->Write(&m_psLight);

	//DirectX::XMFLOAT3 pos = pCamera->GetPos();
	//DirectX::XMFLOAT4 pos4(pos.x, pos.y, pos.z, 0.0f);
	//m_pCameraCB->Write(&pos4);

	EffectParam::iterator paramIt = m_effectParam.begin();
	while (paramIt != m_effectParam.end())
	{
		// 描画時間が終わっているか確認
		if (!(*paramIt).IsPlay())	// 終わっていればエフェクトの停止命令、パラメータを削除
		{
			m_manager->StopEffect((*paramIt).GetHandle());
			paramIt = m_effectParam.erase(paramIt);
			continue;
		}
		XMFLOAT3 pos = (*paramIt).GetPos();
		XMFLOAT4 rot = (*paramIt).GetRot();
		XMFLOAT3 size = (*paramIt).GetScale();
		Effekseer::Handle handle = (*paramIt).GetHandle();

		m_manager->SetLocation(handle, pos.x, pos.y, pos.z);
		m_manager->SetScale(handle, size.x, size.y, size.z);		// サイズ変更

		m_manager->SetRotation(handle, Effekseer::Vector3D(rot.x, rot.y, rot.z), rot.w);

		// 次へ
		++paramIt;
	}

	// 更新(すべてのエフェクト)
	// update the manager
	m_manager->Update();
}

void CEffekseer::Draw()
{
	// エフェクトの描画
	m_renderer->SetTime(m_nTimer++ / 60.0f);	// 内部の時間を進める

	// 描画
	CDXDevice::Get().SetZBuffer(false);
	CDXDevice::Get().SetBlendState((int)EBlendState::BS_ALPHABLEND);
	m_renderer->BeginRendering();			// 描画開始

	m_manager->Draw();

	m_renderer->EndRendering();				// 描画終了
	CDXDevice::Get().SetBlendState((int)EBlendState::BS_NONE);
	CDXDevice::Get().SetZBuffer(true);
}

void CEffekseer::SetMatrix(Effekseer::Matrix43* mtx, XMFLOAT4X4 XMmtx)
{
	mtx->Value[0][0] = XMmtx._11;
	mtx->Value[0][1] = XMmtx._12;
	mtx->Value[0][2] = XMmtx._13;

	mtx->Value[1][0] = XMmtx._21;
	mtx->Value[1][1] = XMmtx._22;
	mtx->Value[1][2] = XMmtx._23;

	mtx->Value[2][0] = XMmtx._31;
	mtx->Value[2][1] = XMmtx._32;
	mtx->Value[2][2] = XMmtx._33;

	mtx->Value[3][0] = XMmtx._41;
	mtx->Value[3][1] = XMmtx._42;
	mtx->Value[3][2] = XMmtx._43;
}
// kari
bool CEffekseer::Load(std::u16string fileName)
{
	if (m_aResourceMap.find(fileName.c_str()) != m_aResourceMap.end())
	{
		return false;
	}
	
	m_aResourceMap.insert(EffectMapPair(fileName.c_str(), Effekseer::Effect::Create(m_manager, fileName.c_str())
	));
	return true;
}
// ﾌｧｲﾙ読み込み用
bool CEffekseer::Load(std::vector<std::string> EffectName, std::vector<std::u16string> fileName)
{
	//std::vector<std::string>::iterator nameIt = EffectName.begin();
	//std::vector<std::u16string>::iterator fileIt = fileName.begin();
	//// 格納されているファイル名を受け取り、エフェクトを作成
	//while (fileIt != fileName.end())
	//{
	//	// 読み込んでないか調べる
	//	if (m_Effect.find((*nameIt).c_str()) != m_Effect.end())
	//	{
	//		// キーが見つかった = 既に読み込まれたファイル
	//		++nameIt;
	//		++fileIt;
	//		continue;
	//	}
	//	// ファイル名とエフェクトを追加
	//	m_Effect.insert(EffectMapPair((*nameIt).c_str(), Effekseer::Effect::Create(m_manager, (*fileIt).c_str())
	//	));

	//	++nameIt;
	//	++fileIt;
	//}
	return true;
}
// 設定した名前のエフェクトを再生
int CEffekseer::Play(std::u16string effectName, XMFLOAT3 pos)
{
	// 設定されたエフェクトじゃなければ処理を返す
	if (m_aResourceMap.find(effectName) == m_aResourceMap.end())	// findの返値が終端なら処理を返す
	{
		return -1;
	}
	// 再生
	Effekseer::Handle handle = m_manager->Play(m_aResourceMap[effectName], pos.x, pos.y, pos.z);
	m_effectParam.push_back(CEffekseerParam(&m_aResourceMap[effectName], handle,
		m_aResourceMap[effectName]->CalculateTerm().TermMax));

	return handle;
}
// 設定した名前のエフェクトを再生
int CEffekseer::Play(std::u16string effectName, XMFLOAT3 pos, XMFLOAT3 size, XMFLOAT4 rot)
{
	// 設定されたエフェクトじゃなければ処理を返す
	if (m_aResourceMap.find(effectName) == m_aResourceMap.end())	// findの返値が終端なら処理を返す
	{
		return -1;
	}
	// 再生
	Effekseer::Handle handle = m_manager->Play(m_aResourceMap[effectName], pos.x, pos.y, pos.z);
	// パラメータ生成。エフェクトのポインタと再生したエフェクトのハンドルを渡す
	m_effectParam.push_back(CEffekseerParam(&m_aResourceMap[effectName], handle, m_aResourceMap[effectName]->CalculateTerm().TermMax));
	m_effectParam.back().SetPos(pos);
	m_effectParam.back().SetScale(size);
	m_effectParam.back().SetRot(rot);

	m_manager->SetScale(handle, size.x, size.y, size.z);		// サイズ変更
	m_manager->SetRotation(handle, Effekseer::Vector3D(rot.x, rot.y, rot.z), rot.w);
	return handle;
}
// 停止
int CEffekseer::Stop(int handle)
{
	m_manager->StopEffect(handle);

	return -1;
}
int CEffekseer::Move(int handle, XMFLOAT3 pos)
{
	if (!m_manager->Exists(handle))
	{
		return -1;
	}

	// 移動
	Effekseer::Matrix43 mtx;
	m_vsMatrix.view._41 = pos.x;
	m_vsMatrix.view._42 = pos.y;
	m_vsMatrix.view._43 = pos.z;
	// Effekseerの構造体に変換
	SetMatrix(&mtx, m_vsMatrix.view);
	m_manager->SetMatrix(handle, mtx);		// 行列設定
	
	return handle;
}