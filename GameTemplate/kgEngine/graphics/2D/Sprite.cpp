#include "KGstdafx.h"
#include "Sprite.h"

CSprite::CSprite()
{
	m_screenSize.x = FRAME_BUFFER_W;
	m_screenSize.y = FRAME_BUFFER_H;
}

CSprite::~CSprite()
{

}

void CSprite::Init(const wchar_t* fileName)
{
	HRESULT hr;
	hr = DirectX::CreateDDSTextureFromFile(Engine().GetGraphicsEngine().GetD3DDevice(), fileName, &m_tex, &m_srv);
	m_spriteBatch = Engine().GetGraphicsEngine().GetSpriteBatch();
	if (FAILED(hr)) {
#ifdef BUILD_LEVEL_MASTER
		char message[256];
		sprintf_s(message, "�摜�ǂݍ��݂Ɏ��s�B\n�t�@�C���p�X�����Ă܂����H\n%ls\n", fileName);
		MessageBox(NULL, message, "Error", MB_OK);
		std::abort();
#endif
	}
	//�쐬����o�b�t�@�̃T�C�Y��sizeof���Z�q�ŋ��߂�B
	int bufferSize = sizeof(ConstantBuffer);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//�O�ŃN���A�B
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
	bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
	bufferDesc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
																//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
	int bufferSize2 = sizeof(float);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC bufferDesc2;
	ZeroMemory(&bufferDesc2, sizeof(bufferDesc2));				//�O�ŃN���A�B
	bufferDesc2.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
	bufferDesc2.ByteWidth = (((bufferSize2 - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																	//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
	bufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																	//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
	bufferDesc2.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
																	//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
	m_ps.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);
	m_vs.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
	//�쐬�B
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc2, NULL, &m_dg);
}

void CSprite::DrawScreenPos(
	const CVector2& pos,
	const CVector2& scale,
	const CVector2& pivot,
	float rotation,
	const CVector4& color,
	DirectX::SpriteEffects effects,
	float layerDepth
)
{
	m_degree += 0.5f;
	if (!m_srv) { return; }
	ConstantBuffer cb;
	cb.mulColor = m_mulColor;
	layerDepth *= 0.999f; layerDepth += 0.001f;
	layerDepth -= Engine().GetGraphicsEngine().AddAndGetLayerDepthCnt();
	auto device = Engine().GetGraphicsEngine().GetD3DDeviceContext();
	Engine().GetGraphicsEngine().GetSpriteBatch()->Begin(DirectX::SpriteSortMode_BackToFront, nullptr, nullptr, nullptr, nullptr, [=]
		{
			device->UpdateSubresource(m_cb, 0, NULL, &cb, 0, 0);
			//device->VSSetConstantBuffers(0,1, &m_cb);
			device->PSSetConstantBuffers(0, 1, &m_cb);
			float Angle = m_degree * CMath::PI / 180.0f;
			device->UpdateSubresource(m_dg, 0, NULL, &Angle, 0, 0);
			device->PSSetConstantBuffers(1, 1, &m_dg);
			device->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(),NULL,0);
			//device->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), NULL, 0);
		}
	);
	m_spriteBatch->Draw(m_srv, pos.vec, nullptr, color, rotation, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(scale.x, scale.y), effects, layerDepth);
	Engine().GetGraphicsEngine().GetSpriteBatch()->End();
}

