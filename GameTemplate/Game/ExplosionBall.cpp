#include "stdafx.h"
#include "ExplosionBall.h"
#include "Player.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"


ExplosionBall::ExplosionBall()
{

}

ExplosionBall::~ExplosionBall()
{

}

bool ExplosionBall::Start()
{
	m_player = FindGO<Player>("player");
	EffectEngine::GetInstance()->ResistEffect(10, u"Assets/efk/explosion.efk");

	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(10);
	m_effectEmitter->SetScale(m_scale* 1.5f);
	m_moveSpeed = Vector3::AxisZ;
	m_rotation.Apply(m_moveSpeed);
	m_position += m_moveSpeed * 50.0f;
	m_position.y += 160.0f;
	m_moveSpeed *= 900.0f;
	m_rotation.AddRotationDegY(0.0f);
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 80.0f * m_scale.z);
	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("explosion_magic");
	return true;
}

void ExplosionBall::Update()
{
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	m_effectEmitter->SetPosition(m_position);
	m_collisionObject->SetPosition(m_position);

	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer >= 5.0f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}

void ExplosionBall::Render(RenderContext& rc)
{

}
