//=========================================================
// Assimp用ピクセルシェーダ (AssimpPixel.hlsl)
//---------------------------------------------------------
// 更新:2023/01/23 授業でシェーダー慣れてきた&ifが気になったので改修
// 更新:2023/01/25 影追加
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
Texture2D    g_texSunView       : register(t4); // 太陽ﾃｸｽﾁｬ
SamplerState g_sampler			: register(s0);	// サンプラ

// パラメータ
struct VS_INPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
    float4  sunPos      : TEXCOORD3; // 太陽から見た位置
};

#define IF_SUB(x,y,z)      (x + ((x * y - x) * z))


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

// 奥行き確認
// 1か0が返る
float IsNear(float4 sunPos, float sunDepth)
{
    float ret = 0.0f;
    float pixelDepth = sunPos.z / sunPos.w;
    ret = step(sunDepth, pixelDepth);
    return ret;
}

//
// ピクセルシェーダ
//
float4 main(VS_INPUT input) : SV_Target0
{
    // 影
    // 奥行き情報を書き込んだﾃｸｽﾁｬから、距離を取得
    float sunDepth = g_texSunView.Sample(g_sampler, GetSunUV(input.sunPos)).r;
    // シャドウアクネ(マッハバンド)対策
    // 描画するピクセルの奥行きと、ﾃｸｽﾁｬの奥行きが一致していた場合、
    // ﾃｸｽﾁｬに書き込まれたデータとの差異が発生して影が出来ない部分に
    // 影が出来てしまう。ﾃｸｽﾁｬに書き込まれた距離は実際は少し遠い、と置くことで回避できる
    sunDepth += 0.005f; // 数字は適時調整

    // 奥行き確認
    // pixelの方が奥なら1.0f、つまり影
    float shadowRes = IsNear(input.sunPos, sunDepth);
    
    
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
    
    //--- 影結果反映
    // 影なら全て黒
    Diff.rgb *= 1.0f - shadowRes;
    
    // 影ならば描画
    //Alpha = shadowRes;

	return float4(Diff, Alpha);
}
