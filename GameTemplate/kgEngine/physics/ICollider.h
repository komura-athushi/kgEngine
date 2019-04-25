/*!
 * @brief	コライダー。
 */

#pragma once

class btCollisionShape;

/*!
 * @brief	コライダーのインターフェースクラス。
 */
class ICollider {
public:
	virtual btCollisionShape* GetBody() const = 0;
};
