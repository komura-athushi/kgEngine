/*!
 * @brief	�x�N�g���N���X�B
 */

#include "KGstdafx.h"
#include "Vector.h"
#include "Matrix.h"




/*!
*@brief	�s�񂩂�N�H�[�^�j�I�����쐬�B
*/
void CQuaternion::SetRotation(const CMatrix& m)
{
	DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
}

