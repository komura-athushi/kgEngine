#include "KGstdafx.h"
#include "Sprite.h"

CSprite::CSprite()
{
	m_screenSize.x = FRAME_BUFFER_W;
	m_screenSize.y = FRAME_BUFFER_H;
}

CSprite::~CSprite()
{
	if (m_srv != nullptr) {
		m_srv->Release();
	}
	if (m_cb != nullptr) {
		m_cb->Release();
	}
	if (m_tex != nullptr) {
		m_tex->Release();
	}
	if (m_dg != nullptr) {
		m_dg->Release();
	}
}

void CSprite::Init(ID3D11ShaderResourceView* srv)
{
	m_isNormal = true;
	m_srv = srv;
	if (m_srv != nullptr) {
		m_srv->AddRef();	//�Q�ƃJ�E���^�𑝂₷�B
	}
	m_spriteBatch = Engine().GetGraphicsEngine().GetSpriteBatch();
}

void CSprite::Init(const wchar_t* fileName, bool isCircleGauge)
{
	m_isCircleGauge = isCircleGauge;
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
	//�摜�T�C�Y�̎擾
	D3D11_RESOURCE_DIMENSION resType = D3D11_RESOURCE_DIMENSION_UNKNOWN;
	m_tex->GetType(&resType);
	switch (resType) {
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		ID3D11Texture2D* tex2d = static_cast<ID3D11Texture2D*>(m_tex);
		D3D11_TEXTURE2D_DESC desc;
		tex2d->GetDesc(&desc);

		//�摜�T�C�Y�̎擾
		m_width = desc.Width;
		m_height = desc.Height;
		m_sourceRectangle.top = 0;
		m_sourceRectangle.left = 0;
		m_sourceRectangle.bottom = desc.Height;
		m_sourceRectangle.right = desc.Width;
	}
	break;
	default:
		break;
	}
	//�쐬����o�b�t�@�̃T�C�Y��sizeof���Z�q�ŋ��߂�B
	int bufferSize = sizeof(ConstantBuffer);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC bufferDesc;
	{
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//�O�ŃN���A�B
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
		bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																	//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																	//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
		bufferDesc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
		Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
	}											//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
	int bufferSize2 = sizeof(float);
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC bufferDesc2;
	{
		ZeroMemory(&bufferDesc2, sizeof(bufferDesc2));				//�O�ŃN���A�B
		bufferDesc2.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
		bufferDesc2.ByteWidth = (((bufferSize2 - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																		//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
		bufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																		//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
		bufferDesc2.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
	}
	m_ps.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);
	//�쐬�B
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bufferDesc2, NULL, &m_dg);
}

void CSprite::DrawScreenPos(
	const CVector2& pos,
	const CVector2& scale,
	const CVector2& pivot,
	float rotation,
	const CVector4& color,
	DirectX::SpriteEffects effects,
	float layerDepth,
	float degree
)
{
	if (m_srv == nullptr) { return; }
	ConstantBuffer cb;
	cb.mulColor = color;
	layerDepth *= 0.999f; layerDepth += 0.001f;
	layerDepth -= Engine().GetGraphicsEngine().AddAndGetLayerDepthCnt();
	auto device = Engine().GetGraphicsEngine().GetD3DDeviceContext();

	Engine().GetGraphicsEngine().GetSpriteBatch()->Begin(DirectX::SpriteSortMode_BackToFront, nullptr, nullptr, nullptr, nullptr, [=]
		{
			device->UpdateSubresource(m_cb, 0, nullptr, &cb, 0, 0);
			device->PSSetConstantBuffers(0, 0, &m_cb);
			float Angle;
			if (m_isCircleGauge) {
				Angle = degree * CMath::PI / 180.0f;
			}
			else {
				Angle = CMath::PI * 2;
			}
			device->UpdateSubresource(m_dg, 0, nullptr, &Angle, 0, 0);
			device->PSSetConstantBuffers(1, 1, &m_dg);
			device->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), nullptr, 0);
		}

	);
	//Engine().GetGraphicsEngine().GetSpriteBatch()->Begin();
	m_spriteBatch->Draw(m_srv, pos.vec, &m_sourceRectangle, color, rotation, DirectX::XMFLOAT2(pivot.x * m_width, pivot.y * m_height), DirectX::XMFLOAT2(scale.x, scale.y), effects, layerDepth);
	//m_spriteBatch->Draw(m_srv, pos.vec);
	//m_spriteBatch->Draw(m_srv, DirectX::XMFLOAT2(0.0f,0.0f));
	Engine().GetGraphicsEngine().GetSpriteBatch()->End();
}

void CSprite::Draw()
{
	Engine().GetGraphicsEngine().GetSpriteBatch()->Begin();
	m_spriteBatch->Draw(m_srv, DirectX::XMFLOAT2(0.0f, 0.0f));
	m_spriteBatch->End();
}

