//=========================================================
// [meshCommon]
//---------------------------------------------------------
//
//
//=========================================================

#include "common.hlsli"

// パラメータ
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
    
    uint id : SV_InstanceID; // インスタンスID
};

struct MeshOutput
{
    float4 Position : SV_Position;
    float4 Pos4PS   : TEXCOORD0;
    float3 Normal   : TEXCOORD1;
    float2 TexCoord : TEXCOORD2;
    float4 Diffuse  : COLOR0;
};

MeshOutput CalcMesh(VS_INPUT input, matrix world, matrix view, matrix proj)
{
    MeshOutput output;
    float4 wPos = mul(float4(input.Position, 1.0f), world);
    float4x4 VP = mul(view, proj);
    output.Position = mul(wPos, VP);
    output.Pos4PS = wPos;
    output.Normal = mul(float4(input.Normal, 0.0f), world).xyz;
    output.TexCoord = mul(float4(input.TexCoord, 0.0f, 1.0f), g_mTexture).xy;
    output.Diffuse = input.Diffuse;
    return output;
}

/*
 float4 wPos = mul(float4(input.Position, 1.0f), g_Instancing[id].mWorld);
    float4x4 VP = mul(g_view, g_proj);
    output.Position = mul(wPos, VP);
    output.Pos4PS = wPos.xyz;
    output.Normal = mul(float4(input.Normal, 0.0f), g_Instancing[id].mWorld).xyz;
    output.TexCoord = mul(float4(input.TexCoord, 0.0f, 1.0f), g_mTexture).xy;
	output.Diffuse = input.Diffuse;
*/

float4 CalcPSMaterial(MeshOutput input, float4 ambi, float4 diff, float4 spec, float4 emi, float4 flag)
{
    float3 Diff = input.Diffuse.rgb * diff.rgb;
    float3 Ambi = input.Diffuse.rgb * ambi.rgb;
    float Alpha = input.Diffuse.a * diff.a;
    
    //--- アンビエントのα値0より上 == ﾃｸｽﾁｬあり
    //float fTex = step(0.0f, g_vKa.a);
    float fTex = any(ambi.a);
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

    // ligthいずれかが0以外、かつ、flg.rが0の時適用
    if (any(g_vLightDir) & !any(flag.r))
    {
		// 光源有効
        float3 L = normalize(-g_vLightDir.xyz); // 光源へのベクトル
        float3 N = normalize(input.Normal); // 法線ベクトル
        float3 V = normalize(g_vCameraPos.xyz - input.Pos4PS.xyz); // 視点へのベクトル
        float3 H = normalize(L + V); // ハーフベクトル
        float4 Spec = spec;
        Diff = g_vLightAmbient.rgb * Ambi + g_vLightDiffuse.rgb *
			Diff * saturate(dot(L, N)); // 拡散色 + 環境色
        float3 newSpec = g_vLightSpecular.rgb * Spec.rgb *
			pow(saturate(dot(N, H)), Spec.a); // 鏡面反射色
        Diff += newSpec;
    }
    
    Diff += emi.rgb;
    return float4(Diff, Alpha);
}