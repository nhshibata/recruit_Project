//=========================================================
// [meshBuffer.cpp]
// 作成: 2022/08/05
//---------------------------------------------------------
//=========================================================

//--- インクルード部
#include <GraphicsSystem/Shader/meshBuffer.h>
#include <GraphicsSystem/DirectX/DXDevice.h>

using namespace MySpace::Graphics;


CMeshBuffer::CMeshBuffer()
	:m_pIdxBuffer(nullptr), m_pVtxBuffer(nullptr)
{

}
CMeshBuffer::CMeshBuffer(Description desc)
	:m_stDesc(desc),m_pIdxBuffer(nullptr),m_pVtxBuffer(nullptr)
{
	CreateVertex();
	if (m_stDesc.pIdx)CreateIndex();
}
CMeshBuffer::~CMeshBuffer()
{
	SAFE_RELEASE(m_pIdxBuffer);
	SAFE_RELEASE(m_pVtxBuffer);
}

void CMeshBuffer::Bind(UINT slot)
{
	ID3D11DeviceContext* pDC = CDXDevice::Get()->GetDeviceContext();

	D3D11_PRIMITIVE_TOPOLOGY topology;
	switch (m_stDesc.topology)
	{
	case CMeshBuffer::LINE_LIST: topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
	case CMeshBuffer::TRIANGLE_LIST: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
	case CMeshBuffer::TRIANGLE_STRIP: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
	}
	pDC->IASetPrimitiveTopology(topology);

	UINT offset = 0;
	pDC->IASetVertexBuffers(slot, 1, &m_pVtxBuffer, &m_stDesc.vtxSize, &offset);

	if (m_pIdxBuffer)
	{
		DXGI_FORMAT format;
		switch (m_stDesc.idxSize)
		{
		case INDEX8: format = DXGI_FORMAT_R8_UINT; break;
		case INDEX16: format = DXGI_FORMAT_R16_UINT; break;
		case INDEX32: format = DXGI_FORMAT_R32_UINT; break;
		}
		pDC->IASetIndexBuffer(m_pIdxBuffer, format, 0);
		pDC->DrawIndexed(m_stDesc.idxCount, 0, 0);
	}
	else
	{
		pDC->Draw(m_stDesc.vtxCount, 0);
	}
}
void CMeshBuffer::Write(void* data)
{
	if (!m_stDesc.isWrite) { return; }

	ID3D11DeviceContext* pDC = CDXDevice::Get()->GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE mapResource;

	if (FAILED(pDC->Map(m_pVtxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource)))
	{
		return;
	}

	rsize_t size = m_stDesc.vtxCount * m_stDesc.vtxSize;
	memcpy_s(mapResource.pData, size, data, size);
	pDC->Unmap(m_pVtxBuffer, 0);
}
HRESULT CMeshBuffer::Make(Description desc)
{
	HRESULT hr = S_OK;
	m_stDesc = desc;
	hr = CreateVertex();
	if(m_stDesc.pIdx)hr = CreateIndex();
	return hr;
}
HRESULT CMeshBuffer::CreateIndex()
{
	// バッファの情報を設定
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = m_stDesc.idxSize * m_stDesc.idxCount;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	// バッファの初期データ
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = m_stDesc.pIdx;

	// インデックスバッファ生成
	return CDXDevice::Get()->GetDevice()->CreateBuffer(&bufDesc, &subResource, &m_pIdxBuffer);
	
}
HRESULT CMeshBuffer::CreateVertex()
{
	//--- 作成するバッファの情報
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = m_stDesc.vtxSize * m_stDesc.vtxCount;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	if (m_stDesc.isWrite)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	//--- バッファの初期値を設定
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = m_stDesc.pVtx;

	//--- 頂点バッファの作成
	return CDXDevice::Get()->GetDevice()->CreateBuffer(&bufDesc, &subResource, &m_pVtxBuffer);
}