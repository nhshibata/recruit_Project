//==========================================================
// [DepthWritePS]
//---------------------------------------------------------
// 作成:2023/01/22
//---------------------------------------------------------
//
//==========================================================

struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 screenPos : TEXCOORD0;
};


float main(PS_IN pin) : SV_TARGET
{
    // スクリーン座標
    float depth = pin.screenPos.z / pin.screenPos.w;

    return depth;
}