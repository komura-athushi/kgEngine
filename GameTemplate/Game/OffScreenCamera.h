/*****************************************************************//**
 * \file   OffScreenCamera.h
 * \brief  OffScreenCameraクラス
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/Camera.h"
/**
 * \brief 小窓や図鑑用のカメラ.
 */
class OffScreenCamera
{
public:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	OffScreenCamera();
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~OffScreenCamera();
	/**
	 * \brief 更新処理.
	 * 
	 */
	void Update();
	/**
	 * \brief プロジェクション行列を取得.
	 * 
	 * \return プロジェクション行列
	 */
	const CMatrix& GetProjectionMatrix() const
	{
		return m_camera.GetProjectionMatrix();
	}
	/**
	 * \brief ビュー行列を取得.
	 * 
	 * \return ビュー行列
	 */
	const CMatrix& GetViewMatrix() const
	{
		return m_camera.GetViewMatrix();
	}
	/**
	 * \brief 注視点を設定.
	 * 
	 * \param target 注視点
	 */
	void SetTarget(const CVector3& target)
	{
		m_target = target;
	}
	/**
	 * \brief 注視点を取得.
	 * 
	 * \return 注視点
	 */
	const CVector3& GetTarget() const
	{
		return m_target;
	}
	/**
	 * \brief 座標を設定.
	 * 
	 * \param position 座標
	 */
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	/**
	 * \brief 座標を取得.
	 * 
	 * \return 座標
	 */
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/**
	 * \brief 画角を設定.
	 * 
	 * \param angle 画角
	 */
	void SetViewAngle(const float angle)
	{
		m_camera.SetViewAngle(angle);
	}
	/**
	 * \brief カメラを取得.
	 * 
	 * \return カメラ
	 */
	Camera* GetCamera()
	{
		return &m_camera;
	}
private:
	Camera m_camera;									//カメラ
	CVector3 m_position = {0.0f,200.0f,500.0f};			//座標
	CVector3 m_target = { 0.0f,0.0f,0.0f };				//注視点
};

