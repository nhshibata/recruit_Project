//=========================================================
// mesh�p
//---------------------------------------------------------
// �s�N�Z���V�F�[�_
//=========================================================

// �O���[�o��
cbuffer global : register(b11) {
	float4	g_vEye;			// ���_���W
	// ����
	float4	g_vLightDir;	// ��������
	float4	g_vLa;			// ����
	float4	g_vLd;			// �g�U���ˌ�
	float4	g_vLs;			// ���ʔ��ˌ�
	// �}�e���A��
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
};

Texture2D    g_texture    : register(t0);	// �e�N�X�`��
SamplerState g_sampler    : register(s0);	// �T���v��

//--- �萔��`
#define IF_SUB(x,y,z)      (x + ((x * y - x) * z))


float4 main(VS_OUTPUT input) : SV_Target0
{
	float3 Diff = input.Diffuse.rgb * g_vKd.rgb;
	float3 Ambi = input.Diffuse.rgb * g_vKa.rgb;
	float Alpha = input.Diffuse.a * g_vKd.a;
    
    //--- �A���r�G���g�̃��l0���� == ø�������
    //float fTex = step(0.0f, g_vKa.a);
    float fTex = any(g_vKa.a);
    //if (0.0f < g_vKa.a)
    //if (step(0.0f, g_vKa.a))
    {
		// �e�N�X�`���L
        float4 vTd = g_texture.Sample(g_sampler, input.TexCoord);
        //Diff *= vTd.rgb;
        //Ambi *= vTd.rgb;
        //Alpha *= vTd.a;
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
		float3 L = normalize(-g_vLightDir.xyz);				// �����ւ̃x�N�g��
		float3 N = normalize(input.Normal);					// �@���x�N�g��
		float3 V = normalize(g_vEye.xyz - input.Pos4PS);	// ���_�ւ̃x�N�g��
		float3 H = normalize(L + V);						// �n�[�t�x�N�g��
        workDiff = g_vLa.rgb * Ambi + g_vLd.rgb *
			Diff * saturate(dot(L, N));						// �g�U�F + ���F
		float3 Spec = g_vLs.rgb * g_vKs.rgb *
			pow(saturate(dot(N, H)), g_vKs.a);				// ���ʔ��ːF
        workDiff += Spec;
    }
    
    Diff = any(g_vLightDir) ? workDiff : Diff;
    
	Diff += g_vKe.rgb;
    
	return float4(Diff, Alpha);
}

