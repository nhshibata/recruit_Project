//=========================================================
// [PS_GBuffer.hlsl]
//=========================================================

#include <Common.hlsli>

struct PS_OUT
{
    // 出力先に合わせて、セマンティクスに数字を振る
    float4 color    : SV_TARGET0;
    float4 normal   : SV_TARGET1;
    float4 wolrd    : SV_TARGET2;
    float  depth     : SV_TARGET3;
};

// 設定された画面表示用のレンダーターゲットに色を渡す
//PS_OUT main(PS_INPUT pin, out float4 screenColor : SV_Target4)
PS_OUT main(PS_INPUT pin)
{
    PS_OUT pout;
    
    // 色情報書き込み
    pout.color = g_material[pin.id].fDiffuse;
    if (any(g_material[pin.id].fAmbient.a))
        pout.color *= g_texture.Sample(g_sampler, pin.Tex);
    
    // 法線
    // 法線の情報は-1～1だが、ﾃｸｽﾁｬは0～1で扱うため、-1～1 → 0～1への変換が必要
    pout.normal = float4(pin.Normal * 0.5f + 0.5f, 1.0f);
    
    // ﾜｰﾙﾄﾞ座標の書き込み
    // →深度値を書き込む方がよい
    // ﾜｰﾙﾄﾞ座標だとx,y,zの3成分が不調だが、書き込み先は色のrgb各1byteしかない。
    // 深度値からワールド座標を復元することが出来るので、4byteつかって、正確に保存できる
    //pout.wolrd = pin.wPos; ↓
    float depthZ = pin.PosForPS.z / pin.PosForPS.w;
    pout.wolrd = float4(depthZ, depthZ, depthZ, depthZ);
    
    float depthSun = pin.sunPos.z / pin.sunPos.w;
    pout.depth = depthSun; 
    //pout.depth = pin.Pos.z / pin.Pos.w;
    
    return pout;
}
