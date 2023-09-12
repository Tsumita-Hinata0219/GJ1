#pragma once
#include <ImGuiManager.h>
#include <Input.h>
#include <Model.h>
#include <WorldTransform.h>

#include "Collider.h"
#include "CollisionConfig.h"
#include "Function.h"

#include <list>

class DemoPlayer : public Collider {

public:
	DemoPlayer();
	~DemoPlayer();

	void Initialize(Model* model, Vector3 position);

	void Update();

	void Draw(ViewProjection viewProjection);

	void Move();

	void CalcAABB();

	void SetIsHit(bool falg) { isHit = falg; }

	void onCollision() override;

	Vector3 GetWorldPosition() override;

	AABB GetAABB() override;

private:
	WorldTransform worldTransform_; // ���[���h�ϊ��f�[�^

	Model* model_ = nullptr; // ���f��

	uint32_t playerTextureHandle_ = 0u; // �e�N�X�`���n���h��

	Input* input_ = nullptr; // �L�[�{�[�h����

	const float radius_ = 2.0f; // ���a

	AABB aabb_;

	bool isHit;
};