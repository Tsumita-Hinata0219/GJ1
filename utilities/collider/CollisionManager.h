#pragma once
#include <list>

#include "Function.h"
#include "Collider.h"


class CollisionManager {

public:

	/// <summary>
	/// 総当たりの判定
	/// </summary>
	void CheckRoundRobin();

	/// <summary>
	/// コライダーを受け取り、リストに登録する
	/// </summary>
	void ColliderPushBack(Collider* collider);

	/// <summary>
	/// コライダーリストをクリアにする
	/// </summary>
	void ColliderClear();

	/// <summary>
	/// 衝突判定計算
	/// </summary>
	bool CheckonCollision(AABB aabb1, AABB aabb2);

private:

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);


private:

	// コライダーリスト
	std::list<Collider*> colliders_;

};