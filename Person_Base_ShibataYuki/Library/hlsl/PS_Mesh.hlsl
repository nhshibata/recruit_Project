//=========================================================
// [PS_Mesh]
//---------------------------------------------------------
// 更新:2023/01/25 影追加
// 更新:2023/02/06 名前変更
//---------------------------------------------------------
// mesh用
//=========================================================

#include <meshCommon.hlsli>

//Texture2D    g_texture    : register(t0);	// テクスチャ
//Texture2D    g_texSunView : register(t1);   // 太陽ﾃｸｽﾁｬ
//SamplerState g_sampler    : register(s0);	// サンプラ

float4 main(PS_INPUT input) : SV_Target0
{
    // meshの共通関数
    MeshOutput vin;
    vin.Position = input.Pos;
    vin.Pos4PS = input.PosForPS;
    vin.Diffuse = input.Diffuse;
    vin.TexCoord = input.Tex;
    vin.Normal = input.Normal;
    float4 Diff = CalcPSMaterial(vin, g_material[input.id].fAmbient, g_material[input.id].fDiffuse,
                    g_material[input.id].fSpecular, g_material[input.id].fEmissive, g_material[input.id].fFlags);

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

    // シャドウマップ
    float2 shadowUV = GetSunUV(input.sunPos);
    float result = 1.0f;
    //// 0以上
    //result *= step(0.0f, shadowUV.x);
    //result *= step(0.0f, shadowUV.y);
    //// 1以下
    //result *= step(shadowUV.x, 1.0f + 0.0001f);
    //result *= step(shadowUV.y, 1.0f + 0.0001f);
    result *= step(0.0f, shadowUV.x) * (1.0f - step(1.0f, shadowUV.x));
    result *= step(0.0f, shadowUV.y) * (1.0f - step(1.0f, shadowUV.y));
    shadowRes *= result;
    // 0なら色変更なし、1なら影を影響反映
    shadowRes = 1.0f - shadowRes;
    shadowRes = saturate(shadowRes + 0.3f * shadowRes);
    
    //--- 影結果反映
    // 影なら全て黒
    Diff.rgb *= shadowRes;
    // 影ならば描画
    //Alpha = shadowRes;
    
	return Diff;
}

