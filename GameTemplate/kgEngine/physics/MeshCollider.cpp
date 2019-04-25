#include "KGstdafx.h"
#include "Physics/MeshCollider.h"



MeshCollider::MeshCollider() 
{
}


MeshCollider::~MeshCollider()
{

}

/*!
 * @brief	CSkinModel���烁�b�V���R���C�_�[�𐶐��B
 *@param[in]	model		�X�L�����f���B
 */
void MeshCollider::CreateFromSkinModel( const SkinModel& model, const CMatrix* offsetMatrix )
{
	CMatrix mBias;
	mBias.MakeRotationX(CMath::PI * -0.5f);
	m_stridingMeshInterface = std::make_unique<btTriangleIndexVertexArray>();

	model.FindMesh([&](const auto& mesh){

			ID3D11DeviceContext* deviceContext = g_graphicsEngine->GetD3DDeviceContext();
			//���_�o�b�t�@���쐬�B
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				HRESULT hr = deviceContext->Map(mesh->vertexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
				if (FAILED(hr)) {
					return;
				}
				D3D11_BUFFER_DESC bufferDesc;
				mesh->vertexBuffer->GetDesc(&bufferDesc);
				int vertexCount = bufferDesc.ByteWidth / mesh->vertexStride;
				char* pData = reinterpret_cast<char*>(subresource.pData);
				VertexBufferPtr vertexBuffer = std::make_unique<VertexBuffer>();
				CVector3 pos;
				for (int i = 0; i < vertexCount; i++) {
					pos = *reinterpret_cast<CVector3*>(pData);
					//�o�C�A�X��������B
					mBias.Mul(pos);
					vertexBuffer->push_back(pos);
					//���̒��_�ցB
					pData += mesh->vertexStride;
				}
				//���_�o�b�t�@���A�����b�N
				deviceContext->Unmap(mesh->vertexBuffer.Get(), 0);
				m_vertexBufferArray.push_back(std::move(vertexBuffer));
			}
			//�C���f�b�N�X�o�b�t�@���쐬�B
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				//�C���f�b�N�X�o�b�t�@�����b�N�B
				HRESULT hr = deviceContext->Map(mesh->indexBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subresource);
				if (FAILED(hr)) {
					return;
				}
				D3D11_BUFFER_DESC bufferDesc;
				mesh->indexBuffer->GetDesc(&bufferDesc);
				//@todo cmo�t�@�C���̓C���f�b�N�X�o�b�t�@�̃T�C�Y��2byte�Œ�B
				IndexBufferPtr indexBuffer = std::make_unique<IndexBuffer>();
				int stride = 2;
				int indexCount = bufferDesc.ByteWidth / stride;
				unsigned short* pIndex = reinterpret_cast<unsigned short*>(subresource.pData);
				for (int i = 0; i < indexCount; i++) {
					indexBuffer->push_back(pIndex[i]);
				}
				//�C���f�b�N�X�o�b�t�@���A�����b�N�B
				deviceContext->Unmap(mesh->indexBuffer.Get(), 0);
				m_indexBufferArray.push_back(std::move(indexBuffer));
			}

			//�C���f�b�N�X���b�V�����쐬�B
			btIndexedMesh indexedMesh;
			IndexBuffer* ib = m_indexBufferArray.back().get();
			VertexBuffer* vb = m_vertexBufferArray.back().get();
			indexedMesh.m_numTriangles = (int)ib->size() / 3;
			indexedMesh.m_triangleIndexBase = (unsigned char*)(&ib->front());
			indexedMesh.m_triangleIndexStride = 12;
			indexedMesh.m_numVertices = (int)vb->size();
			indexedMesh.m_vertexBase = (unsigned char*)(&vb->front());
			indexedMesh.m_vertexStride = sizeof(CVector3);
			m_stridingMeshInterface->addIndexedMesh(indexedMesh);
		}
	);
	m_meshShape = std::make_unique<btBvhTriangleMeshShape>(m_stridingMeshInterface.get(), true);
}