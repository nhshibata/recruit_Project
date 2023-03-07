

#include <Common.hlsli>

// パラメータ
struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float4 Diffuse : COLOR0;
};

float4 main(VS_OUTPUT pin) : SV_TARGET
{
    float4 color = g_ColorTexture.Sample(g_sampler, pin.TexCoord);
 
     // 内積
    // 同じ方向のベクトル同士なら正の値
    // 垂直方向のベクトル同士なら0
    // 反対方向のベクトル同士なら負の値
    
    // オブジェクトの法線とライトのベクトルから内積を計算
    // そのまま計算すると明るくなる部分が負の値になるので、
    //「あえて」ライトのベクトルを反転させる
    float4 normal = g_NormalTexture.Sample(g_sampler, pin.TexCoord);
    float3 N = normalize(normal.xyz);
    float3 L = normalize(-g_vLightDir.xyz);
    
    // 拡散光(diffuse)物体の表面に直接あたる光、物体の色を表す
    float diffuse = dot(N, L);

    // 内積の値を基にランプﾃｸｽﾁｬの色を引っ張り出す
    // ※内積の結果は-1～1なので0～1に変換する
    diffuse = diffuse * 0.5f + 0.5f;
    
    float4 toonColor = g_rampTexture.Sample(g_sampler, float2(diffuse, 0));
    
    color.rgb *= toonColor.rgb;
 
    return color;
}