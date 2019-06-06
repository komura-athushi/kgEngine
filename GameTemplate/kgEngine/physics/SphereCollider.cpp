#include "KGstdafx.h"
#include "SphereCollider.h"

/*!
	* @brief	デストラクタ。
	*/
SphereCollider::~SphereCollider()
{
	delete m_shape;
}