// ���������p�s�N�Z���V�F�[�_ [ExpPixel.hlsl]

// �O���[�o��
cbuffer global : register(b1) {
	float4 g_vEye; // ���_���W
	// ����
	float4 g_vLightDir; // ��������
	float4 g_vLa; // ����
	float4 g_vLd; // �g�U���ˌ�
	float4 g_vLs; // ���ʔ��ˌ�
	// �}�e���A��
	float4 g_vKa; // �A���r�G���g�F(+�e�N�X�`���L��)
	float4 g_vKd; // �f�B�t���[�Y�F
	float4 g_vKs; // �X�y�L�����F(+�X�y�L�������x)
	float4 g_vKe; // �G�~�b�V�u�F
};

// �p�����[�^
struct VS_OUTPUT {
	float4 Position : SV_Position;
	float3 Pos4PS : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float4 Diffuse : COLOR;
};

float4 main(VS_OUTPUT input) : SV_Target0
{
	float3 Diff = g_vKd.rgb * input.Diffuse.rgb;
	float Alpha = g_vKd.a * input.Diffuse.a;
	if (Alpha <= 0.0f) discard;
	if (g_vLightDir.x != 0.0f || g_vLightDir.y != 0.0f || g_vLightDir.z != 0.0f) {
		// �����L��
		float3 L = normalize(-g_vLightDir.xyz); // �����ւ̃x�N�g��
		float3 N = normalize(input.Normal); // �@���x�N�g��
		float LN = dot(L, N);
		if (LN < 0.0f) {
			LN = -LN;
			N = -N;
		}
		float3 V = normalize(g_vEye.xyz - input.Pos4PS); // ���_�ւ̃x�N�g��
		float3 H = normalize(L + V); // �n�[�t�x�N�g��
		Diff = g_vLa.rgb * g_vKa.rgb + g_vLd.rgb *
		Diff * saturate(LN); // �g�U�F + ���F
		float3 Spec = g_vLs.rgb * g_vKs.rgb *
		pow(saturate(dot(N, H)), g_vKs.a); // ���ʔ��ːF
		Diff += Spec;
	}
	Diff += g_vKe.rgb; // �����F
	return float4(Diff, Alpha);
}
