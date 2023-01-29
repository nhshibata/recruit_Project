//=========================================================
// Assimp�p�s�N�Z���V�F�[�_ (AssimpPixel.hlsl)
//---------------------------------------------------------
// �X�V:2023/01/23 ���ƂŃV�F�[�_�[����Ă���&if���C�ɂȂ����̂ŉ��C
// �X�V:2023/01/25 �e�ǉ�
//---------------------------------------------------------
//=========================================================

// �O���[�o��
cbuffer global : register(b0) {
	matrix	g_mtxWVP;			// ���[���h�~�r���[�~�ˉe�s��
	matrix	g_mtxWorld;			// ���[���h�s��
	matrix	g_mtxTexture;		// �e�N�X�`���s��
	float4	g_vCameraPos;		// ���_���W(���[���h���)
	float4	g_vLightDir;		// ��������(���[���h���)
	float4	g_vLightAmbient;	// ����
	float4	g_vLightDiffuse;	// �g�U��
	float4	g_vLightSpecular;	// ���ʔ��ˌ�
};

// �}�e���A��
cbuffer global2 : register(b1) {
	float4	g_Ambient;			// ���F
	float4	g_Diffuse;			// �g�U�F+�A���t�@
	float4	g_Specular;			// ���ʔ��ːF+���x
	float4	g_Emissive;			// �����F
	float4	g_Flags;			// �g�U�F�e�N�X�`���L��, �����F�e�N�X�`���L��, ���߃e�N�X�`���L��
};

Texture2D    g_texture			: register(t0);	// �e�N�X�`��
Texture2D    g_texEmissive		: register(t1);	// �����e�N�X�`��
Texture2D    g_texTransparent	: register(t2);	// ���߃e�N�X�`��
Texture2D    g_texSpecular		: register(t3);	// ���ʔ��˃e�N�X�`��
Texture2D    g_texSunView       : register(t4); // ���zø���
SamplerState g_sampler			: register(s0);	// �T���v��

// �p�����[�^
struct VS_INPUT {
	float4	Pos			: SV_Position;
	float2	Tex			: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	PosForPS	: TEXCOORD2;
    float4  sunPos      : TEXCOORD3; // ���z���猩���ʒu
};

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
    float ret = 0.0f;
    float pixelDepth = sunPos.z / sunPos.w;
    ret = step(sunDepth, pixelDepth);
    return ret;
}

//
// �s�N�Z���V�F�[�_
//
float4 main(VS_INPUT input) : SV_Target0
{
    // �e
    // ���s��������������ø�������A�������擾
    float sunDepth = g_texSunView.Sample(g_sampler, GetSunUV(input.sunPos)).r;
    // �V���h�E�A�N�l(�}�b�n�o���h)�΍�
    // �`�悷��s�N�Z���̉��s���ƁAø����̉��s������v���Ă����ꍇ�A
    // ø����ɏ������܂ꂽ�f�[�^�Ƃ̍��ق��������ĉe���o���Ȃ�������
    // �e���o���Ă��܂��Bø����ɏ������܂ꂽ�����͎��ۂ͏��������A�ƒu�����Ƃŉ���ł���
    sunDepth += 0.005f; // �����͓K������

    // ���s���m�F
    // pixel�̕������Ȃ�1.0f�A�܂�e
    float shadowRes = IsNear(input.sunPos, sunDepth);
    
    
	float3 Diff = g_Diffuse.rgb;
	float Alpha = g_Diffuse.a;
    float3 Ambi = g_Ambient.rgb;
     // �g�U�F�e�N�X�`���L��
    {
        float x = any(g_Flags.x);
        float4 TexDiff = g_texture.Sample(g_sampler, input.Tex);
        Diff =  IF_SUB(Diff, TexDiff.rgb, x);
        Ambi =  IF_SUB(Ambi, TexDiff.rgb, x);
        Alpha = IF_SUB(Alpha, TexDiff.a, x);
       
    }
    
    // �����F�e�N�X�`���L��
    {
        int z = any(g_Flags.z);
        float4 TexTran = g_texTransparent.Sample(g_sampler, input.Tex);
        Alpha = IF_SUB(Alpha, TexTran.r * TexTran.g * TexTran.b * TexTran.a, z);
    }
    clip(Alpha - 1.0f); // ���S�����Ȃ�`�悵�Ȃ�
	// ���߃e�N�X�`���L��
    float3 Spec = g_Specular.rgb;
    {
        float4 TexSpec = g_texSpecular.Sample(g_sampler, input.Tex);
        int w = any(g_Flags.w);
        Spec = IF_SUB(Spec, TexSpec.rgb, w);
    }

  {
        int nLight = any(g_vLightDir);
        float3 L = normalize(-g_vLightDir.xyz);                  // �����ւ̃x�N�g��
        float3 N = normalize(input.Normal);                      // �@���x�N�g��
        float3 V = normalize(g_vCameraPos.xyz - input.PosForPS); // ���_�ւ̃x�N�g��
        float3 H = normalize(L + V);                             // �n�[�t�x�N�g��
        Diff = nLight ? g_vLightAmbient.rgb * g_Ambient.rgb +
			g_vLightDiffuse.rgb * Diff * saturate(dot(L, N)) : Diff; // �g�U�F + ���F
        Spec = nLight ? Spec * g_vLightSpecular.rgb *
			pow(saturate(dot(N, H)), g_Specular.a) : Spec; // ���ʔ��ːF
        Diff += Spec;
    }

	float3 Emis = g_Emissive.rgb;
	{   // �e�N�X�`���L��
        int y = any(g_Flags.y);
		float4 TexEmis = g_texEmissive.Sample(g_sampler, input.Tex);
        Emis = IF_SUB(Emis, TexEmis.rgb * TexEmis.a, y);
    }
	Diff += Emis;
    
    //--- �e���ʔ��f
    // �e�Ȃ�S�č�
    Diff.rgb *= 1.0f - shadowRes;
    
    // �e�Ȃ�Ε`��
    //Alpha = shadowRes;

	return float4(Diff, Alpha);
}
