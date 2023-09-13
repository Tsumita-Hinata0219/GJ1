#pragma once
#include "Function.h"


/// <summary>
/// 衝突判定オブジェクトクラス
/// </summary>
class Collider {

public:

	/// <summary>
	/// 純粋仮想関数
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void onCollision(int num) = 0;


	/// <summary>
	/// 半径の取得
	/// </summary>
	float GetRadius() { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }


	/// <summary>
	/// 純粋仮想関数
	/// ワールド座標を取得
	/// </summary>
	virtual Vector3 GetWorldPosition() = 0;

	/// <summary>
	/// AABBの取得
	/// </summary>
	virtual AABB GetAABB() { return aabb_; }


	/// <summary>
	/// 衝突属性(自分)の取得・設定
	/// </summary>
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }


	/// <summary>
	/// 衝突属性(相手)の取得・設定
	/// </summary>
	uint32_t GetCollisionMask() { return collisionMask_; }
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }




private:

	// 衝突半径　デフォルト値
	float radius_ = 2.0f;

	// 今回はAABBで衝突判定をとる
	AABB aabb_;


	/// 0xffffffffは初期値で。すべてのビットが1の状態。すべてのものと当たる
	// 衝突属性(プレイヤー)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突属性(マップボックス) 
	uint32_t collisionMask_ = 0xffffffff;
};


