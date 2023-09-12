#include "Map.h"
#include "GameScene.h"


/// <summary>
/// 初期化処理
/// </summary>
void Map::Initialize(Model* model, Vector3 position, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;

	worldTransform_.translation_ = position;
	worldTransform_.Initialize();

	// 衝突属性
	SetCollisionAttribute(kCollisionAttributeMapBox);
	SetCollisionMask(kCollisionAttributePlayer);
}



/// <summary>
/// 更新処理
/// </summary>
void Map::Update() {
	
	// AABBに必要な値を求める
	CalcAABB();

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}



/// <summary>
/// AABBの計算に必要な値を求める
/// </summary>
void Map::CalcAABB() {

	aabb_.min = {
	    GetWorldPosition().x - (map_BoxSize_ / 2),
	    GetWorldPosition().y - (map_BoxSize_ / 2),
	    GetWorldPosition().z - (map_BoxSize_ / 2),
	};
	aabb_.max = {
	    GetWorldPosition().x + (map_BoxSize_ / 2),
	    GetWorldPosition().y + (map_BoxSize_ / 2),
	    GetWorldPosition().z + (map_BoxSize_ / 2),
	};
}



/// <summary>
/// 描画処理
/// </summary>
void Map::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}



/// <summary>
/// マップの破壊
/// </summary>
void Map::IsBreak() {

	IsBreak_ = true;
}



/// <summary>
/// 衝突時を検出したら呼びだされるコールバッグ関数
/// </summary>
void Map::onCollision() {

}



/// <summary>
/// ワールド座標を取得
/// </summary>
Vector3 Map::GetWorldPosition() {

	// ワールド座標を取得
	Vector3 worldPos{};

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



/// <summary>
/// AABBの取得
/// </summary>
AABB Map::GetAABB() { return aabb_; }