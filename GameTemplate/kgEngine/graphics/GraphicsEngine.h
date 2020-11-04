#pragma once
#include "shadow/kgShadowMap.h"
#include "PostEffect.h"
#include "2D/Sprite.h"
class ShadowMap;
class NormalMap;
class DepthValueMap;
class EdgeDetection;
class CascadeShadowMap;

static const int m_splitMaximumNumber = 2;
/*!
 *@brief	�O���t�B�b�N�X�G���W���B
 */
 /// <summary>
 /// �����_�����O���[�h�B
 /// </summary>
enum EnRenderMode {
	enRenderMode_Invalid,			//�s���ȃ����_�����O���[�h�B
	enRenderMode_CreateShadowMap,	//�V���h�E�}�b�v�����B
	enRenderMode_CreateCascadeShadowMap,	//�J�X�P�[�h�V���h�E�}�b�v�̐���
	enRenderMode_Normal,			//�ʏ탌���_�����O�B
	enRenderMode_NormalMap,			//�@���}�b�v����
	enRenderMode_DepthValueMap,		//�[�x�l�}�b�v����
	enRenderMode_Num,				//�����_�����O���[�h�̐��B
};
class GraphicsEngine:Noncopyable
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	/*!
	 *@brief	�������B
	 *@param[in]	hWnd		�E�B���h�E�n���h���B
	 */
	void Init(HWND hWnd);
	/*!
	 *@brief	����B
	 */
	void Release();
	/*!
	 *@brief	D3D11�f�o�C�X���擾�B
	 */
	ID3D11Device* GetD3DDevice()
	{
		return m_pd3dDevice;
	}
	/*!
	 *@brief	D3D11�f�o�C�X�R���e�L�X�g���擾�B
	 */
	ID3D11DeviceContext* GetD3DDeviceContext()
	{
		return m_pd3dDeviceContext;
	}
	/*!
	 *@brief	�`��J�n�B
	 */
	void BegineRender();
	/*!
	 *@brief	�`��I���B
	 */
	void EndRender();
	//�V���h�E�}�b�v��`��
	void ShadowMapRender();
	//�G�b�W���o
	void EdgeDelectionRender();
	void EffektUpdate();
	//�|�X�g�����_�����O
	void PostRender();
	//�J�X�P�[�h�V���h�E�}�b�v���擾
	CascadeShadowMap* GetCascadeShadowMap()
	{
		return m_cascadeShadowMap.get();
	}
	//�@���}�b�v���擾
	NormalMap* GetNormalMap()
	{
		return m_normalMap.get();
	}
	//�[�x�l�}�b�v�������
	DepthValueMap* GetDepthValueMap()
	{
		return m_depthValueMao.get();
	}
	//�G�b�W�}�b�v���擾
	EdgeDetection* GetEdgeDelection()
	{
		return m_edgeDelection.get();
	}
	//�S��ʕ`��p�̃|�X�g�G�t�F�N�g���擾
	PostEffect* GetPostEffect()
	{
		return m_postEffect.get();
	}
	//Sprite�擾
	DirectX::SpriteBatch* GetSpriteBatch() const
	{
		return m_spriteBatch.get();
	}
	//��Z�ς݃A���t�@��
	DirectX::SpriteBatch* GetSpriteBatchPMA() const
	{
		return m_spriteBatchPMA.get();
	}
	//�f�t�H���g�t�H���g�擾
	DirectX::SpriteFont* GetSpriteFont() const
	{
		return m_spriteFont.get();
	}
	float AddAndGetLayerDepthCnt() {
		m_layerDepthCnt += 0.001f / 2048.0f;
		return m_layerDepthCnt;
	}
	void ResetLayerDepthCnt() {
		m_layerDepthCnt = 0.0f;
	}
	/// <summary>
	/// ���C�������_�����O�^�[�Q�b�g���擾�B
	/// </summary>
	/// <returns></returns>
	RenderTarget* GetMainRenderTarget()
	{
		return &m_mainRenderTarget;
	}
	//�����_�����O�^�[�Q�b�g�����C���ɕύX����
	void ChangeMainRenderTarget();
	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̐؂�ւ��B
	/// </summary>
	/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
	/// <param name="viewport">�r���[�|�[�g</param>
	void ChangeRenderTarget(RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	//�V���h�E�}�b�v�p�̃��C�g�J�����̒����_��ݒ�
	void SetLightCameraTarget(const CVector3& target, const int number = 0)
	{
		m_target[number] = target;
	}
	//�V���h�E�}�b�v�p�̃��C�g�J�����̍��W��ݒ�
	void SetLightCameraPosition(const CVector3& position, const int number = 0)
	{
		m_position[number] = position;
	}
	//�G�b�W���o�̃I���I�t
	void SetisEdge(bool flag)
	{
		m_isEdge = flag;
	}
	//�����_�[�^�[�Q�b�g���t���[���o�b�t�@�ɕύX
	void ChangeRenderTargetFrameBuffer()
	{
		ChangeRenderTarget(m_frameBufferRenderTargetView, m_frameBufferDepthStencilView, &m_frameBufferViewports);
	}
	//�r���[�|�[�g��ݒ�
	void SetViewPort()
	{
		m_pd3dDeviceContext->RSSetViewports(1, &m_frameBufferViewports);
	}
	//�r���[�|�[�g����ʕ����p�ɐݒ�
	//0�ň�Ԗ�
	void SetSplitViewPort(const int number)
	{
		//m_frameBufferViewports = m_splitViewPorts[number];
		m_pd3dDeviceContext->RSSetViewports(1, &m_splitViewPorts[number]);
	}
	//�r���[�|�[�g��ʏ�ɐݒ�
	void SetNormalViewPort()
	{
		//m_frameBufferViewports = m_normalViewPorts;
		//m_pd3dDeviceContext->RSSetViewports(1, &m_frameBufferViewports);
		m_pd3dDeviceContext->RSSetViewports(1, &m_normalViewPorts);
	}
	//��ʕ��������ݒ�
	void SetisSplit(bool flag)
	{
		m_isSplit = flag;
	}
	//��ʕ�������H
	bool GetisSplit() const
	{
		return m_isSplit;
	}
	//��ʕ��������擾
	int GetSplitNumber()
	{
		if (m_isSplit) {
			return m_splitMaximumNumber;
		}
		else {
			return 1;
		}
	}
	void SetBlend()
	{
		//��Z�����p�̃u�����f�B���O�X�e�[�g��ݒ肷��B
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_pd3dDeviceContext->OMSetBlendState(m_finalBlendState, blendFactor, 0xffffffff);
	}
private:
	D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D �f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g�B
	ID3D11Device*			m_pd3dDevice = NULL;		//D3D11�f�o�C�X�B
	IDXGISwapChain*			m_pSwapChain = NULL;		//�X���b�v�`�F�C���B
	ID3D11DeviceContext*	m_pd3dDeviceContext = NULL;	//D3D11�f�o�C�X�R���e�L�X�g�B
	ID3D11RenderTargetView* m_backBuffer = NULL;		//�o�b�N�o�b�t�@�B
	ID3D11RasterizerState*	m_rasterizerState = NULL;	//���X�^���C�U�X�e�[�g�B
	ID3D11Texture2D*		m_depthStencil = NULL;		//�f�v�X�X�e���V���B
	ID3D11DepthStencilView* m_depthStencilView = NULL;	//�f�v�X�X�e���V���r���[�B
	std::unique_ptr<CascadeShadowMap> m_cascadeShadowMap = nullptr;		//�J�X�P�[�h�V���h�E�}�b�v
	std::unique_ptr<NormalMap>				m_normalMap = nullptr;		//�@���}�b�v
	std::unique_ptr<DepthValueMap>			m_depthValueMao = nullptr;  //�[�x�l�}�b�v
	std::unique_ptr<EdgeDetection>			m_edgeDelection = nullptr;  //�G�b�W���o
	RenderTarget			m_mainRenderTarget;			//!<���C�������_�����O�^�[�Q�b�g�B
	CSprite m_copyMainRtToFrameBufferSprite;
	//Sprite
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatchPMA;
	float m_layerDepthCnt = 0.0f;

	std::unique_ptr<PostEffect> m_postEffect = nullptr;
	D3D11_VIEWPORT m_frameBufferViewports;			//�t���[���o�b�t�@�̃r���[�|�[�g�B
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[�B
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//�t���[���o�b�t�@�̃f�v�X�X�e���V���r���[�B

	CVector3 m_target[m_splitMaximumNumber] = { CVector3::Zero() , CVector3::Zero()};
	CVector3 m_position[m_splitMaximumNumber] = { { 0.0f,1000.0f,1000.0f }, { 0.0f,1000.0f,1000.0f } };

	bool m_isEdge = true;
	//�r���[�|�[�g�͕`�悷��͈͂��w�肷��A�V���h�E�}�b�v�̎��͕ς���񂶂Ⴜ
	D3D11_VIEWPORT m_normalViewPorts;				//�ʏ�r���[�|�[�g
	D3D11_VIEWPORT m_splitViewPorts[2];				//�����r���[�|�[�g
	ID3D11BlendState* m_finalBlendState = nullptr;		//�ŏI�����p�̃u�����f�B���O�X�e�[�g�B
	bool m_isSplit = false;
};

//extern GraphicsEngine* g_graphicsEngine;			//�O���t�B�b�N�X�G���W��