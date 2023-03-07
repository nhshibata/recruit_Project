//=========================================================
// [PS_AssimpToon]
//---------------------------------------------------------
// 作成:2023/02/09
//---------------------------------------------------------
// 階調
//=========================================================

#include <modelCommon.hlsli>

float4 main(PS_INPUT input) : SV_Target0
{
    ModelOutput model;
    model.Pos = input.Pos;
    model.Normal = input.Normal;
    model.Tex = input.Tex;
    model.PosForPS = input.PosForPS;
    float4 Diff = CalcMaterial(g_material[input.id].fAmbient, g_material[input.id].fDiffuse, g_material[input.id].fSpecular, g_material[input.id].fEmissive, model);
    
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