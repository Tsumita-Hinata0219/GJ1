#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	//* 解放処理 *//

	// Player プレイヤー
	//delete player_;
	delete demoPlayer_;

	// Model モデル
	delete model_;

	// Map マップ
	for (int index = 0; index < 4; index++) {
		for (Map* map : mapBoxs_[index]) {
			delete map;
		}
	}

	// Skydome 天球
	delete skydome_;

	// ゲームカメラ
	delete gameCamera_;

	// コライダー
	delete collisionManager_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 1000.0f;
	viewProjection_.Initialize();



	/* ----- Scene シーン ----- */
	// シーン
	scene_ = TITLE;
	// 現在選択しているステージ
	nowStageSelect_ = 0;



	/* ----- 背景スプライト ----- */
	// テクスチャ
	titleTextureHandle_ = TextureManager::Load("/picture/Title.png");
	stageSelectTextureHandle_ = TextureManager::Load("/picture/StageSelect.png");
	gameClearTextureHandle_ = TextureManager::Load("/picture/GameClear.png");
	gameOverTextureHandle_ = TextureManager::Load("/picture/GameOver.png");
	pushATextureHandle_ = TextureManager::Load("/picture/pushA.png");
	pushBTextureHandle_ = TextureManager::Load("/picture/pushB.png");
	pushXTextureHandle_ = TextureManager::Load("/picture/pushX.png");
	pushYTextureHandle_ = TextureManager::Load("/picture/pushY.png");
	pushLeftTextureHandle_ = TextureManager::Load("/picture/pushLeft.png");
	pushRightTextureHandle_ = TextureManager::Load("/picture/pushRight.png");
	// スプライト
	titleSprite_ = Sprite::Create(titleTextureHandle_, {0.0f, 0.0f});
	stageSelectSprite_ = Sprite::Create(stageSelectTextureHandle_, {0.0f, 0.0f});
	gameClearSprite_ = Sprite::Create(gameClearTextureHandle_, {0.0f, 0.0f});
	gameOverSprite_ = Sprite::Create(gameOverTextureHandle_, {0.0f, 0.0f});
	pushASprite_ = Sprite::Create(pushATextureHandle_, {448.0f, 473.0f});
	pushBSprite_ = Sprite::Create(pushBTextureHandle_, {448.0f, 550.0f});
	pushXSprite_ = Sprite::Create(pushXTextureHandle_, {448.0f, 473.0f});
	pushYSprite_ = Sprite::Create(pushYTextureHandle_, {448.0f, 473.0f});
	pushLeftSprite_ = Sprite::Create(pushLeftTextureHandle_, {320.0f, 550.0f});
	pushRightSprite_ = Sprite::Create(pushRightTextureHandle_, {832.0f, 550.0f});



	/* ----- Map マップ ----- */
	// マップボックスのテクスチャを読み込む
	stage_.boxTexture[0] = TextureManager::Load("/picture/uvChecker.png");
	stage_.boxTexture[1] = TextureManager::Load("/picture/num1.png");
	stage_.boxTexture[2] = TextureManager::Load("/picture/num2.png");
	stage_.boxTexture[3] = TextureManager::Load("/picture/num3.png");
	stage_.boxTexture[5] = TextureManager::Load("/picture/num5.png");
	// ステージのCSVを読み込む
	stage_.stageCSV[0] = ("Resources/parameter/Map1.csv");
	stage_.stageCSV[1] = ("Resources/parameter/Map2.csv");
	stage_.stageCSV[2] = ("Resources/parameter/Map3.csv");
	stage_.stageCSV[3] = ("Resources/parameter/Map4.csv");
	// 生成
	map_ = new Map();
	Vector3 position(0.0f, 0.0f, 0.0f);
	// 初期化
	map_->Initialize(model_, position, stage_.boxTexture[0], kCollisionAttributeMapBox_Ground);
	// ゲームシーンを渡す
	map_->SetGameScene(this);
	// CSVを読み込む
	for (int index = 0; index < 4; index++) {
		LoadMapData(stage_.stageCSV[index], index);
	}
	// 現在進行中のステージ
	nowMap_ = 0;



	/* ----- Player 自キャラ ----- */
	// player_ = new Player();
	// Vector3 playerPosition(0.0f, 10.0f, 0.0f);
	// player_->Initialize(model_, playerPosition);
	demoPlayer_ = new DemoPlayer();
	Vector3 deoPlayerPos(0.0f, 0.0f, 0.0f);
	demoPlayer_->Initialize(model_, deoPlayerPos);



	/* ----- Skydome 天球 ----- */
	// モデルの読み込み
	modelSkydome_ = Model::CreateFromOBJ("Skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);



	/* ----- CollisionManager 当たり判定 ----- */
	collisionManager_ = new CollisionManager();



	/* ----- GameCamera ゲームカメラ ----- */
	gameCamera_ = new GameCamera();
	gameCameraTransform_.translation_ = { 25.0f, -23.5f, -64.0f };
	gameCamera_->Initialize(gameCameraTransform_);


	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);


#ifdef _DEBUG

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

#endif
}

void GameScene::Update() {


	switch (scene_) {

		// タイトル
	    case TITLE:

			// ボタンを押したらシーン変更
			if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
			return;
			}
			if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				scene_ = STAGESELECT;
			nowStageSelect_ = 1;
			}

			break;


		// ステージセレクト
		case STAGESELECT:

			// ボタンを押したらシーン変更
		    if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
			    return;
		    }
			// Xボタンでタイトルに戻る
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			    scene_ = TITLE;
		    }
			// Aボタンで選択しているステージに移動
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			    
				if (nowStageSelect_ > 0) {

					// 現在選択しているステージに移動
				    if (nowStageSelect_ == 1) {
					    scene_ = STAGE1;
					    nowMap_ = 0;
				    } 
					else if (nowStageSelect_ == 2) {
					    scene_ = STAGE2;
					    nowMap_ = 1;
				    } 
					else if (nowStageSelect_ == 3) {
					    scene_ = STAGE3;
					    nowMap_ = 2;
				    } 
					else if (nowStageSelect_ == 4) {
					    scene_ = STAGE4;
					    nowMap_ = 3;
				    }
				}
		    }

		    if (nowStageSelect_ > 4) {
			    nowStageSelect_ = 1;
		    } 
			else if (nowStageSelect_ < 1) {
			    nowStageSelect_ = 4;
			}
			// 十字左右キーでステージセレクト
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
			    nowStageSelect_ = nowStageSelect_ - 1;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
			    nowStageSelect_ = nowStageSelect_ + 1;
		    }
		   

			break;


		// ステージ１
		case STAGE1:

			/* ----- Player 自キャラ ----- */
		    // player_->Update();
		    demoPlayer_->Update();

		    /* ----- Map マップ ----- */
		    for (Map* map : mapBoxs_[0]) {
			    map->Update();
		    }
		    // マップ生成スクリプト実行
		    UpdateMapData(nowMap_);

		    /* ----- Skydome 天球 ----- */
		    skydome_->Update();

		    /* ----- CollisionManager 当たり判定 ----- */
		    CheckAllCollision();

			// ボタンを押したらシーン変更
		    if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
			    return;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			    scene_ = STAGESELECT;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			    scene_ = GAMECLEAR;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			    scene_ = GAMEOVER;
		    }

			break;


		// ステージ2
		case STAGE2:

			/* ----- Player 自キャラ ----- */
		    // player_->Update();
		    demoPlayer_->Update();

		    /* ----- Map マップ ----- */
		    for (Map* map : mapBoxs_[1]) {
			    map->Update();
		    }
		    // マップ生成スクリプト実行
		    UpdateMapData(nowMap_);

		    /* ----- Skydome 天球 ----- */
		    skydome_->Update();

		    /* ----- CollisionManager 当たり判定 ----- */
		    CheckAllCollision();

			// ボタンを押したらシーン変更
		    if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
			    return;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			    scene_ = STAGESELECT;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			    scene_ = GAMECLEAR;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			    scene_ = GAMEOVER;
		    }

			break;


		// ステージ3
	    case STAGE3:

			/* ----- Player 自キャラ ----- */
		    // player_->Update();
		    demoPlayer_->Update();

		    /* ----- Map マップ ----- */
		    for (Map* map : mapBoxs_[2]) {
			    map->Update();
		    }
		    // マップ生成スクリプト実行
		    UpdateMapData(nowMap_);

		    /* ----- Skydome 天球 ----- */
		    skydome_->Update();

		    /* ----- CollisionManager 当たり判定 ----- */
		    CheckAllCollision();

			// ボタンを押したらシーン変更
		    if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
			    return;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			    scene_ = STAGESELECT;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			    scene_ = GAMECLEAR;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			    scene_ = GAMEOVER;
		    }

			break;


		// ステージ4
	    case STAGE4:

			/* ----- Player 自キャラ ----- */
		    // player_->Update();
		    demoPlayer_->Update();

		    /* ----- Map マップ ----- */
		    for (Map* map : mapBoxs_[3]) {
			    map->Update();
		    }
		    // マップ生成スクリプト実行
		    UpdateMapData(nowMap_);

		    /* ----- Skydome 天球 ----- */
		    skydome_->Update();

		    /* ----- CollisionManager 当たり判定 ----- */
		    CheckAllCollision();

			// ボタンを押したらシーン変更
		    if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
			    return;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			    scene_ = STAGESELECT;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			    scene_ = GAMECLEAR;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
			    scene_ = GAMEOVER;
		    }

			break;


		// ゲームクリア
	    case GAMECLEAR:

			// ボタンを押したらシーン変更
		    if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
			    return;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			    scene_ = STAGESELECT;
		    }

			break;


		// ゲームオーバー
	    case GAMEOVER:

			// ボタンを押したらシーン変更
		    if (!Input::GetInstance()->GetJoystickState(0, joyState_)) {
			    return;
		    }
		    if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			    scene_ = STAGESELECT;
		    }

			break;
	}


#ifdef _DEBUG

	// エンターでカメラの切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}

	ImGui::Begin("nowScene");

	ImGui::Text("sceene = %d", scene_);
	ImGui::Text("TITLE = 0");
	ImGui::Text("STAGESELECT = 1");
	ImGui::Text("STAGE1 = 2");
	ImGui::Text("STAGE2 = 3");
	ImGui::Text("STAGE3 = 4");
	ImGui::Text("STAGE4 = 5");
	ImGui::Text("GAMECLEAR = 6");
	ImGui::Text("GAMEOVER = 7");

	ImGui::End();


	ImGui::Begin("nowStageSelect");

	ImGui::Text("stageSelect = %d", nowStageSelect_);
	ImGui::Text("STAGE1 = 1");
	ImGui::Text("STAGE2 = 2");
	ImGui::Text("STAGE3 = 3");
	ImGui::Text("STAGE4 = 4");

	ImGui::End();

	
	ImGui::Begin("nowMap");

	ImGui::Text("map = %d", nowMap_);
	ImGui::Text("STAGE1 = 0");
	ImGui::Text("STAGE2 = 1");
	ImGui::Text("STAGE3 = 2");
	ImGui::Text("STAGE4 = 3");

	ImGui::End();

#endif // DEBUG

	/* ----- Camera カメラ ----- */

	// カメラの処理
	if (isDebugCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {

		gameCamera_->Update();
		viewProjection_.matView = gameCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = gameCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
}

void GameScene::CheckAllCollision() {

	// コライダーリストをクリアにする
	collisionManager_->ColliderClear();

	// コライダー
	std::list<Collider*> colliders_;

	// コライダーをリストに登録
	// プレイヤー
	//collisionManager_->ColliderPushBack(player_);
	collisionManager_->ColliderPushBack(demoPlayer_);

	// マップボックス
	for (int index = 0; index < 4; index++) {
		for (Map* map : mapBoxs_[index]) {
			collisionManager_->ColliderPushBack(map);
		}
	}

	// コライダー2つの衝突判定 今回はAABB
	collisionManager_->CheckRoundRobin();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>


	switch (scene_) {

	// タイトル
	case TITLE:

		titleSprite_->Draw();
		pushASprite_->Draw();

		break;

	// ステージセレクト
	case STAGESELECT:

		stageSelectSprite_->Draw();
		pushBSprite_->Draw();
		pushLeftSprite_->Draw();
		pushRightSprite_->Draw();


		break;

	// ゲームクリア
	case GAMECLEAR:

		gameClearSprite_->Draw();
		pushASprite_->Draw();

		break;

	// ゲームオーバー
	case GAMEOVER:

		gameOverSprite_->Draw();
		pushASprite_->Draw();

		break;
	}



	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>


	switch (scene_) {

	case STAGE1:

		/* ----- Player 自キャラ ----- */
		// player_->Draw(viewProjection_);
		demoPlayer_->Draw(viewProjection_);

		/* ----- Map マップ ----- */
		// マップを描画する
		for (Map* map : mapBoxs_[0]) {
			map->Draw(viewProjection_);
		}

		/* ----- Skydome 天球 ----- */
		skydome_->Draw(viewProjection_);

		break;

	case STAGE2:

		/* ----- Player 自キャラ ----- */
		// player_->Draw(viewProjection_);
		demoPlayer_->Draw(viewProjection_);

		/* ----- Map マップ ----- */
		// マップを描画する
		for (Map* map : mapBoxs_[1]) {
			map->Draw(viewProjection_);
		}

		/* ----- Skydome 天球 ----- */
		skydome_->Draw(viewProjection_);

		break;

	case STAGE3:

		/* ----- Player 自キャラ ----- */
		// player_->Draw(viewProjection_);
		demoPlayer_->Draw(viewProjection_);

		/* ----- Map マップ ----- */
		// マップを描画する
		for (Map* map : mapBoxs_[2]) {
			map->Draw(viewProjection_);
		}

		/* ----- Skydome 天球 ----- */
		skydome_->Draw(viewProjection_);

		break;

	case STAGE4:

		/* ----- Player 自キャラ ----- */
		// player_->Draw(viewProjection_);
		demoPlayer_->Draw(viewProjection_);

		/* ----- Map マップ ----- */
		// マップを描画する
		for (Map* map : mapBoxs_[3]) {
			map->Draw(viewProjection_);
		}

		/* ----- Skydome 天球 ----- */
		skydome_->Draw(viewProjection_);

		break;
	}


	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}



///// <summary>
///// ステージを変更する
///// </summary>
//void GameScene::ChangeStage(int index) {
//
//	for (Map* map : mapBoxs_) {
//		map->SetIsBreak(true);
//	}
//
//	// 元からあったステージを削除する
//	mapBoxs_.remove_if([](Map* map) {
//		if (map->GetIsBreak()) {
//			delete map;
//			return true;
//		}
//		return false;
//	});
//
//	index;
//	//LoadMapData(stage_.stageCSV[index], index);
//}



/// <summary>
/// マップデータの読み込み
/// </summary>
void GameScene::LoadMapData(const char* fileName, int index) {

	// ファイルを開く
	std::ifstream mapFile;
	mapFile.open(fileName);
	assert(mapFile.is_open());

	// ファイルの内容を文字列ストリームにコピー
	mapBoxCreateCommands_[index] << mapFile.rdbuf();

	// ファイルを閉じる
	mapFile.close();
}



/// <summary>
/// マップ生成スクリプト実行
/// </summary>
void GameScene::UpdateMapData(int index) {

	// 1行分の文字列を入れる変数
	std::string line;

	// 読み込んでるマップの現在行
	int lineCount = 0;

	// マップの座標を入れる変数
	Vector3 position{};

	// CVSファイルの最後の行までループを回す
	while (getline(mapBoxCreateCommands_[index], line)) {

		// 読み込んだ一行をカンマで区切り代入
		vector<string> strvec = split(line, ',');

		// 一行のサイズ分繰り返し文を回す
		for (int colmnCount = 0; colmnCount < strvec.size(); colmnCount++) {

			// CSVファイルの中の対応する数字を見つけたらマップを生成する
			if (stoi(strvec.at(colmnCount)) == 1) {

				// 座標を決めてブロックを生成
				GeneratedMap(index, CreateMapVector(colmnCount, lineCount), stage_.boxTexture[1], kCollisionAttributeMapBox_Ground);
			}
			else if (stoi(strvec.at(colmnCount)) == 2) {

				// 座標を決めてブロックを生成
				GeneratedMap(index, CreateMapVector(colmnCount, lineCount), stage_.boxTexture[2], kCollisionAttributeMapBox_Damage);
			}
			else if (stoi(strvec.at(colmnCount)) == 3) {

				// 座標を決めてブロックを生成
				GeneratedMap(index, CreateMapVector(colmnCount, lineCount), stage_.boxTexture[3], kCollisionAttributeMapBox_State);
			} 
			else if (stoi(strvec.at(colmnCount)) == 5) {

				// 座標を決めてブロックを生成
				GeneratedMap(index, CreateMapVector(colmnCount, lineCount), stage_.boxTexture[5], kCollisionAttributeMapBox_Goal);
			}
		}
		// カウントに1を足す
		lineCount = lineCount + 1;
	}
}



/// <summary>
/// マップの座標を決める
/// </summary>
Vector3 GameScene::CreateMapVector(int indexX, int indexY) {

	Vector3 result{};

	result.x = map_->GetMap_BoxSize() * float(indexX);
	result.y = map_->GetMap_BoxSize() * float(-indexY);
	result.z = 0.0f;

	return result;
}



/// <summary>
/// マップを生成する
/// </summary>
void GameScene::GeneratedMap(int index, Vector3 position, uint32_t mapBoxTextureHandle, uint32_t filter) {

	// 生成
	map_ = new Map();
	// 初期化
	map_->Initialize(model_, position, mapBoxTextureHandle, filter);
	// ゲームシーンを渡す
	map_->SetGameScene(this);
	// マップを登録する
	mapBoxs_[index].push_back(map_);
}

