#pragma once

//Sprite��Factory
class FontFactory {
public:
	DirectX::SpriteFont* Load(const wchar_t* path);
private:
	//unique_ptr  �X�}�[�g�|�C���^�Anew�������������w���|�C���^�����݂��Ȃ���Ύ����I��
	//delete�����
	std::unordered_map<int, std::unique_ptr<DirectX::SpriteFont>> m_fontmap;
};

class CFont
{
public:
	CFont();
	~CFont();
	//�t�H���g�����[�h����
	void LoadFont(const wchar_t* path) {
		m_spriteFont = m_fontfactory.Load(path);
	}
	//�����ݒ�̃V�X�e���t�H���g���g�p����
	void UseSystemFont()
	{
		m_spriteFont = Engine().GetGraphicsEngine().GetSpriteFont();
	}

	//�`�悷��
		//��layerDepth�̑�O�����_�ȉ��͎g��Ȃ��ق��������Ǝv��
		//const CVector2& pos ��0.0f�`1.0f�͈̔́B{0.0f,0.0f}�ŉ�ʍ���
	void Draw(
		wchar_t const* text,
		const CVector2& pos,
		const CVector4& color = CVector4::White(),
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f
	);
	//const CVector2& pos ���X�N���[�����W�Ŏw�肷��ŁB�E���̍��W=��ʉ𑜓x�@
	void DrawScreenPos(
		wchar_t const* text,
		const CVector2& pos,
		const CVector4& color = CVector4::White(),
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f
	);
private:
	DirectX::SpriteBatch* m_spriteBatch = nullptr;
	DirectX::SpriteFont* m_spriteFont = nullptr;
	CVector2 m_screenSize;

	static FontFactory m_fontfactory;

};

