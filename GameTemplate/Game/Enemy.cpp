#include "stdafx.h"
#include "Enemy.h"

#include "Game.h"
#include "Player.h"
#include "Tower.h"

#include "GamePoint.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"


Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	//アニメーションの読み込み
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/renormal/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Assult].Load("Assets/animData/renormal/run.tka");
	m_animationClips[enAnimationClip_Assult].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/renormal/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveDamage].Load("Assets/animData/renormal/damage.tka");
	m_animationClips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/renormal/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ElectricShock].Load("Assets/animData/renormal/electric.tka");
	m_animationClips[enAnimationClip_ElectricShock].SetLoopFlag(false);

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/enemy/renormal/renormal.tkm",
	m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);

	EffectEngine::GetInstance()->ResistEffect(11, u"Assets/efk/explosion.efk");

	//m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	//m_modelRender.Update();

	//キャラクターコントローラー
	m_charaCon.Init(
		70.0f,			//半径
		200.0f,			//高さ
		m_position      //座標
	);

	//剣のボーンのIDを取得
	m_swordBoneId = m_modelRender.FindBoneID(L"sword");
	//アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_player = FindGO<Player>("player");
	m_tower = FindGO<Tower>("tower");
	m_game = FindGO<Game>("game");
	return true;
}

void Enemy::Update()
{
	Electric();
	Attack();
	Assult();
	Collision();
	Rotation();
	PlayAnimation();
	ManageState();
	
	m_modelRender.Update();
}

void Enemy::Attack()
{
	if (m_enemyState != enEnemyState_Attack)
	{
		return;
	}
	if (m_isUnderAttack == true)
	{
		MakeAttackCollision();
	}

}

void Enemy::Electric()
{
	if (m_enemyState != enEnemyState_ElectricShock)
	{
		return;
	}
	electrictimer -= g_gameTime->GetFrameDeltaTime();
}

void Enemy::MakeAttackCollision()
{
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionPosition += m_forward * 50.0f;
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(20.0f,190.0f, 20.0f));
		collisionObject->SetName("enemy_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void Enemy::DeathEffect()
{
	GamePoint* gamePoint = NewGO<GamePoint>(0);
	Vector3 gamePointPosition = m_position;
	//gamePointPosition += m_forward * 10.0f;
	gamePointPosition.y += 30.0f;
	gamePoint->SetPosition(gamePointPosition);
	gamePoint->SetRotation(m_rotation);
}

void Enemy::Collision()
{
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		return;
	}

	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("item_thunder");
	for (auto collision : collisions3)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_enemyState = enEnemyState_ElectricShock;
		}
	}
	
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//Hpを減らす
			m_hp -= 1;
			if (m_player->m_mp < 40)
			{
				m_player->m_mp += 5;
			}
			if (m_hp <= 0)
			{
				//ダウンステートへ
				m_enemyState = enEnemyState_Down;
				m_player->HavePoint += 100;
			}
			else
			{
				//被ダメージステートへ
				m_enemyState = enEnemyState_ReceiveDamage;
			}
		}
	}

	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_magic");
	for (auto collision : collisions2)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 1;
			//HPが0になったら。
			if (m_hp == 0)
			{
				m_enemyState = enEnemyState_Down;
				m_player->HavePoint += 100;
			}
			else
			{
				m_enemyState = enEnemyState_ReceiveDamage;
			}
		}
	}

	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("explosion_magic");
	for (auto collision : collisions4)
	{
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 1;
			//コリジョンオブジェクトを作成する
			CollisionObject * m_collisionObject;
			m_collisionObject = NewGO<CollisionObject>(0);
			//球状のコリジョンを作成する
			m_collisionObject->CreateSphere(m_position,
				Quaternion::Identity,
				300.0f);
			m_collisionObject->SetName("explosion");
			EffectEmitter* effectEmitter1 = NewGO<EffectEmitter>(0);
			effectEmitter1->Init(11);
			effectEmitter1->SetScale(Vector3::One * 8.0f);
			Vector3 EffectPosition = m_position;
			EffectPosition.y += 200.0f;
			effectEmitter1->SetPosition(EffectPosition);
			//エフェクトを再生する
			effectEmitter1->Play();
			//HPが0になったら。
			if (m_hp == 0)
			{
				m_enemyState = enEnemyState_Down;
				m_player->HavePoint += 100;
			}
			else
			{
				m_enemyState = enEnemyState_ReceiveDamage;
			}
		}
	}
}

void Enemy::Assult()
{
	//突撃ステートではなかったら
	if (m_enemyState != enEnemyState_Assult)
	{
		//何もしない
		return;
	}
	Vector3 modelPosition = m_position;
	Vector3 diff_tower = m_tower->GetPosition() - m_position;
	diff_tower.Normalize();
	m_moveSpeed.x = diff_tower.x * 300.0f;
	m_moveSpeed.z = diff_tower.z * 300.0f;
	m_moveSpeed.y -= 10000.0f * g_gameTime->GetFrameDeltaTime();
	m_modelRender.SetPosition(m_position);
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}

	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);

	m_modelRender.SetRotation(m_rotation);

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

const bool Enemy::SearchPlayer() const
{
	//プレイヤーを見つける
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 600.0 * 600.0f)
	{
		diff.Normalize();
		float cos = m_forward.Dot(diff);
		float angle = acosf(cos);
		if (angle <= (Math::PI / 180.0f) * 120.0f)
		{
			return true;
		}
	}
	//プレイヤーを見つけてない
	return false;
}

const bool Enemy::IsCanTowerAttack() const
{
	Vector3 diff = m_tower->GetPosition() - m_position;
	if (diff.LengthSq() <= 500.0f * 500.0f)
	{
		return true;
	}
	return false;
}


const bool Enemy::IsCanPlayerAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 500.0f * 500.0f)
	{
		return true;
	}
	return false;
}


void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機
	case enEnemyState_Idle:
		m_modelRender.SetAnimationSpeed(5.5f);
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enEnemyState_Assult:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Assult, 0.1f);
		break;
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.4f);
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.7f);
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enEnemyState_ElectricShock:
		m_modelRender.SetAnimationSpeed(0.5f);
		m_modelRender.PlayAnimation(enAnimationClip_ElectricShock, 0.1f);
		break;
	case enEnemyState_Down:
		m_modelRender.SetAnimationSpeed(1.7f);
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機
	case enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
	case enEnemyState_Assult:
		ProcessAssultStateTransition();
		break;
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	case enEnemyState_ReceiveDamage:
		ProcessReceiveDamageTransition();
		break;
	case enEnemyState_ElectricShock:
		ProcessElectricShockTransition();
		break;
	case enEnemyState_Down:
		ProcessDownTransition();
		break;
	default:
		break;
	}
}

void Enemy::ProcessCommonStateTransition()
{   
	//エネミーはプレイヤーへの攻撃より塔への攻撃を優先する
	Vector3 diff_player = m_player->GetPosition() - m_position;
	Vector3 diff_tower = m_tower->GetPosition() - m_position;

	if (IsCanTowerAttack() == true)
	{
		if (IsCanPlayerAttack() == true)
		{
			diff_player.Normalize();
			m_moveSpeed = diff_player * 300.0f;
			int ram = rand() % 100;
			if (ram > 30)
			{
				//攻撃
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//魔法攻撃
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}

		else {
			diff_tower.Normalize();
			m_moveSpeed = diff_tower * 300.0f;
			ram = rand() % 100;
			if (ram > 30)
			{
				//攻撃
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//魔法攻撃
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
	}
	else if (IsCanPlayerAttack() == true)
	{
		diff_player.Normalize();
		m_moveSpeed = diff_player * 300.0f;
		int ram = rand() % 100;
		if (ram > 30)
		{
			//攻撃
			m_enemyState = enEnemyState_Attack;
			m_isUnderAttack = false;
			return;
		}
		else
		{
			//魔法攻撃
			m_enemyState = enEnemyState_Idle;
			return;
		}
	}
	else
	{
		//塔に突撃
		m_enemyState = enEnemyState_Assult;
		return;
	}
}

void Enemy::ProcessIdleStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{ 	
		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	}
}

void Enemy::ProcessAssultStateTransition()
{
	ProcessCommonStateTransition();
}

void Enemy::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{ 		
		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	}
}

void Enemy::ProcessReceiveDamageTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (electrictimer < 9.0f)
		{
			m_enemyState = enEnemyState_ElectricShock;
			return;
		}
		else {
			ProcessCommonStateTransition();
			return;
		}
	}
}

void Enemy::ProcessElectricShockTransition()
{
	if (electrictimer <= 0.0f)
	{
		ProcessCommonStateTransition();
		electrictimer = 9.0f;
		return;
	}

}

void Enemy::ProcessDownTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する	
		DeleteGO(this);
		m_game->m_DeadEnemynum++;
		return;
	}
}

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		m_isUnderAttack = true;
	}

	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		m_isUnderAttack = false;
	}
}


void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
