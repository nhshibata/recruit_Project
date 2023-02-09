//=========================================================
// [PS_MeshToon]
//---------------------------------------------------------
// 作成:2023/02/09
//---------------------------------------------------------
// ネガ
//=========================================================

#include "meshCommon.hlsli"

// パラメータ
struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 Pos4PS : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float2 TexCoord : TEXCOORD2;
    float4 Diffuse : COLOR0;
    float4 SunPos : TEXCOORD3; // 太陽から見た位置
    
    uint id : SV_InstanceID; // インスタンスID
};

float4 main(VS_OUTPUT input) : SV_Target0
{
    // meshの共通関数
    MeshOutput vin;
    vin.Position = input.Position;
    vin.Pos4PS = input.Pos4PS;
    vin.Diffuse = input.Diffuse;
    vin.TexCoord = input.TexCoord;
    vin.Normal = input.Normal;
    float4 Diff = CalcPSMaterial(vin, g_material[input.id].fAmbient, g_material[input.id].fDiffuse,
                    g_material[input.id].fSpecular, g_material[input.id].fEmissive, g_material[input.id].fFlags);
    
    float4 color = Diff;
    float3 negaColor;
    negaColor.r = 1.0f - color.r;
    negaColor.g = 1.0f - color.g;
    negaColor.b = 1.0f - color.b;
    Diff.rgb = lerp(color.rgb, negaColor, g_rate.r);

    return Diff;
}
