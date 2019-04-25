/*!
 * @brief	カプセルコライダー。
 */

#include "KGstdafx.h"
#include "Physics/CapsuleCollider.h"



/*!
	* @brief	デストラクタ。
	*/
CapsuleCollider::~CapsuleCollider()
{
	delete shape;
}
