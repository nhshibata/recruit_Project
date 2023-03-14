//==========================================================
// [DepthOfField.hlsl]
//---------------------------------------------------------
// 被写界深度
//==========================================================

#include <Common.hlsli>

// ピクセルシェーダーへの入力
struct PSIn
{
    float4 pos : SV_POSITION; // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL; // 法線
    float3 tangent : TANGENT; // 接ベクトル
    float3 biNormal : BINORMAL; // 従法線ベクトル
    float2 uv : TEXCOORD0; // UV座標
    float3 worldPos : TEXCOORD1; // ワールド空間でのピクセルの座標

    // step-7 カメラ空間でのZ値を記録する変数を追加
    float3 depthInView : TEXCOORD2; // カメラ空間でのZ値
};

//float4 CalcPBR(PSIn psIn)
//{
//    // 法線を計算
//    float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

//    // step-2 アルベドカラー、スペキュラカラー、金属度をサンプリングする
//    // アルベドカラー（拡散反射光）
//    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
//    // スペキュラカラー（鏡面反射光）
//    float3 specColor = g_specularMap.SampleLevel(g_sampler, psIn.uv, 0).rgb;
//    // 金属度
//    float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;

//    // 視線に向かって伸びるベクトルを計算する
//    float3 toEye = normalize(eyePos - psIn.worldPos);

//    float3 lig = 0;
//    for (int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++)
//    {
//        // step-3 ディズニーベースの拡散反射を実装する
//        // フレネル反射を考慮した拡散反射を計算
//        float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -directionalLight[ligNo].direction, toEye);
//        // 正規化Lambert拡散反射を求める
//        float NdotL = saturate(dot(normal, -directionalLight[ligNo].direction));
//        float3 lambertDiffuse = directionalLight[ligNo].color * NdotL;
//        // 最終的な拡散反射光を計算する
//        float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

//        // step-6 クックトランスモデルを利用した鏡面反射率を計算する
//        // クックトランスモデルの鏡面反射率を計算する
//        float3 spec = CookTrranceSpecular(-directionalLight[ligNo].direction, toEye, normal, metaric) * directionalLight[ligNo].color;
//        // 金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白
//        // スペキュラカラーの強さを鏡面反射率として扱う
//        float specTerm = length(specColor.xyz);
//        spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

//        // step-7 鏡面反射率を使って、拡散反射光と鏡面反射光を合成する
//        // 鏡面反射率が高ければ、拡散反射は弱くなる
//        lig += diffuse * (1.0f - specTerm) + spec;

//    }

//    // 環境光による底上げ
//    lig += ambientLight * albedoColor;

//    float4 finalColor = 1.0f;
//    finalColor.xyz = lig;
//    return finalColor;
//}


float4 main(PSIn pin) : SV_TARGET
{
	 //// step-12 ボケ画像書き込み用のピクセルシェーダーを実装
  //  // カメラ空間での深度値をサンプリング
  //  float depth = depthTexture.Sample(Sampler, In.uv);

  //  // カメラ空間での深度値が200以下ならピクセルキル
  //  //      -> ボケ画像を描きこまない
  //  clip(depth - 200.0f);

  //  // ボケ画像をサンプリング
  //  float4 boke = bokeTexture.Sample(Sampler, In.uv);

  //  // 深度値から不透明度を計算する
  //  // 深度値800からボケが始まり、深度値2000で最大のボケ具合になる
  //  //  -> つまり、深度値2000で不透明度が1になる
  //  boke.a = min(1.0f, (depth - 800.0f) / 2000.0f);

  //  // ボケ画像を出力
  //  return boke;
    return float4(0, 0, 0, 0);
}