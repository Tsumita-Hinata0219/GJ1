#include "Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() {}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player() {}



/// <summary>
/// 初期化処理
/// </summary>
void Player::Initialize(Model* model, Vector3 position) {

	assert(model);
	this->model_ = model;

	worldTransform_.translation_ = position;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();



	// マップビット
	mapBit_.GroundBox = 3;
	mapBit_.DamageBox = 5;
	mapBit_.StartBox = 9;
	mapBit_.GoalBox = 17;

	// 衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);

	// プレイヤーが誰と当たるのか
	uint32_t newCollisionAttribute = 0;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_Ground;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_Damage;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_State;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_Goal;
	SetCollisionMask(newCollisionAttribute);
}



/// <summary>
/// 更新処理
/// </summary>
void Player::Update() {

	/*画像*/
	worldTransform_.TransferMatrix();

	/*移動*/
	Move();

	/*弾*/
	Box();
	tim_--;
	// 赤
	Vector3 moveBoxRed = {0, 0, 0};
	moveBoxRed.y += boxSpeedRed_;
	for (BoxType* box : REDs_) {
		box->Update(moveBoxRed);
	}
	// 青
	Vector3 moveBoxBulue = {0, 0, 0};

	moveBoxBulue.x += boxSpeedBlue_;

	for (BoxType* box : BLUEs_) {
		box->Update(moveBoxBulue);
	}

	HitBox();

	#ifdef _DEBUG

	ImGui::Begin("Player");

	ImGui::DragFloat3("Translation", &worldTransform_.translation_.x, 0.01f);

	ImGui::End();

#endif
}



/// <summary>
/// 描画処理
/// </summary>
void Player::Draw(ViewProjection viewProjection) {

	/*画像*/
	model_->Draw(this->worldTransform_, viewProjection);
	/*操作キー*/
	input_ = Input::GetInstance();
	/*弾*/
	for (BoxType* box : REDs_) {
		box->Draw(viewProjection);
	}
	for (BoxType* box : BLUEs_) {
		box->Draw(viewProjection);
	}
}



/// <summary>
/// 移動処理
/// </summary>
void Player::Move() {

	worldTransform_.TransferMatrix();

#pragma region キーボード操作

	Vector3 Keyboardmove = {0, 0, 0};

	if (input_->PushKey(DIK_LEFT)) {
		Keyboardmove.x -= kPlayerSpeed_;
	} else if (input_->PushKey(DIK_RIGHT)) {
		Keyboardmove.x += kPlayerSpeed_;
	}

#pragma endregion

#pragma region ジャンプ

	/*acce_.y = -0.08f;

	if ((input_->TriggerKey(DIK_SPACE) || joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
	    worldTransform_.translation_.y == worldTransform_.scale_.y) {
		velocity_.y = 1.0f;
	}

	velocity_.y += acce_.y;
	worldTransform_.translation_.y += velocity_.y;

	if (worldTransform_.translation_.y <= worldTransform_.scale_.y) {
		worldTransform_.translation_.y = worldTransform_.scale_.y;
	}*/

#pragma endregion

#pragma region 回転

	if (input_->TriggerKey(DIK_LEFT)) {
		playerRotateLeftFlag = true;
	}

	if (input_->TriggerKey(DIK_RIGHT)) {
		playerRotateRightFlag = true;
	}

	// 左
	if (playerRotateLeftFlag == 1) {
		worldTransform_.rotation_.y = 2.5f;

		playerRotateLeftFlag = false;
	}

	// 右
	if (playerRotateRightFlag == 1) {
		worldTransform_.rotation_.y = 13.5f;

		playerRotateRightFlag = false;
	}

#pragma endregion

#pragma region コントローラ

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// 右スティックのX軸の値を取得
		float rightThumbstickX = (float)joyState.Gamepad.sThumbRX / SHRT_MAX;

		// スティックの値に基づいて移動を計算
		Vector3 move = {rightThumbstickX, 0.0f, 0.0f};
		move = Multiply(kPlayerSpeed_, Normalize(move));

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

#pragma endregion


	worldTransform_.translation_ = Add(worldTransform_.translation_, Keyboardmove);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.UpdateMatrix();

	//// 移動制限
	//const float kMoveLimitX = 33;
	//const float kMoveLimitY = 18;
	//worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	//worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	//worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	//worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}



/// <summary>
/// 衝突判定処理
/// </summary>
void Player::onCollision(int num) {

	for (Map* ground : MapGrounds_) {

		ground->GetAABB().max.y;
	}

	// 壁や床に当たった時の処理
	if (num == mapBit_.GroundBox) {

	} 
	// ダメージ床に当たった時の処理
	else if (num == mapBit_.DamageBox) {

	} 
	// スタートボックスに当たった時の処理
	else if (num == mapBit_.StartBox) {

	} 
	// ゴールボックスに当たった時の処理
	else if (num == mapBit_.GoalBox) {

	}
}



/// <summary>
/// ワールド座標の取得
/// </summary>
Vector3 Player::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos{};
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}



/// <summary>
/// AABBの取得
/// </summary>
AABB Player::GetAABB() {

	// プレイヤーのAABBの最小座標と最大座標を計算
	Vector3 min = {
	    worldTransform_.translation_.x - playerWidth_ / 2,
	    worldTransform_.translation_.y - playerHeight_ / 2,
	    worldTransform_.translation_.z - playerDepth_ / 2};
	Vector3 max = {
	    worldTransform_.translation_.x + playerWidth_ / 2,
	    worldTransform_.translation_.y + playerHeight_ / 2,
	    worldTransform_.translation_.z + playerDepth_ / 2};

	// 計算した最小座標と最大座標からAABBを構築
	AABB aabb{};
	aabb.min = min;
	aabb.max = max;

	return aabb;
}



/// <summary>
/// ボックスを出す処理
/// </summary>
void Player::Box() {

	if (tim_ <= 0) {
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}
		// 赤
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			redMove_ = true;
			blueMove_ = false;
			boxSpeedRed_ = 0.0f;
			TIM_ = 180;
			// 右
			if (rLetGo_ == true) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
					lLetGo_ = false;
					if (bButtonReleased_) {
						BoxType* boxRed_ = new BoxType;
						bulletOffset_.x += 2.0f;
						// 弾の位置を計算してオフセットを適用
						Vector3 bulletPosition = Add(worldTransform_.translation_, bulletOffset_);

						boxRed_->Initialize(model_, bulletPosition, redBox_);

						REDs_.push_back(boxRed_);
						bButtonReleased_ = false; // 十字キーが押されたことを記録
						tim_ = 20;
					}
				} else {
					bButtonReleased_ = true; // 十字キーがリリースされたことを記録
				}
			}
			// 左
			if (lLetGo_ == true) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
					rLetGo_ = false;
					if (bButtonReleased_) {
						BoxType* boxRed_ = new BoxType;
						bulletOffset_.x -= 2.0f;
						// 弾の位置を計算してオフセットを適用
						Vector3 bulletPosition = Add(worldTransform_.translation_, bulletOffset_);

						boxRed_->Initialize(model_, bulletPosition, redBox_);

						REDs_.push_back(boxRed_);
						bButtonReleased_ = false; // 十字キーが押されたことを記録
						tim_ = 20;
					}
				} else {
					bButtonReleased_ = true; // 十字キーがリリースされたことを記録
				}
			}

		}
		// 動き
		else {
			if (redMove_ == true) {
				lLetGo_ = true;
				rLetGo_ = true;
				bulletOffset_.x = 0.0f;
				TIM_--;

				if (TIM_ <= 0) {
					boxSpeedRed_ = 2.0f;
				}
			}
		}
		// 青
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			blueMove_ = true;
			redMove_ = false;
			boxSpeedBlue_ = 0.0f;
			TIM_ = 180;
			// 右
			if (rLetGo_ == true) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
					lLetGo_ = false;
					if (bButtonReleased_) {
						BoxType* boxBlue_ = new BoxType;
						bulletOffset_.x += 2.0f;
						// 弾の位置を計算してオフセットを適用
						Vector3 bulletPosition = Add(worldTransform_.translation_, bulletOffset_);

						boxBlue_->Initialize(model_, bulletPosition, blueBox_);

						BLUEs_.push_back(boxBlue_);
						bButtonReleased_ = false; // 十字キーが押されたことを記録
						tim_ = 20;
					}
				} else {
					bButtonReleased_ = true; // 十字キーがリリースされたことを記録
					RL_ = true;
				}
			}
			// 左
			if (lLetGo_ == true) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
					rLetGo_ = false;
					if (bButtonReleased_) {
						BoxType* boxBlue_ = new BoxType;
						bulletOffset_.x -= 2.0f;
						// 弾の位置を計算してオフセットを適用
						Vector3 bulletPosition = Add(worldTransform_.translation_, bulletOffset_);

						boxBlue_->Initialize(model_, bulletPosition, blueBox_);

						BLUEs_.push_back(boxBlue_);
						bButtonReleased_ = false; // 十字キーボタンが押されたことを記録
						tim_ = 20;
					}
				} else {
					bButtonReleased_ = true; // 十字キーがリリースされたことを記録
					RL_ = false;
				}
			}

		}
		// 動き
		else {
			if (blueMove_ == true) {

				lLetGo_ = true;
				rLetGo_ = true;
				bulletOffset_.x = 0.0f;
				TIM_--;
				if (TIM_ <= 0) {
					boxSpeedBlue_ = 2.0f;
				}
			}
		}
	}
}



/// <summary>
/// 速度を設定する
/// </summary>
void Player::SetVelocity(const Vector3& newVelocity) {

	velocity_ = newVelocity;
}



/// <summary>
/// プレイヤー座標とボックスの座標がどうのこうのする処理
/// </summary>
void Player::AddJustPositionOnBox(float newPosY) {

	worldTransform_.translation_.y = newPosY;
	// ここでジャンプの状態をリセットするかどうかの処理を追加
	velocity_.y = 0.0f;
}



/// <summary>
/// プレイヤーとボックスの衝突処理
/// </summary>
void Player::HitBox() {

	// PlayerのAABBを取得
	AABB playerAABB = GetAABB();

	// 赤の箱に対しての処理
	for (BoxType* box : REDs_) {
		// BoxTypeのAABBを取得
		AABB boxAABB = box->GetAABB();
		
		// PlayerとBoxのAABBが交差しているか判定
		if (AABBAABB::onCollision(playerAABB, boxAABB)) {
			// PlayerがBoxの上にいる場合、PlayerのY座標をBoxの上に調整
			float newPlayerY = boxAABB.max.y + playerHeight_ / 2;
			AddJustPositionOnBox(newPlayerY);
		}
	}

	// 青の箱に対しての処理
	for (BoxType* box : BLUEs_) {
		// BoxTypeのAABBを取得
		AABB boxAABB = box->GetAABB();

		// PlayerとBoxのAABBが交差しているか判定
		if (AABBAABB::onCollision(playerAABB, boxAABB)) {
			// PlayerがBoxの上にいる場合、PlayerのY座標をBoxの上に調整
			float newPlayerY = boxAABB.max.y + playerHeight_ / 2;
			AddJustPositionOnBox(newPlayerY);
		}
	}
}



void Player::AddGroundMap(Map* ground) {
	MapGrounds_.push_back(ground);
}

