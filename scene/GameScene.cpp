#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	//* 解放処理 *//

	// Model モデル
	delete model_;

	// Map マップ
	for (Map* map : mapBoxs_) {
		delete map;
	}
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



	/* ----- Map マップ ----- */
	// マップのボックスに使うテクスチャを読み込む
	mapBoxTextureHandle_ = TextureManager::Load("/picture/uvChecker.png");
	// 生成
	map_ = new Map();
	Vector3 position(0.0f, 0.0f, 0.0f);
	// 初期化
	map_->Initialize(model_, position, mapBoxTextureHandle_);
	// ゲームシーンを渡す
	map_->SetGameScene(this);
	// ステージのCSV
	stage_.one = ("Resources/parameter/demoMap1.csv");
	stage_.two = ("Resources/parameter/demoMap2.csv");
	// CSVを読み込む
	LoadMapData(stage_.two);



	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {

	/* ----- Map マップ ----- */
	// マップを生成する
	for (Map* map : mapBoxs_) {
		map->Update();
	}
	// マップ生成スクリプト実行
	UpdateMapData();


	/* ----- Camera カメラ ----- */

#ifdef _DEBUG

	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		} else {
			isDebugCameraActive_ = false;
		}
	}

#endif // DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
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


	/* ----- Map マップ ----- */
	// マップを描画する
	for (Map* map : mapBoxs_) {
		map->Draw(viewProjection_);
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




/// <summary>
/// マップデータの読み込み
/// </summary>
void GameScene::LoadMapData(const char* fileName) {

	// ファイルを開く
	std::ifstream mapFile;
	mapFile.open(fileName);
	assert(mapFile.is_open());

	// ファイルの内容を文字列ストリームにコピー
	mapBoxCreateCommands_ << mapFile.rdbuf();

	// ファイルを閉じる
	mapFile.close();
}



/// <summary>
/// マップ生成スクリプト実行
/// </summary>
void GameScene::UpdateMapData() {

	// 1行分の文字列を入れる変数
	std::string line;

	// 読み込んでるマップの現在行
	int lineCount = 0;

	// マップの座標を入れる変数
	Vector3 position{};

	// CVSファイルの最後の行までループを回す
	while (getline(mapBoxCreateCommands_, line)) {

		// 読み込んだ一行をカンマで区切り代入
		vector<string> strvec = split(line, ',');

		// 一行のサイズ分繰り返し文を回す
		for (int colmnCount = 0; colmnCount < strvec.size(); colmnCount++) {

			// CSVファイルの中の1を見つけたら
			if (stoi(strvec.at(colmnCount)) == 1) {

				// X座標
				float x = map_->GetMap_BoxSize() * float(colmnCount);

				// Y座標
				float y = map_->GetMap_BoxSize() * float(lineCount);

				// Z座標
				float z = 0.0f;

				position = {x, y, z};
			}
			// マップを生成する
			GeneratedMap(position);
		}
		// カウントに1を足す
		lineCount = lineCount + 1;
	}
}



/// <summary>
/// マップを生成する
/// </summary>
void GameScene::GeneratedMap(Vector3 position) {

	// 生成
	map_ = new Map();
	// 初期化
	map_->Initialize(model_, position, mapBoxTextureHandle_);
	// ゲームシーンを渡す
	map_->SetGameScene(this);
	// マップを登録する
	mapBoxs_.push_back(map_);
}