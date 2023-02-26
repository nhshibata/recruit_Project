//==========================================================
// [PS_Mix2DColor]
//---------------------------------------------------------
// �F�摜3���킹
// PostProcess�摜��`�悷�邩�m�F����
// 2D�摜���畡���̃��C���[�̉��s���͔��f�ł��Ȃ�����
// GBuffer�̐F�摜�ƃ|�X�g�v���Z�X�O�̉摜���r���A�����Ȃ���Ε`�悳�ꂽ�Ɣ��肷��
//==========================================================

#include <common.hlsli>

struct PS_IN
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = g_ColorTexture.Sample(g_sampler, pin.uv);    // GBuffer�J���[ø���
    float4 mainColor = g_texture.Sample(g_sampler, pin.uv);     // Ҳ�ø���
    float4 postColor = g_postTexture.Sample(g_sampler, pin.uv); // �|�X�g�v���Z�X��ø���
    
    // �`�悳�ꂽ�F��GBuffer�ɏ������܂ꂽ�F
    float4 check = mainColor - color;
    
    // ����0�ł͂Ȃ����m�F
    float disp = any(check);
    // �������� = 1.0f�Ȃ�s�N�Z���L��
    clip(-disp);

    return postColor;
}