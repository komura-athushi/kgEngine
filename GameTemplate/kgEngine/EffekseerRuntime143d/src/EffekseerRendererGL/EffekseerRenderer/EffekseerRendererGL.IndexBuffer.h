﻿
#pragma once

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerRendererGL.RendererImplemented.h"
#include "../../EffekseerRendererCommon/EffekseerRenderer.IndexBufferBase.h"
#include "EffekseerRendererGL.DeviceObject.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class IndexBuffer
	: public DeviceObject
	, public ::EffekseerRenderer::IndexBufferBase
{
private:
	GLuint					m_buffer;

	IndexBuffer( RendererImplemented* renderer, GLuint buffer, int maxCount, bool isDynamic );

public:
	virtual ~IndexBuffer();

	static IndexBuffer* Create( RendererImplemented* renderer, int maxCount, bool isDynamic );

	GLuint GetInterface() { return m_buffer; }

public:	// デバイス復旧用
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;

public:
	void Lock() override;
	void Unlock() override;

	bool IsValid();
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
