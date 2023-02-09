//==========================================================
// [modelCommon]
//---------------------------------------------------------
// 作成:2023/02/07 
// 作成:2023/02/08 SkinVert関数のif変更(動的な変数だったため)
//---------------------------------------------------------
// モデルに必要な処理まとめ
//==========================================================

#include "common.hlsli"

// パラメータ
struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD0;
    uint4 Bone : BONE_INDEX; // ボーンのインデックス
    float4 Weight : BONE_WEIGHT; // ボーンの重み
    
    uint id : SV_InstanceID; // インスタンスID
};

// スキニング後の頂点・法線
struct SKIN
{
    float4 Pos;
    float3 Norm;
};

// 頂点をスキニング (ボーンにより移動)
SKIN SkinVert(VS_INPUT input)
{
    SKIN output;
    float4 Pos = float4(input.Pos, 1.0f);
    float3 Norm = input.Normal;
    
    // 一度計算
    {
		// ボーン0
        uint uBone = input.Bone.x;
        float fWeight = input.Weight.x;
        matrix m = g_BoneWorld[uBone];
        output.Pos = fWeight * mul(Pos, m);
        output.Norm = fWeight * mul(Norm, (float3x3) m);
		// ボーン1
        uBone = input.Bone.y;
        fWeight = input.Weight.y;
        m = g_BoneWorld[uBone];
        output.Pos += fWeight * mul(Pos, m);
        output.Norm += fWeight * mul(Norm, (float3x3) m);
		// ボーン2
        uBone = input.Bone.z;
        fWeight = input.Weight.z;
        m = g_BoneWorld[uBone];
        output.Pos += fWeight * mul(Pos, m);
        output.Norm += fWeight * mul(Norm, (float3x3) m);
		// ボーン3
        uBone = input.Bone.w;
        fWeight = input.Weight.w;
        m = g_BoneWorld[uBone];
        output.Pos += fWeight * mul(Pos, m);
        output.Norm += fWeight * mul(Norm, (float3x3) m);
    }
    
    // weightが0か(0なら0,0でなければ1)
    float isWeight = any(input.Weight.x);
    // ボーン無し
    // 0.0fならそのまま、1.0fなら適用
    output.Pos = lerp(Pos, output.Pos, isWeight);
    output.Norm = lerp(Norm, output.Norm, isWeight);
    
    return output;
}


struct ModelOutput
{
    float4 Pos : SV_Position;
    float2 Tex : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 PosForPS : TEXCOORD2;
};

ModelOutput SkinModel(VS_INPUT vin, float4x4 mtx, matrix world, matrix view, matrix proj, matrix mTexture)
{
    ModelOutput vout;

    SKIN vSkinned = SkinVert(vin);
    float4x4 mWorld = mul(world, mtx);
    float4x4 VP = mul(view, proj);
    float4x4 WVP = mul(mWorld, VP);
    vout.Pos = mul(vSkinned.Pos, WVP);
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), mTexture).xy;
    vout.Normal = mul(vSkinned.Norm, (float3x3) mWorld);
    vout.PosForPS = mul(vSkinned.Pos, mWorld).xyz;

    return vout;
}

/*
    SKIN vSkinned = SkinVert(input);
    float4x4 mWorld = mul(g_world, g_Instancing[input.id].mWorld);
    float4x4 VP = mul(g_view, g_proj);
    output.Pos = mul(vSkinned.Pos, mul(mWorld, VP));
    output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), g_mtxTexture).xy;
    output.Normal = mul(vSkinned.Norm, (float3x3) mWorld);
    output.PosForPS = mul(vSkinned.Pos, mWorld).xyz;
*/

float4 CalcMaterial(float4 ambi, float4 diff, float4 spec, float4 emi, ModelOutput input)
{
    float3 Diff = g_Diffuse.rgb * diff.rgb;
    float3 Ambi = g_Ambient.rgb * ambi.rgb;
    float Alpha = g_Diffuse.a * diff.a;
    
    // 拡散色テクスチャ有無
    {
        float x = any(g_Flags.x); // not zero == 1
        float4 TexDiff = g_texture.Sample(g_sampler, input.Tex);
        Diff = IF_SUB(Diff, TexDiff.rgb, x);
        Ambi = IF_SUB(Ambi, TexDiff.rgb, x);
        Alpha = IF_SUB(Alpha, TexDiff.a, x);
    }
    
    // 発光色テクスチャ有無
    {
        int z = any(g_Flags.z);
        float4 TexTran = g_texTransparent.Sample(g_sampler, input.Tex);
        Alpha = IF_SUB(Alpha, TexTran.r * TexTran.g * TexTran.b * TexTran.a, z);
    }
    clip(Alpha - 0.0001f); // 完全透明なら描画しない
    
	// 透過テクスチャ有無
    float3 Spec = g_Specular.rgb * spec.rgb;
    {
        float4 TexSpec = g_texSpecular.Sample(g_sampler, input.Tex);
        int w = any(g_Flags.w);
        Spec = IF_SUB(Spec, TexSpec.rgb, w);
    }
    
    //--- 光
    // xyzいずれかが0以外なら
    // Lightは定数なので動的ではない
    if (any(g_vLightDir)) 
    {
        float3 L = normalize(-g_vLightDir.xyz);                 // 光源へのベクトル
        float3 N = normalize(input.Normal);                     // 法線ベクトル
        float3 V = normalize(g_vCameraPos.xyz - input.PosForPS); // 視点へのベクトル
        float3 H = normalize(L + V); // ハーフベクトル
        float3 mulAmbi = ambi.rgb * g_Ambient.rgb;
        float mulSpec = g_Specular.a * spec.a;
        Diff = g_vLightAmbient.rgb * mulAmbi.rgb + g_vLightDiffuse.rgb * Diff * saturate(dot(L, N)); // 拡散色 + 環境色
        Spec = Spec * g_vLightSpecular.rgb * pow(saturate(dot(N, H)), mulSpec); // 鏡面反射色
        Diff += Spec;
    }

    float3 Emis = g_Emissive.rgb * emi.rgb;
	{   // テクスチャ有無
        int y = any(g_Flags.y);
        float4 TexEmis = g_texEmissive.Sample(g_sampler, input.Tex);
        Emis = IF_SUB(Emis, TexEmis.rgb * TexEmis.a, y);
    }
    Diff += Emis;
    
    return float4(Diff, Alpha);
}

/*
    float3 Diff = g_Diffuse.rgb * g_material[input.id].fDiffuse.rgb;
    float3 Ambi = g_Ambient.rgb * g_material[input.id].fAmbient.rgb;
    float Alpha = g_Diffuse.a * g_material[input.id].fDiffuse.a;
     // 拡散色テクスチャ有無
    {
        float x = any(g_Flags.x); // not zero == 1
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
    clip(Alpha - 0.0001f); // 完全透明なら描画しない
    
	// 透過テクスチャ有無
    float3 Spec = g_Specular.rgb * g_material[input.id].fSpecular.rgb;
    {
        float4 TexSpec = g_texSpecular.Sample(g_sampler, input.Tex);
        int w = any(g_Flags.w);
        Spec = IF_SUB(Spec, TexSpec.rgb, w);
    }
    
    //--- 光
    float3 workDiff = Diff;
    {
        float3 L = normalize(-g_vLightDir.xyz);                  // 光源へのベクトル
        float3 N = normalize(input.Normal);                      // 法線ベクトル
        float3 V = normalize(g_vCameraPos.xyz - input.PosForPS); // 視点へのベクトル
        float3 H = normalize(L + V);                             // ハーフベクトル
        float3 mulAmbi = g_material[input.id].fAmbient.rgb * g_Ambient.rgb;
        float spec = g_Specular.a * g_material[input.id].fSpecular.a;
        workDiff = g_vLightAmbient.rgb * mulAmbi.rgb + g_vLightDiffuse.rgb * Diff * saturate(dot(L, N)); // 拡散色 + 環境色
        Spec = Spec * g_vLightSpecular.rgb * pow(saturate(dot(N, H)), spec); // 鏡面反射色
        workDiff += Spec;
    }
    Diff = any(g_vLightDir) ? workDiff : Diff;

    float3 Emis = g_Emissive.rgb * g_material[input.id].fEmissive.rgb;
	{   // テクスチャ有無
        int y = any(g_Flags.y);
		float4 TexEmis = g_texEmissive.Sample(g_sampler, input.Tex);
        Emis = IF_SUB(Emis, TexEmis.rgb * TexEmis.a, y);
    }
	Diff += Emis;
    

*/