//=========================================================
// [PS_GBuffer.hlsl]
//=========================================================

#include <Common.hlsli>

struct PS_OUT
{
    // �o�͐�ɍ��킹�āA�Z�}���e�B�N�X�ɐ�����U��
    float4 color    : SV_TARGET0;
    float4 normal   : SV_TARGET1;
    float4 wolrd    : SV_TARGET2;
    float4 depth    : SV_TARGET3;
};

// �ݒ肳�ꂽ��ʕ\���p�̃����_�[�^�[�Q�b�g�ɐF��n��
PS_OUT main(PS_INPUT pin, out float4 mainColor : SV_Target4)
{
    PS_OUT pout;
    
    pout.color = g_material[pin.id].fDiffuse;
    // �F��񏑂�����
    if (any(g_material[pin.id].fAmbient.a))
        pout.color *= g_texture.Sample(g_sampler, pin.Tex);
    mainColor = pout.color;
    
    // �@��
    // �@���̏���-1�`1�����Aø�����0�`1�ň������߁A-1�`1 �� 0�`1�ւ̕ϊ����K�v
    pout.normal = float4(pin.Normal * 0.5f + 0.5f, 1.0f);
    
    // ܰ��ލ��W�̏�������
    // ���[�x�l���������ޕ����悢
    // ܰ��ލ��W����x,y,z��3�������s�������A�������ݐ�͐F��rgb�e1byte�����Ȃ��B
    // �[�x�l���烏�[���h���W�𕜌����邱�Ƃ��o����̂ŁA4byte�����āA���m�ɕۑ��ł���
    //pout.wolrd = pin.wPos; ��
    float depthZ = pin.PosForPS.z / pin.PosForPS.w;
    pout.wolrd = float4(depthZ, depthZ, depthZ, depthZ);
    
    pout.depth = float4(depthZ, depthZ, depthZ, depthZ);
    
    return pout;
}
