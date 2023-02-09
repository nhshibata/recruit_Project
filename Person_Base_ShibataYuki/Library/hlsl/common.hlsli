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
    float4 fAmbient;    // 環境色
    float4 fDiffuse;    // 拡散色+アルファ
    float4 fSpecular;   // 鏡面反射色+強度
    float4 fEmissive;   // 発光色
    float4 fFlags;      // 拡散色テクスチャ有無, 発光色テクスチャ有無, 透過テクスチャ有無
};


//==========================================================
// 定数バッファ
//---------------------------------------------------------
// cb0 WVP 描画時
// cb1 Camera&Light 描画前
// cb2 マテリアル 描画時
// cb3 ボーン モデル描画時
// cb4 インスタンシングマトリックス 描画時
// cb5 インスタンシングマテリアル 描画時
// cb6 ライト情報 描画前
//==========================================================

// VS
// meshVS、DepthWriteVSで使用
cbuffer WVP : register(b0)
{
    matrix g_world;
    matrix g_view;
    matrix g_proj;
    matrix g_mTexture; // テクスチャ行列
};

// PS/VS model
// グローバル
cbuffer global : register(b1)
{
    float4 g_vCameraPos;     // 視点座標(ワールド空間)
    float4 g_vLightDir;      // 光源方向(ワールド空間)
    float4 g_vLightAmbient;  // 環境光
    float4 g_vLightDiffuse;  // 拡散光
    float4 g_vLightSpecular; // 鏡面反射光
};

// PS model
// マテリアル
cbuffer global2 : register(b2)
{
    float4 g_Ambient;   // 環境色
    float4 g_Diffuse;   // 拡散色+アルファ
    float4 g_Specular;  // 鏡面反射色+強度
    float4 g_Emissive;  // 発光色
    float4 g_Flags;     // 拡散色テクスチャ有無, 発光色テクスチャ有無, 透過テクスチャ有無
};

// VS
// ボーンのポーズ行列
cbuffer global_bones : register(b3)
{
    matrix g_BoneWorld[MAX_BONE_MATRIX];
};

// VS model
// ワールド行列配列
cbuffer InstancingGlobal : register(b4)
{
    InstancingData g_Instancing[MAX_WORLD_MATRIX];
}

// PS Mesh/Model
// インスタンシング使用時の各自マテリアル
cbuffer InstancingMaterial : register(b5)
{
    MaterialData g_material[MAX_WORLD_MATRIX];
};

// 太陽の位置にあるｶﾒﾗに表示するための行列
// meshVS
cbuffer SunCamera : register(b6)
{
    float4x4 g_sunView;
    float4x4 g_sunProj;
};

// レート
// PS用
cbuffer Rate : register(b7)
{
    float4 g_rate;
}


//==========================================================
// 関数
//==========================================================
float2 GetSunUV(float4 sunPos)
{
    //--- スクリーン座標に合わせる
    // 座標をwで割る
    float2 uv = sunPos.xy / sunPos.w;
     // 画面の座標-1～1をﾃｸｽﾁｬの座標0～1へ変換
    uv = uv * 0.5f + 0.5f;
    // 画面のY座標は下から上に向かって増えるが、ﾃｸｽﾁｬのY座標は上から下に向かって増えるので反転
    uv.y = 1.0f - uv.y;
    return uv;
}

// 奥行き確認(第二引数よりpixelの奥行きが上なら1)
// 1か0が返る
float IsNear(float4 pixelPos, float sunDepth)
{
    float ret = 0.0f;
    float pixelDepth = pixelPos.z / pixelPos.w;
    ret = step(sunDepth, pixelDepth);
    return ret;
}

//==========================================================
// ﾃｸｽﾁｬ/ｻﾝﾌﾟﾗ
//==========================================================
Texture2D g_texture : register(t0);         // テクスチャ
Texture2D g_texEmissive : register(t1);     // 発光テクスチャ
Texture2D g_texTransparent : register(t2);  // 透過テクスチャ
Texture2D g_texSpecular : register(t3);     // 鏡面反射テクスチャ
Texture2D g_texSunView : register(t4);      // 影ﾃｸｽﾁｬ
Texture2D g_rampTexture : register(t5);     // Toon用

SamplerState g_sampler : register(s0);      // サンプラ
