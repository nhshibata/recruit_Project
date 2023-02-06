//=========================================================
// [PS_Mesh]
//---------------------------------------------------------
// 更新:2023/01/25 影追加
// 更新:2023/02/06 名前変更
//---------------------------------------------------------
// mesh用
//=========================================================

#include "common.hlsli"

// グローバル
cbuffer global : register(b1) {
	float4	g_vEye;			// 視点座標
	// 光源
    float4 g_vLightDirect; // 光源方向
	float4	g_vLa;			// 環境光
	float4	g_vLd;			// 拡散反射光
	float4	g_vLs;			// 鏡面反射光
	// マテリアル(TODO: 必要ない?)
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
    float4  SunPos      : TEXCOORD3; // 太陽から見た位置
    
    uint id : SV_InstanceID; // インスタンスID
};

Texture2D    g_texture    : register(t0);	// テクスチャ
Texture2D    g_texSunView : register(t1);   // 太陽ﾃｸｽﾁｬ
SamplerState g_sampler    : register(s0);	// サンプラ

//--- 定数定義
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
    float pixelDepth = sunPos.z / sunPos.w;
    return step(sunDepth, pixelDepth);
}

float4 main(VS_OUTPUT input) : SV_Target0
{
    // 影
    // 奥行き情報を書き込んだﾃｸｽﾁｬから、距離を取得
    float sunDepth = g_texSunView.Sample(g_sampler, GetSunUV(input.SunPos)).r;
    // シャドウアクネ(マッハバンド)対策
    // 描画するピクセルの奥行きと、ﾃｸｽﾁｬの奥行きが一致していた場合、
    // ﾃｸｽﾁｬに書き込まれたデータとの差異が発生して影が出来ない部分に
    // 影が出来てしまう。ﾃｸｽﾁｬに書き込まれた距離は実際は少し遠い、と置くことで回避できる
    sunDepth += 0.005f; // 数字は適時調整

    // 奥行き確認
    // pixelの方が奥なら1.0f、つまり影
    float shadowRes = IsNear(input.SunPos, sunDepth);

    float3 Diff = input.Diffuse.rgb * g_material[input.id].fDiffuse.rgb;
    float3 Ambi = input.Diffuse.rgb * g_material[input.id].fAmbient.rgb;
    float Alpha = input.Diffuse.a * g_material[input.id].fDiffuse.a;
    
    //--- アンビエントのα値0より上 == ﾃｸｽﾁｬあり
    //float fTex = step(0.0f, g_vKa.a);
    float fTex = any(g_material[input.id].fAmbient.a);
    //if (0.0f < g_vKa.a)
    {
		// テクスチャ有
        float4 vTd = g_texture.Sample(g_sampler, input.TexCoord);
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
        float3 L = normalize(-g_vLightDirect.xyz); // 光源へのベクトル
		float3 N = normalize(input.Normal);					// 法線ベクトル
		float3 V = normalize(g_vEye.xyz - input.Pos4PS);	// 視点へのベクトル
		float3 H = normalize(L + V);						// ハーフベクトル
        float4 spec = g_material[input.id].fSpecular;
        workDiff = g_vLa.rgb * Ambi + g_vLd.rgb *
			Diff * saturate(dot(L, N));						// 拡散色 + 環境色
		float3 Spec = g_vLs.rgb * spec.rgb *
			pow(saturate(dot(N, H)), spec.a);				// 鏡面反射色
        workDiff += Spec;
    }
    
    Diff = any(g_vLightDirect) ? workDiff : Diff;
    
    Diff += g_material[input.id].fEmissive.rgb;

    //--- 影結果反映
    // 影なら全て黒
    Diff.rgb *= 1.0f - shadowRes;
    // 影ならば描画
    //Alpha = shadowRes;
    
	return float4(Diff, Alpha);
}

