//=========================================================
// mesh用
//---------------------------------------------------------
// ピクセルシェーダ
//=========================================================

// グローバル
cbuffer global : register(b11) {
	float4	g_vEye;			// 視点座標
	// 光源
	float4	g_vLightDir;	// 光源方向
	float4	g_vLa;			// 環境光
	float4	g_vLd;			// 拡散反射光
	float4	g_vLs;			// 鏡面反射光
	// マテリアル
	float4	g_vKa;			// アンビエント色(+テクスチャ有無)
	float4	g_vKd;			// ディフューズ色
	float4	g_vKs;			// スペキュラ色(+スペキュラ強度)
	float4	g_vKe;			// エミッシブ色
};

// パラメータ
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float2	TexCoord	: TEXCOORD2;
	float4	Diffuse		: COLOR0;
};

Texture2D    g_texture    : register(t0);	// テクスチャ
SamplerState g_sampler    : register(s0);	// サンプラ

//--- 定数定義
#define IF_SUB(x,y,z)      (x + ((x * y - x) * z))


float4 main(VS_OUTPUT input) : SV_Target0
{
	float3 Diff = input.Diffuse.rgb * g_vKd.rgb;
	float3 Ambi = input.Diffuse.rgb * g_vKa.rgb;
	float Alpha = input.Diffuse.a * g_vKd.a;
    
    //--- アンビエントのα値0より上 == ﾃｸｽﾁｬあり
    //float fTex = step(0.0f, g_vKa.a);
    float fTex = any(g_vKa.a);
    //if (0.0f < g_vKa.a)
    //if (step(0.0f, g_vKa.a))
    {
		// テクスチャ有
        float4 vTd = g_texture.Sample(g_sampler, input.TexCoord);
        //Diff *= vTd.rgb;
        //Ambi *= vTd.rgb;
        //Alpha *= vTd.a;
        Diff = IF_SUB(Diff, vTd.rgb, fTex);
        Ambi = IF_SUB(Ambi, vTd.rgb, fTex);
        Alpha = IF_SUB(Alpha, vTd.a, fTex);
    }

    //if (Alpha <= 0.0f)
       // discard;
	clip(Alpha - 0.001f);

    float3 workDiff = Diff;
    //if (g_vLightDir.x != 0.0f || g_vLightDir.y != 0.0f || g_vLightDir.z != 0.0f)
    {
		// 光源有効
		float3 L = normalize(-g_vLightDir.xyz);				// 光源へのベクトル
		float3 N = normalize(input.Normal);					// 法線ベクトル
		float3 V = normalize(g_vEye.xyz - input.Pos4PS);	// 視点へのベクトル
		float3 H = normalize(L + V);						// ハーフベクトル
        workDiff = g_vLa.rgb * Ambi + g_vLd.rgb *
			Diff * saturate(dot(L, N));						// 拡散色 + 環境色
		float3 Spec = g_vLs.rgb * g_vKs.rgb *
			pow(saturate(dot(N, H)), g_vKs.a);				// 鏡面反射色
        workDiff += Spec;
    }
    
    Diff = any(g_vLightDir) ? workDiff : Diff;
    
	Diff += g_vKe.rgb;
    
	return float4(Diff, Alpha);
}

