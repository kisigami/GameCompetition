#include "stdafx.h"
#include "Point.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

Point::Point()
{

}

Point::~Point()
{

}

bool Point::Start()
{
	EffectEngine::GetInstance()->ResistEffect(19, u"Assets/efk/enemydeath.efk");
	m_effectEmitter = NewGO <EffectEmitter>(0);
	m_effectEmitter->Init(19);
	m_effectEmitter->SetScale(m_scale * 0.8f);
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();
	
	//m_collisionObject = NewGO<CollisionObject>(0);
	//m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 80.0f * m_scale.z);

	return true;
}

void Point::Update()
{
	m_effectEmitter->SetPosition(m_position);
	m_collisionObject->SetPosition(m_position);

	m_timer += g_gameTime->GetFrameDeltaTime();

	if (m_timer >= 5.0f)
	{
		//©g‚ğíœ‚·‚éB
		DeleteGO(this);
	}
}