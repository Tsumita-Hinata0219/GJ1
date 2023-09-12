﻿#include "Player.h"


Player::Player() {}
Player::~Player() {}


void Player::Initialize(Model* model, Vector3 position) {

	// NULLポインタチェック
	assert(model);

	this->model_ = model;

	playerTextureHandle_ = TextureManager::Load("/picture/uvChecker.png");


	// プレイヤー
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();

	isHit = 0;

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);

	uint32_t newCollisionAttribute = 0;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_Ground;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_Damage;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_State;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_Goal;
	SetCollisionMask(newCollisionAttribute);
}



void Player::Update() {

	Move();

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// アフィン変換行列
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.UpdateMatrix();


	CalcAABB();


#ifdef _DEBUG


	ImGui::Begin("Player");

	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::Text("isHit : %d", isHit);

	ImGui::DragFloat3("Pla_Min", &aabb_.min.x, 0.1f, -1.0f, 5.0f);
	ImGui::DragFloat3("Pla_Max", &aabb_.max.x, 0.1f, -1.0f, 5.0f);

	ImGui::End();


#endif



}


void Player::Draw(ViewProjection viewProjection) {

	
	model_->Draw(worldTransform_, viewProjection, playerTextureHandle_);

}


void Player::Move() {

	Vector3 move = {0.0f, 0.0f, 0.0f}; // 移動ベクトル

	const float kCharacterSpeed = 0.3f; // 移動速度

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} 
	else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	if (input_->PushKey(DIK_R)) {
		isHit = false;
	}

	// 移動行列に移動ベクトルを加算
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}



void Player::CalcAABB() {

	aabb_.min = { 
		GetWorldPosition().x - (radius_ / 2),
	    GetWorldPosition().y - (radius_ / 2),
	    GetWorldPosition().z - (radius_ / 2),
	};
	aabb_.max = {
	    GetWorldPosition().x + (radius_ / 2),
	    GetWorldPosition().y + (radius_ / 2),
	    GetWorldPosition().z + (radius_ / 2),
	};
}



void Player::onCollision() {

	isHit = 1;
}



Vector3 Player::GetWorldPosition() {

	// ワールド座標を取得
	Vector3 worldPos{};

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



AABB Player::GetAABB() { return aabb_; }