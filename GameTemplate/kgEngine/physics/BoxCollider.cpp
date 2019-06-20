#include "KGstdafx.h"
#include "Physics/BoxCollider.h"


/*!
* @brief	�R���X�g���N�^�B
*/
BoxCollider::BoxCollider():shape(NULL)
{
}
/*!
 * @brief	�f�X�g���N�^�B
 */
BoxCollider::~BoxCollider()
{
	delete shape;
}
/*!
 * @brief	�{�b�N�X�R���C�_�[���쐬�B
 */
void BoxCollider::Create(const CVector3& size)
{
	shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
}