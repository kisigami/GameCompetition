#include "stdafx.h"
#include "LongSwordEnemy.h"
#include "Player.h"
#include "Tower.h"
#include "IceBall.h"
#include "Game.h"
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

LongSwordEnemy::LongSwordEnemy()
{

}

LongSwordEnemy::~LongSwordEnemy()
{

}

bool LongSwordEnemy::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/longsword/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Assult].Load("Assets/animData/longsword/walk.tka");
	m_animationClips[enAnimationClip_Assult].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/longsword/attack2.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Attack2].Load("Assets/animData/longsword/attack.tka");
	m_animationClips[enAnimationClip_Attack2].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveDamage].Load("Assets/animData/longsword/damage.tka");
	m_animationClips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/longsword/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_MagicAttack].Load("Assets/animData/longsword/magic.tka");
	m_animationClips[enAnimationClip_MagicAttack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ElectricShock].Load("Assets/animData/longsword/electric.tka");
	m_animationClips[enAnimationClip_ElectricShock].SetLoopFlag(false);


	m_modelRender.Init("Assets/modelData/enemy/longsword/longsword3.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	m_charaCon.Init(
		120.0f,			//半径
		220.0f,			//高さ
		m_position      //座標
	);

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

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

const bool LongSwordEnemy::SearchPlayer() const
{
	//プレイヤーを見つける
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 2000.0 * 2000.0f)
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

const bool LongSwordEnemy::IsCanPlayerAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 600.0f * 600.0f)
	{
		return true;
	}
	return false;
}
const bool LongSwordEnemy::IsCanTowerAttack() const
{
	Vector3 diff = m_tower->GetPosition() - m_position;
	if (diff.LengthSq() <= 600.0f * 600.0f)
	{
		return true;
	}
	return false;
}


void LongSwordEnemy::Update()
{ 
	Electric();
	Chase();
	Assult();
	Rotation();
	Attack();
	Collision();
	ManageState();
	PlayAnimation();

	m_modelRender.Update();
}


void LongSwordEnemy::Electric()
{
	if (m_enemyState != enEnemyState_ElectricShock)
	{
		return;
	}
	electrictimer -= g_gameTime->GetFrameDeltaTime();
}

void LongSwordEnemy::MakeMagic()
{
	IceBall* iceBall = NewGO<IceBall>(0);
	Vector3 iceBallPosition = m_position;
	iceBallPosition.y += 55.0f;
	iceBallPosition += m_forward * 300.0f;
	iceBall->SetPosition(iceBallPosition);
	iceBall->SetRotation(m_rotation);
	iceBall->SetEnUser(IceBall::enUser_Enemy);
}

void LongSwordEnemy::Assult()
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

void LongSwordEnemy::Chase()
{
	//突撃ステートではなかったら
	if (m_enemyState != enEnemyState_Chase)
	{
		//何もしない
		return;
	}
	Vector3 modelPosition = m_position;
	Vector3 diff_player = m_player->GetPosition() - m_position;
	diff_player.Normalize();
	m_moveSpeed.x = diff_player.x * 300.0f;
	m_moveSpeed.z = diff_player.z * 300.0f;
	m_moveSpeed.y -= 6000.0f * g_gameTime->GetFrameDeltaTime();
	m_modelRender.SetPosition(m_position);
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void LongSwordEnemy::Rotation()
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

void LongSwordEnemy::Collision()
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
}

void LongSwordEnemy::Attack()
{
	if (m_enemyState != enEnemyState_Attack&&
		m_enemyState != enEnemyState_Attack2)
	{
		return;
	}
	if (m_isUnderAttack == true)
	{
		MakeAttackCollision();
	}
}

void LongSwordEnemy::MakeAttackCollision()
{
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(120.0f, 700.0f, 20.0f));
	collisionObject->SetName("enemy_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void LongSwordEnemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機
	case enEnemyState_Idle:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
	case enEnemyState_Assult:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Assult, 0.1f);
		break;
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.0f);
		m_modelRender.PlayAnimation(enAnimationClip_Assult, 0.1f);
		break;
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.1f);
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
	case enEnemyState_Attack2:
		m_modelRender.SetAnimationSpeed(1.1f);
		m_modelRender.PlayAnimation(enAnimationClip_Attack2, 0.1f);
		break;
	case enEnemyState_MagicAttack:
		m_modelRender.SetAnimationSpeed(1.6);
		m_modelRender.PlayAnimation(enAnimationClip_MagicAttack, 0.1f);
		break;
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.4f);
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
	case enEnemyState_ElectricShock:
		m_modelRender.SetAnimationSpeed(0.5f);
		m_modelRender.PlayAnimation(enAnimationClip_ElectricShock, 0.1f);
		break;
	case enEnemyState_Down:
		m_modelRender.SetAnimationSpeed(1.4f);
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void LongSwordEnemy::ManageState()
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
	case enEnemyState_Chase:
		ProcessAssultStateTransition();
		break;
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	case enEnemyState_Attack2:
		ProcessAttackStateTransition();
		break;
	case enEnemyState_MagicAttack:
		ProcessMagicAttackStateTransition();
		break;
	case enEnemyState_ElectricShock:
		ProcessElectricShockStateTransition();
		break;
	case enEnemyState_ReceiveDamage:
		ProcessReceiveDamageTransition();
		break;
	case enEnemyState_Down:
		ProcessDownTransition();
		break;
	default:
		break;
	}
}

void LongSwordEnemy::ProcessCommonStateTransition()
{
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
				if (m_hp <= 3)
				{
					m_enemyState = enEnemyState_Attack2;
					m_isUnderAttack = false;
					return;
				}
				//攻撃
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//魔法攻撃
				m_enemyState = enEnemyState_MagicAttack;
				return;
			}
		}

		else {
			diff_tower.Normalize();
			m_moveSpeed = diff_tower * 300.0f;
			ram = rand() % 100;
			if (ram > 30)
			{
				if (m_hp <= 3)
				{
					m_enemyState = enEnemyState_Attack2;
					m_isUnderAttack = false;
					return;
				}
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//魔法攻撃
				m_enemyState = enEnemyState_MagicAttack;
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
			if (m_hp <= 3)
			{
				m_enemyState = enEnemyState_Attack2;
				m_isUnderAttack = false;
				return;
			}

			m_enemyState = enEnemyState_Attack;
			m_isUnderAttack = false;
			return;
		}
		else
		{
			//魔法攻撃
			m_enemyState = enEnemyState_MagicAttack;
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


void LongSwordEnemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void LongSwordEnemy::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	}
}

void LongSwordEnemy::ProcessMagicAttackStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	}
}

void LongSwordEnemy::ProcessAssultStateTransition()
{
	ProcessCommonStateTransition();
}

void LongSwordEnemy::ProcessReceiveDamageTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (electrictimer < 5.0f)
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

void LongSwordEnemy::ProcessElectricShockStateTransition()
{
	if (electrictimer <= 0.0f)
	{
		electrictimer = 5.0f;
		ProcessCommonStateTransition();
		return;
	}
}

void LongSwordEnemy::ProcessDownTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		DeleteGO(this);
		m_game->m_DeadEnemynum++;
		return;
	}
}

void LongSwordEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		m_isUnderAttack = true;
	}

	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		m_isUnderAttack = false;
	}
	if (wcscmp(eventName, L"magic_attack") == 0)
	{
		MakeMagic();
	}
}

void LongSwordEnemy::Render(RenderContext& rc)
{
	m_fontRender3.Draw(rc);
	m_modelRender.Draw(rc);
}