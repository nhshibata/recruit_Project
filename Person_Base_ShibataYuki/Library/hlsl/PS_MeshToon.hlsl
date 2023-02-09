//=========================================================
// [PS_MeshToon]
//---------------------------------------------------------
// 作成:2023/02/09
//---------------------------------------------------------
// 階調
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
    
     // 通常ﾃｸｽﾁｬ取得
    //float4 color = g_texture.Sample(g_sampler, input.TexCoord);
    
    // 内積
    // 同じ方向のベクトル同士なら正の値
    // 垂直方向のベクトル同士なら0
    // 反対方向のベクトル同士なら負の値
    
    // オブジェクトの法線とライトのベクトルから内積を計算
    // そのまま計算すると明るくなる部分が負の値になるので、
    //「あえて」ライトのベクトルを反転させる
    float3 N = normalize(input.Normal);
    float3 L = normalize(-g_vLightDir.xyz);
    
    // 拡散光(diffuse)物体の表面に直接あたる光、物体の色を表す
    float diffuse = dot(N, L);

    // 内積の値を基にランプﾃｸｽﾁｬの色を引っ張り出す
    // ※内積の結果は-1～1なので0～1に変換する
    diffuse = diffuse * 0.5f + 0.5f;
    
    float4 toonColor = g_rampTexture.Sample(g_sampler, float2(diffuse, 0));
    
    Diff.rgb *= toonColor.rgb;

    return Diff;
}
