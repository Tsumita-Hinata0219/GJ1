#include "DemoPlayer.h"



DemoPlayer::DemoPlayer() {}
DemoPlayer::~DemoPlayer() {}



void DemoPlayer::Initialize(Model* model, Vector3 position) {

	// NULL�|�C���^�`�F�b�N
	assert(model);

	this->model_ = model;

	playerTextureHandle_ = TextureManager::Load("/picture/uvChecker.png");

	// �v���C���[
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();

	isHit = 0;

	// �V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();

	// �Փˑ���
	SetCollisionAttribute(kCollisionAttributePlayer);

	uint32_t newCollisionAttribute = 0;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_Ground;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_Damage;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_State;
	newCollisionAttribute = newCollisionAttribute | kCollisionAttributeMapBox_Goal;
	SetCollisionMask(newCollisionAttribute);
}




void DemoPlayer::Update() {

	Move();

	// �s���萔�o�b�t�@�ɓ]��
	worldTransform_.TransferMatrix();

	// �A�t�B���ϊ��s��
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





void DemoPlayer::Draw(ViewProjection viewProjection) {

	model_->Draw(worldTransform_, viewProjection, playerTextureHandle_);
}





void DemoPlayer::Move() {

	Vector3 move = {0.0f, 0.0f, 0.0f}; // �ړ��x�N�g��

	const float kCharacterSpeed = 0.3f; // �ړ����x

	// �����������ňړ��x�N�g����ύX
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	if (input_->PushKey(DIK_R)) {
		isHit = false;
	}

	// �ړ��s��Ɉړ��x�N�g�������Z
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void DemoPlayer::CalcAABB() {

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

void DemoPlayer::onCollision() { isHit = 1; }

Vector3 DemoPlayer::GetWorldPosition() {

	// ���[���h���W���擾
	Vector3 worldPos{};

	// ���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB DemoPlayer::GetAABB() { return aabb_; }