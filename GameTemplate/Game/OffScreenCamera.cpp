#include "stdafx.h"
#include "OffScreenCamera.h"

OffScreenCamera::OffScreenCamera()
{
	m_camera.SetNear(5);
	m_camera.SetFar(10000);
}

OffScreenCamera::~OffScreenCamera()
{

}

void OffScreenCamera::Update()
{
	m_camera.SetPosition(m_position);
	m_camera.SetTarget(m_position);
	m_camera.Update();
}