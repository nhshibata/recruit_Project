//=========================================================
// Assimp用ピクセルシェーダ (AssimpPixel.hlsl)
//---------------------------------------------------------
//=========================================================

// グローバル
cbuffer global : register(b0) {
	matrix	g_mtxWVP;			// ワールド×ビュー×射影行列
	matrix	g_mtxWorld;			// ワールド行列
	matrix	g_mtxTexture;		// テクスチャ行列
	float4	g_vCameraPos;		// 視点座標(ワールド空間)
	float4	g_vLightDir;		// 光源方向(ワールド空間)
	float4	g_vLightAmbient;	// 環境光
	float4	g_vLightDiffuse;	// 拡散光
	float4	g_vLightSpecular;	// 鏡面反射光
};

// マテリアル
cbuffer global2 : register(b1) {
	float4	g_Ambient;			// 環境色
	float4	g_Diffuse;			// 拡散色+アルファ
	float4	g_Specular;			// 鏡面反射色+強度
	float4	g_Emissive;			// 発光色
	float4	g_Flags;			// 拡散色テクスチャ有無, 発光色テクスチャ有無, 透過テクスチャ有無
};

Texture2D    g_texture			: register(t0);	// テクスチャ
Texture2D    g_texEmissive		: register(t1);	// 発光テクスチャ
Texture2D    g_texTransparent	: register(t2);	// 透過テクスチャ
Texture2D    g_texSpecular		: register(t3);	// 鏡面反射テクスチャ
SamplerState g_sampler			: register(s0);	// サンプラ

// パラメータ
struct VS_INPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
};

#define IF_SUB(x,y,z)      (x + ((x * y - x) * z))


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
