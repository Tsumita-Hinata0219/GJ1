#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	//* 解放処理 *//

	// Player プレイヤー
	delete player_;

	// Model モデル
	delete model_;

	// Map マップ
	for (Map* map : mapBoxs_) {
		delete map;
	}

	// Skydome 天球
	delete skydome_;

	// ゲームカメラ
	delete gameCamera_;
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



	/* ----- Player 自キャラ ----- */
	player_ = new Player();
	Vector3 playerPosition(0.0f, 10.0f, 0.0f);
	player_->Initialize(model_, playerPosition);



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
	map_->Initialize(model_, position, stage_.boxTexture[0]);
	// ゲームシーンを渡す
	map_->SetGameScene(this);
	// CSVを読み込む
	for (int index = 0; index < 4; index++) {
		LoadMapData(stage_.stageCSV[index], index);
	}
	// 現在進行中のステージ
	nowMap_ = 0;


	/* ----- Skydome 天球 ----- */
	// モデルの読み込み
	modelSkydome_ = Model::CreateFromOBJ("Skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);



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


	/* ----- Player 自キャラ ----- */
	player_->Update();


	/* ----- Map マップ ----- */
	// マップを生成する
	for (Map* map : mapBoxs_) {
		map->Update();
	}
	// マップ生成スクリプト実行
	UpdateMapData(nowMap_);



	/* ----- Skydome 天球 ----- */
	skydome_->Update();


	onCollision();
	//CheckAllCollision();

#ifdef _DEBUG


	// エンターでカメラの切り替え
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}

	// キー操作でマップ変更
	if (input_->TriggerKey(DIK_LEFT)) {

		nowMap_ = nowMap_ - 1;

		ChangeStage(nowMap_);
	}
	if (input_->TriggerKey(DIK_RIGHT)) {

		nowMap_ = nowMap_ + 1;

		ChangeStage(nowMap_);
	}
	if (nowMap_ > 3) {
		nowMap_ = 0;
	} else if (nowMap_ < 0) {
		nowMap_ = 3;
	}

	
	ImGui::Begin("nowMap");

	ImGui::Text("%d", nowMap_);

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

void GameScene::onCollision() {

	// コライダー
	std::list<Collider*> colliders_;

	// コライダーをリストに登録
	colliders_.push_back(player_);
	for (Map* map : mapBoxs_) {
		colliders_.push_back(map);
	}
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
			CheckCollisitionPair(colliderA, colliderB);
		}
	}
}

void GameScene::CheckCollisitionPair(Collider* colliderA, Collider* colliderB) {

	// 衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
	    (colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {

		// 属性判定の時点で当たらないペアの場合は、早期リターン
		return;
	}

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

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

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

	/* ----- Player 自キャラ ----- */
	player_->Draw(viewProjection_);



	/* ----- Map マップ ----- */
	// マップを描画する
	for (Map* map : mapBoxs_) {
		map->Draw(viewProjection_);
	}



	/* ----- Skydome 天球 ----- */
	skydome_->Draw(viewProjection_);




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



/// <summary>
/// ステージを変更する
/// </summary>
void GameScene::ChangeStage(int index) {

	for (Map* map : mapBoxs_) {
		map->SetIsBreak(true);
	}

	// 元からあったステージを削除する
	mapBoxs_.remove_if([](Map* map) {
		if (map->GetIsBreak()) {
			delete map;
			return true;
		}
		return false;
	});

	index;
	//LoadMapData(stage_.stageCSV[index], index);
}



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
				GeneratedMap(CreateMapVector(colmnCount, lineCount), stage_.boxTexture[1]);
			}
			else if (stoi(strvec.at(colmnCount)) == 2) {

				// 座標を決めてブロックを生成
				GeneratedMap(CreateMapVector(colmnCount, lineCount), stage_.boxTexture[2]);
			}
			else if (stoi(strvec.at(colmnCount)) == 3) {

				// 座標を決めてブロックを生成
				GeneratedMap(CreateMapVector(colmnCount, lineCount), stage_.boxTexture[3]);
			} 
			else if (stoi(strvec.at(colmnCount)) == 5) {

				// 座標を決めてブロックを生成
				GeneratedMap(CreateMapVector(colmnCount, lineCount), stage_.boxTexture[5]);
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
void GameScene::GeneratedMap(Vector3 position, uint32_t mapBoxTextureHandle) {

	// 生成
	map_ = new Map();
	// 初期化
	map_->Initialize(model_, position, mapBoxTextureHandle);
	// ゲームシーンを渡す
	map_->SetGameScene(this);
	// マップを登録する
	mapBoxs_.push_back(map_);
}

