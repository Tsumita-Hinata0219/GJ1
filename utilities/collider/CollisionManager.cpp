#include "CollisionManager.h"



/// <summary>
/// 衝突判定と応答
/// </summary>
void CollisionManager::CheckAllCollision() {

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {

		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {

			// イテレータBからコライダーBを取得する
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}



/// <summary>
/// コライダー2つの衝突判定 今回はAABB
/// </summary>
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {


	AABB aabb1 = colliderA->GetAABB();
	AABB aabb2 = colliderB->GetAABB();

	// AABB1とAABB2の衝突判定
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
	    (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
	    (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		// コライダーAの衝突時コールバッグを呼び出す
		colliderA->onCollision();
		// コライダーBの衝突時コールバッグを呼び出す
		colliderB->onCollision();
	}
}



/// <summary>
/// コライダーを受け取り、リストに登録する
/// </summary>
void CollisionManager::ColliderPushBack(Collider* collider) {

	colliders_.push_back(collider);

}



/// <summary>
/// コライダーリストをクリアにする関数
/// </summary>
void CollisionManager::ColliderClear() { 

	colliders_.clear();
}


