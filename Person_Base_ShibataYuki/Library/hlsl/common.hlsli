//=========================================================
// [common.hlsli]
//---------------------------------------------------------
// 更新:2023/02/05 定数管理のため、 hlsli追加
//---------------------------------------------------------
// 定数、構造体管理
// なるべくPS、VSどこで使うかコメント
//=========================================================

//--- 定数定義
#define MAX_BONE_MATRIX	    64
#define MAX_WORLD_MATRIX    400
#define IF_SUB(x,y,z)       (x + ((x * y - x) * z))

//--- 構造体定義
// 行列
struct InstancingData
{
    matrix mWorld;
};

// マテリアル
struct MaterialData
{
    float4 fAmbient; // 環境色
    float4 fDiffuse; // 拡散色+アルファ
    float4 fSpecular; // 鏡面反射色+強度
    float4 fEmissive; // 発光色
    float4 fFlags; // 拡散色テクスチャ有無, 発光色テクスチャ有無, 透過テクスチャ有無
};

// meshVS
// グローバル
cbuffer global : register(b0)
{
    matrix g_mVP; // ワールド×ビュー×射影行列
    matrix g_mWorld; // ワールド行列
    matrix g_mTexture;
};

// model用PS/VS
// グローバル
cbuffer global : register(b0)
{
    matrix g_mtxVP;         // ビュー×射影行列
    matrix g_mtxWorld;      // ワールド行列
    matrix g_mtxTexture;    // テクスチャ行列
    float4 g_vCameraPos;    // 視点座標(ワールド空間)
    float4 g_vLightDir;     // 光源方向(ワールド空間)
    float4 g_vLightAmbient; // 環境光
    float4 g_vLightDiffuse; // 拡散光
    float4 g_vLightSpecular; // 鏡面反射光
};

// model用PS
// マテリアル
cbuffer global2 : register(b1)
{
    float4 g_Ambient;   // 環境色
    float4 g_Diffuse;   // 拡散色+アルファ
    float4 g_Specular;  // 鏡面反射色+強度
    float4 g_Emissive;  // 発光色
    float4 g_Flags;     // 拡散色テクスチャ有無, 発光色テクスチャ有無, 透過テクスチャ有無
};

// VS
// ボーンのポーズ行列
cbuffer global_bones : register(b2)
{
    matrix g_BoneWorld[MAX_BONE_MATRIX];
};

// model VS
// ワールド行列配列
cbuffer InstancingGlobal : register(b3)
{
    InstancingData g_Instancing[MAX_WORLD_MATRIX];
}

// 太陽の位置にあるｶﾒﾗに表示するための行列
// meshVS
cbuffer SunCamera : register(b4)
{
    float4x4 sunView;
    float4x4 sunProj;
};

// VS
// DepthWriteVSで使用
cbuffer WVP : register(b5)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

// PS Mesh/Model
// インスタンシング使用時の各自マテリアル
cbuffer InstancingMaterial : register(b6)
{
    MaterialData g_material[MAX_WORLD_MATRIX];
};
