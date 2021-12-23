#include "stdafx.h"
#include "IceBall.h"
#include "Player.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"


#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

IceBall::IceBall()
{

}

IceBall::~IceBall()
{
	m_effectEmitter->Stop();
	DeleteGO(m_effectEmitter);
	DeleteGO(m_collisionObject);
}

bool IceBall::Start()
{
	m_player = FindGO<Player>("player");
	EffectEngine::GetInstance()->ResistEffect(15, u"Assets/efk/enemymagic.efk");
	EffectEngine::GetInstance()->ResistEffect(16, u"Assets/efk/iceball.efk");
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/magic3.wav");

	if (m_enUser == enUser_Enemy) {
		m_effectEmitter = NewGO <EffectEmitter>(0);
		m_effectEmitter->Init(15);
		m_effectEmitter->SetScale(m_scale * 1.0f);
		m_moveSpeed = Vector3::AxisZ;
		m_rotation.Apply(m_moveSpeed);
		m_position += m_moveSpeed * 50.0f;
		m_position.y += 160.0f;
		m_moveSpeed *= 1600.0f;
		m_rotation.AddRotationDegY(0.0f);
		m_effectEmitter->SetRotation(m_rotation);
		m_effectEmitter->Play();
		SoundSource* m_se = NewGO<SoundSource>(0);
		m_se->Init(5);
		m_se->SetVolume(0.3f);
		m_se->Play(false);
		m_collisionObject = NewGO<CollisionObject>(0);
		m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 80.0f * m_scale.z);
		m_collisionObject->SetIsEnableAutoDelete(false);
	}

	else {
		m_effectEmitter = NewGO <EffectEmitter>(0);
		m_effectEmitter->Init(16);
		m_effectEmitter->SetScale(m_scale * 1.0f);
		m_moveSpeed = Vector3::AxisZ;
		m_rotation.Apply(m_moveSpeed);
		m_position += m_moveSpeed * 50.0f;
		m_position.y += 160.0f;
		m_moveSpeed *= 1900.0f;
		m_rotation.AddRotationDegY(0.0f);
		m_effectEmitter->SetRotation(m_rotation);
		m_effectEmitter->Play();
		SoundSource* m_se = NewGO<SoundSource>(0);
		m_se->Init(5);
		m_se->SetVolume(0.3f);
		m_se->Play(false);
		m_collisionObject = NewGO<CollisionObject>(0);
		m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 80.0f * m_scale.z);
		m_collisionObject->SetIsEnableAutoDelete(false);
	}

	if (m_enUser == enUser_Player)
	{
		m_collisionObject->SetName("player_magic");
	}
	else if (m_enUser == enUser_Enemy)
	{
		m_collisionObject->SetName("enemy_magic");
	}
	return true;
}

void IceBall::Update()
{
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack");
	//配列をfor文で回す
	for (auto collision : collisions2)
	{
		//コリジョンとプレイヤーの攻撃が衝突したら
		if (collision->IsHit(m_collisionObject))
		{
			DeleteGO(this);
			m_player->m_mp += 5.0f;
		}
	}

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


