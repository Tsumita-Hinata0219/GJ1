#include "CollisionManager.h"



/// <summary>
/// 総当たりの判定
/// </summary>
void CollisionManager::CheckRoundRobin() {

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

	// 2つのオブジェクトのAABBを取得
	AABB aabb1 = colliderA->GetAABB();
	AABB aabb2 = colliderB->GetAABB();


	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
	    (colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {

		// 属性判定の時点で当たらないペアの場合は早期リターン
		return;
	} 


	// 衝突フィルター　プレイヤー : マップブロック
	uint32_t val1 = colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask();
	uint32_t val2 = colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask();


	// プレイヤーとマップボックス床壁
	if ((val1 + val2) == 3) {

		// 衝突判定
		if (CheckonCollision(colliderA->GetAABB(), colliderB->GetAABB())) {

			// 衝突時のコールバッグを呼び出す
			colliderA->onCollision(3);
			colliderB->onCollision(3);
		}
	} 
	// プレイヤーとマップボックスダメージ
	else if ((val1 + val2) == 5) {

		// 衝突判定
		if (CheckonCollision(colliderA->GetAABB(), colliderB->GetAABB())) {

			// 衝突時のコールバッグを呼び出す
			colliderA->onCollision(5);
			colliderB->onCollision(5);
		}
	} 
	// プレイヤーとマップボックススタート
	else if ((val1 + val2) == 9) {
		// 衝突判定
		if (CheckonCollision(colliderA->GetAABB(), colliderB->GetAABB())) {

			// 衝突時のコールバッグを呼び出す
			colliderA->onCollision(9);
			colliderB->onCollision(9);
		}
	} 
	// プレイヤーとマップボックスゴール
	else if ((val1 + val2) == 17) {

		// 衝突判定
		if (CheckonCollision(colliderA->GetAABB(), colliderB->GetAABB())) {

			// 衝突時のコールバッグを呼び出す
			colliderA->onCollision(17);
			colliderB->onCollision(17);
		}
	}
}



/// <summary>
/// 衝突判定計算
/// </summary>
bool CollisionManager::CheckonCollision(AABB aabb1, AABB aabb2) {

	// AABB1とAABB2の衝突判定
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&
	    (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&
	    (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		// 当たっている
		return true;
	}
	// 当たっていない
	return false;
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


