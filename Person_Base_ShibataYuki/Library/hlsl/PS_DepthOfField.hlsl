//==========================================================
// [DepthOfField.hlsl]
//---------------------------------------------------------
// ��ʊE�[�x
//==========================================================

#include <Common.hlsli>

// �s�N�Z���V�F�[�_�[�ւ̓���
struct PSIn
{
    float4 pos : SV_POSITION; // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL; // �@��
    float3 tangent : TANGENT; // �ڃx�N�g��
    float3 biNormal : BINORMAL; // �]�@���x�N�g��
    float2 uv : TEXCOORD0; // UV���W
    float3 worldPos : TEXCOORD1; // ���[���h��Ԃł̃s�N�Z���̍��W

    // step-7 �J������Ԃł�Z�l���L�^����ϐ���ǉ�
    float3 depthInView : TEXCOORD2; // �J������Ԃł�Z�l
};

//float4 CalcPBR(PSIn psIn)
//{
//    // �@�����v�Z
//    float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);

//    // step-2 �A���x�h�J���[�A�X�y�L�����J���[�A�����x���T���v�����O����
//    // �A���x�h�J���[�i�g�U���ˌ��j
//    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
//    // �X�y�L�����J���[�i���ʔ��ˌ��j
//    float3 specColor = g_specularMap.SampleLevel(g_sampler, psIn.uv, 0).rgb;
//    // �����x
//    float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;

//    // �����Ɍ������ĐL�т�x�N�g�����v�Z����
//    float3 toEye = normalize(eyePos - psIn.worldPos);

//    float3 lig = 0;
//    for (int ligNo = 0; ligNo < NUM_DIRECTIONAL_LIGHT; ligNo++)
//    {
//        // step-3 �f�B�Y�j�[�x�[�X�̊g�U���˂���������
//        // �t���l�����˂��l�������g�U���˂��v�Z
//        float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -directionalLight[ligNo].direction, toEye);
//        // ���K��Lambert�g�U���˂����߂�
//        float NdotL = saturate(dot(normal, -directionalLight[ligNo].direction));
//        float3 lambertDiffuse = directionalLight[ligNo].color * NdotL;
//        // �ŏI�I�Ȋg�U���ˌ����v�Z����
//        float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

//        // step-6 �N�b�N�g�����X���f���𗘗p�������ʔ��˗����v�Z����
//        // �N�b�N�g�����X���f���̋��ʔ��˗����v�Z����
//        float3 spec = CookTrranceSpecular(-directionalLight[ligNo].direction, toEye, normal, metaric) * directionalLight[ligNo].color;
//        // �����x��������΁A���ʔ��˂̓X�y�L�����J���[�A�Ⴏ��Δ�
//        // �X�y�L�����J���[�̋��������ʔ��˗��Ƃ��Ĉ���
//        float specTerm = length(specColor.xyz);
//        spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

//        // step-7 ���ʔ��˗����g���āA�g�U���ˌ��Ƌ��ʔ��ˌ�����������
//        // ���ʔ��˗���������΁A�g�U���˂͎キ�Ȃ�
//        lig += diffuse * (1.0f - specTerm) + spec;

//    }

//    // �����ɂ���グ
//    lig += ambientLight * albedoColor;

//    float4 finalColor = 1.0f;
//    finalColor.xyz = lig;
//    return finalColor;
//}


float4 main(PSIn pin) : SV_TARGET
{
	 //// step-12 �{�P�摜�������ݗp�̃s�N�Z���V�F�[�_�[������
  //  // �J������Ԃł̐[�x�l���T���v�����O
  //  float depth = depthTexture.Sample(Sampler, In.uv);

  //  // �J������Ԃł̐[�x�l��200�ȉ��Ȃ�s�N�Z���L��
  //  //      -> �{�P�摜��`�����܂Ȃ�
  //  clip(depth - 200.0f);

  //  // �{�P�摜���T���v�����O
  //  float4 boke = bokeTexture.Sample(Sampler, In.uv);

  //  // �[�x�l����s�����x���v�Z����
  //  // �[�x�l800����{�P���n�܂�A�[�x�l2000�ōő�̃{�P��ɂȂ�
  //  //  -> �܂�A�[�x�l2000�ŕs�����x��1�ɂȂ�
  //  boke.a = min(1.0f, (depth - 800.0f) / 2000.0f);

  //  // �{�P�摜���o��
  //  return boke;
    return float4(0, 0, 0, 0);
}