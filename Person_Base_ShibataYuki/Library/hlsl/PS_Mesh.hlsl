//=========================================================
// [PS_Mesh]
//---------------------------------------------------------
// �X�V:2023/01/25 �e�ǉ�
// �X�V:2023/02/06 ���O�ύX
//---------------------------------------------------------
// mesh�p
//=========================================================

#include "common.hlsli"

// �O���[�o��
cbuffer global : register(b1) {
	float4	g_vEye;			// ���_���W
	// ����
    float4 g_vLightDirect; // ��������
	float4	g_vLa;			// ����
	float4	g_vLd;			// �g�U���ˌ�
	float4	g_vLs;			// ���ʔ��ˌ�
	// �}�e���A��(TODO: �K�v�Ȃ�?)
	float4	g_vKa;			// �A���r�G���g�F(+�e�N�X�`���L��)
	float4	g_vKd;			// �f�B�t���[�Y�F
	float4	g_vKs;			// �X�y�L�����F(+�X�y�L�������x)
	float4	g_vKe;			// �G�~�b�V�u�F
};

// �p�����[�^
struct VS_OUTPUT {
	float4	Position	: SV_Position;
	float3	Pos4PS		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float2	TexCoord	: TEXCOORD2;
	float4	Diffuse		: COLOR0;
    float4  SunPos      : TEXCOORD3; // ���z���猩���ʒu
    
    uint id : SV_InstanceID; // �C���X�^���XID
};

Texture2D    g_texture    : register(t0);	// �e�N�X�`��
Texture2D    g_texSunView : register(t1);   // ���zø���
SamplerState g_sampler    : register(s0);	// �T���v��

//--- �萔��`
#define IF_SUB(x,y,z)      (x + ((x * y - x) * z))

float2 GetSunUV(float4 sunPos)
{
    //--- �X�N���[�����W�ɍ��킹��
    // ���W��w�Ŋ���
    float2 uv = sunPos.xy / sunPos.w;
     // ��ʂ̍��W-1�`1��ø����̍��W0�`1�֕ϊ�
    uv = uv * 0.5f + 0.5f;
    // ��ʂ�Y���W�͉������Ɍ������đ����邪�Aø�����Y���W�͏ォ�牺�Ɍ������đ�����̂Ŕ��]
    uv.y = 1.0f - uv.y;
    return uv;
}

// ���s���m�F
// 1��0���Ԃ�
float IsNear(float4 sunPos, float sunDepth)
{
    float pixelDepth = sunPos.z / sunPos.w;
    return step(sunDepth, pixelDepth);
}

float4 main(VS_OUTPUT input) : SV_Target0
{
    // �e
    // ���s��������������ø�������A�������擾
    float sunDepth = g_texSunView.Sample(g_sampler, GetSunUV(input.SunPos)).r;
    // �V���h�E�A�N�l(�}�b�n�o���h)�΍�
    // �`�悷��s�N�Z���̉��s���ƁAø����̉��s������v���Ă����ꍇ�A
    // ø����ɏ������܂ꂽ�f�[�^�Ƃ̍��ق��������ĉe���o���Ȃ�������
    // �e���o���Ă��܂��Bø����ɏ������܂ꂽ�����͎��ۂ͏��������A�ƒu�����Ƃŉ���ł���
    sunDepth += 0.005f; // �����͓K������

    // ���s���m�F
    // pixel�̕������Ȃ�1.0f�A�܂�e
    float shadowRes = IsNear(input.SunPos, sunDepth);

    float3 Diff = input.Diffuse.rgb * g_material[input.id].fDiffuse.rgb;
    float3 Ambi = input.Diffuse.rgb * g_material[input.id].fAmbient.rgb;
    float Alpha = input.Diffuse.a * g_material[input.id].fDiffuse.a;
    
    //--- �A���r�G���g�̃��l0���� == ø�������
    //float fTex = step(0.0f, g_vKa.a);
    float fTex = any(g_material[input.id].fAmbient.a);
    //if (0.0f < g_vKa.a)
    {
		// �e�N�X�`���L
        float4 vTd = g_texture.Sample(g_sampler, input.TexCoord);
        Diff = IF_SUB(Diff, vTd.rgb, fTex);
        Ambi = IF_SUB(Ambi, vTd.rgb, fTex);
        Alpha = IF_SUB(Alpha, vTd.a, fTex);
    }

    //if (Alpha <= 0.0f)
       // discard;
	clip(Alpha - 0.001f);

    float3 workDiff = Diff;
    //if (g_vLightDir.x != 0.0f || g_vLightDir.y != 0.0f || g_vLightDir.z != 0.0f)
    {
		// �����L��
        float3 L = normalize(-g_vLightDirect.xyz); // �����ւ̃x�N�g��
		float3 N = normalize(input.Normal);					// �@���x�N�g��
		float3 V = normalize(g_vEye.xyz - input.Pos4PS);	// ���_�ւ̃x�N�g��
		float3 H = normalize(L + V);						// �n�[�t�x�N�g��
        float4 spec = g_material[input.id].fSpecular;
        workDiff = g_vLa.rgb * Ambi + g_vLd.rgb *
			Diff * saturate(dot(L, N));						// �g�U�F + ���F
		float3 Spec = g_vLs.rgb * spec.rgb *
			pow(saturate(dot(N, H)), spec.a);				// ���ʔ��ːF
        workDiff += Spec;
    }
    
    Diff = any(g_vLightDirect) ? workDiff : Diff;
    
    Diff += g_material[input.id].fEmissive.rgb;

    //--- �e���ʔ��f
    // �e�Ȃ�S�č�
    Diff.rgb *= 1.0f - shadowRes;
    // �e�Ȃ�Ε`��
    //Alpha = shadowRes;
    
	return float4(Diff, Alpha);
}

