#pragma once
/// <summary>
/// �����_�����O�^�[�Q�b�g
/// </summary>
class RenderTarget
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	RenderTarget();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~RenderTarget();

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̉���B
	/// </summary>
	/// <remarks>
	/// ���̊֐��̓f�X�g���N�^����Ă΂�Ă��܂��B
	/// �����I�ȃ^�C�~���O�Ń����_�����O�^�[�Q�b�g������������ꍇ�Ɏg�p���Ă��������B
	/// </remarks>
	void Release();

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̍쐬�B
	/// </summary>
	/// <param name="w">�e�N�X�`���̕�</param>
	/// <param name="h">�e�N�X�`���̍���</param>
	/// <param name="texFormat">�e�N�X�`���̃t�H�[�}�b�g</param>
	void Create(unsigned int w, unsigned int h, DXGI_FORMAT texFormat);

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���̃N���A�B
	/// </summary>
	/// <param name="clearColor">�N���A�J���[</param>
	void ClearRenderTarget(float* clearColor);

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�r���[���擾�B
	/// </summary>
	/// <returns>�����_�����O�^�[�Q�b�g�r���[</returns>
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return m_renderTargetView;
	}

	/// <summary>
	/// �f�v�X�X�e���V���r���[���擾�B
	/// </summary>
	/// <returns>�f�v�X�X�e���V���r���[</returns>
	ID3D11DepthStencilView* GetDepthStensilView()
	{
		return m_depthStencilView;
	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g��SRV���擾�B
	/// </summary>
	/// <returns>�����_�����O�^�[�Q�b�g��SRV</returns>
	ID3D11ShaderResourceView* GetRenderTargetSRV()
	{
		return m_renderTargetSRV;
	}
	//�r���[�|�[�g��ݒ�
	void SetViewPort(D3D11_VIEWPORT& viewPort)
	{
		m_viewport = viewPort;
	}
	/// <summary>
	/// �r���[�|�[�g�̎擾�B
	/// </summary>
	/// <returns>�r���[�|�[�g</returns>
	D3D11_VIEWPORT* GetViewport()
	{
		return &m_viewport;
	}
	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̕����擾�B
	/// </summary>
	/// <returns></returns>
	float GetWidth() const
	{
		return m_width;
	}
	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̍������擾�B
	/// </summary>
	/// <returns></returns>
	float GetHeight() const
	{
		return m_height;
	}
	/// <summary>
	/// �����_�����O�^�[�Q�b�g�ƂȂ��Ă���e�N�X�`���̃t�H�[�}�b�g���擾�B
	/// </summary>
	/// <returns></returns>
	DXGI_FORMAT GetTextureFormat() const
	{
		return m_format;
	}
private:
	float m_width = 0;										//�����_�����O�^�[�Q�b�g�̕��B
	float m_height = 0;										//�����_�����O�^�[�Q�b�g�̍����B
	DXGI_FORMAT m_format = DXGI_FORMAT_UNKNOWN;				//�����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g�B
	ID3D11Texture2D* m_renderTargetTex = nullptr;		//!<�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`���B
	ID3D11RenderTargetView* m_renderTargetView = nullptr;		//!<�����_�[�^�[�Q�b�g�r���[�B
	ID3D11ShaderResourceView* m_renderTargetSRV = nullptr;		//!<�����_�����O�^�[�Q�b�g��SRV
	ID3D11Texture2D* m_depthStencilTex = nullptr;		//!<�f�v�X�X�e���V���ƂȂ�e�N�X�`���B
	ID3D11DepthStencilView* m_depthStencilView = nullptr;		//!<�f�v�X�X�e���V���r���[�B
	D3D11_VIEWPORT				m_viewport;							//!<�r���[�|�[�g�B
};

