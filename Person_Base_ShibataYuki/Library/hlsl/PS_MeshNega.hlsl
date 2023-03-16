//=========================================================
// [PS_MeshToon]
//---------------------------------------------------------
// çÏê¨:2023/02/09
//---------------------------------------------------------
// ÉlÉK
//=========================================================

#include <meshCommon.hlsli>

float4 main(PS_INPUT input) : SV_Target0
{
    // meshÇÃã§í ä÷êî
    MeshOutput vin;
    vin.Position = input.Pos;
    vin.Pos4PS = input.PosForPS;
    vin.Diffuse = input.Diffuse;
    vin.TexCoord = input.Tex;
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
