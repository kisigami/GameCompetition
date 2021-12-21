#include "stdafx.h"
#include "GamePoint.h"

#include "graphics/effect/EffectEmitter.h"
#include "collision/CollisionObject.h"

GamePoint::GamePoint()
{

}

GamePoint::~GamePoint()
{
	m_effectEmitter->Stop();
	DeleteGO(m_effectEmitter);
	DeleteGO(m_collisionObject);
}

bool GamePoint::Start()
{
	EffectEngine::GetInstance()->ResistEffect(25, u"Assets/efk/enemydeath.efk");
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->SetScale(Vector3(2.0f, 3.0f, 2.0f));
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->SetPosition(m_position);
	m_effectEmitter->Init(25);
	m_effectEmitter->Play();

	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateSphere(m_position,
		Quaternion::Identity,
		120.0f);

	m_collisionObject->SetIsEnableAutoDelete(false);
	m_collisionObject->SetName("enemy_point");

	return true;
}

void GamePoint::Update()
{

	m_timer -= g_gameTime->GetFrameDeltaTime();

	if (m_timer <= 0.0)
	{
		m_effectEmitter->Stop();
		DeleteGO(this);
	}
}
