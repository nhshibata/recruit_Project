//=========================================================
// Assimp用ピクセルシェーダ (AssimpPixel.hlsl)
//---------------------------------------------------------
// 更新:2023/02/05 定数管理のため、hlsli追加
//=========================================================

#include "common.hlsli"
//#include <common.hlsli>

// パラメータ
struct VS_INPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
    
};

Texture2D    g_texture			: register(t0);	// テクスチャ
Texture2D    g_texEmissive		: register(t1);	// 発光テクスチャ
Texture2D    g_texTransparent	: register(t2);	// 透過テクスチャ
Texture2D    g_texSpecular		: register(t3);	// 鏡面反射テクスチャ
SamplerState g_sampler			: register(s0);	// サンプラ

//
// ピクセルシェーダ
//
float4 main(VS_INPUT input) : SV_Target0
{
	float3 Diff = g_Diffuse.rgb;
	float Alpha = g_Diffuse.a;
    float3 Ambi = g_Ambient.rgb;
     // 拡散色テクスチャ有無
    {
        float x = any(g_Flags.x);
        float4 TexDiff = g_texture.Sample(g_sampler, input.Tex);
        Diff =  IF_SUB(Diff, TexDiff.rgb, x);
        Ambi =  IF_SUB(Ambi, TexDiff.rgb, x);
        Alpha = IF_SUB(Alpha, TexDiff.a, x);
       
    }
    
    // 発光色テクスチャ有無
    {
        int z = any(g_Flags.z);
        float4 TexTran = g_texTransparent.Sample(g_sampler, input.Tex);
        Alpha = IF_SUB(Alpha, TexTran.r * TexTran.g * TexTran.b * TexTran.a, z);
    }
    clip(Alpha - 1.0f); // 完全透明なら描画しない
    
	// 透過テクスチャ有無
    float3 Spec = g_Specular.rgb;
    {
        float4 TexSpec = g_texSpecular.Sample(g_sampler, input.Tex);
        int w = any(g_Flags.w);
        Spec = IF_SUB(Spec, TexSpec.rgb, w);
    }

  {
        int nLight = any(g_vLightDir);
        float3 L = normalize(-g_vLightDir.xyz);                  // 光源へのベクトル
        float3 N = normalize(input.Normal);                      // 法線ベクトル
        float3 V = normalize(g_vCameraPos.xyz - input.PosForPS); // 視点へのベクトル
        float3 H = normalize(L + V);                             // ハーフベクトル
        Diff = nLight ? g_vLightAmbient.rgb * g_Ambient.rgb +
			g_vLightDiffuse.rgb * Diff * saturate(dot(L, N)) : Diff; // 拡散色 + 環境色
        Spec = nLight ? Spec * g_vLightSpecular.rgb *
			pow(saturate(dot(N, H)), g_Specular.a) : Spec; // 鏡面反射色
        Diff += Spec;
    }

	float3 Emis = g_Emissive.rgb;
	{   // テクスチャ有無
        int y = any(g_Flags.y);
		float4 TexEmis = g_texEmissive.Sample(g_sampler, input.Tex);
        Emis = IF_SUB(Emis, TexEmis.rgb * TexEmis.a, y);
    }
	Diff += Emis;

	return float4(Diff, Alpha);
}
