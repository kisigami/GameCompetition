#include "stdafx.h"
#include "Enemy.h"

#include "Game.h"
#include "Player.h"
#include "Tower.h"

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

	//エフェクトの読み込み
	EffectEngine::GetInstance()->ResistEffect(11, u"Assets/efk/enemydamage.efk");

	//サウンドの読み込み
	g_soundEngine->ResistWaveFileBank(32, "Assets/sound/enemydamage.wav");

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
	//拘束処理
	Electric();
	//攻撃処理
	Attack();
	//突撃処理
	Assult();
	//当たり判定
	Collision();
	//回転処理
	Rotation();
	//アニメーションの再生
	PlayAnimation();
	//ステートの遷移処理
	ManageState();
	
	//モデルの更新
	m_modelRender.Update();
}

void Enemy::Attack()
{
	//攻撃ステートではなかったら
	if (m_enemyState != enEnemyState_Attack)
	{
		//何もしない
		return;
	}
	//フラグがtrueなら
	if (m_isUnderAttack == true)
	{
		//攻撃の当たり判定を作成する
		MakeAttackCollision();
	}
}

void Enemy::Electric()
{
	//拘束ステートではなかったら
	if (m_enemyState != enEnemyState_ElectricShock)
	{
		//何もしない
		return;
	}
	//拘束タイマーを減らす
	electrictimer -= g_gameTime->GetFrameDeltaTime();
}

void Enemy::MakeAttackCollision()
{
	//当たり判定を作成する
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//座標を少し前にする
	collisionPosition += m_forward * 50.0f;
	//箱状の当たり判定を作成する
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(20.0f,190.0f, 20.0f));
	//名前を付ける
	collisionObject->SetName("enemy_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void Enemy::MakeDamageEffect()
{
	//エフェクトを作成する
	EffectEmitter* m_effectEmitter;
	m_effectEmitter = NewGO<EffectEmitter>(0);
	Vector3 EffectPosition = m_position;
	Vector3 EffectScale = m_scale;
	m_effectEmitter->SetScale(Vector3(0.5f, 1.0f, 1.0f));
	//座標を上にする
	EffectPosition.y += 180.0f;
	m_effectEmitter->SetPosition(EffectPosition);
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Init(11);
	//エフェクトを再生する
	m_effectEmitter->Play();
}


void Enemy::Collision()
{
	//被ダメージステートかダウンステートだったら
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		//何もしない
		return;
	}

	//スキルの当たり判定
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("item_thunder");
	for (auto collision : collisions3)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//拘束ステートへ
			m_enemyState = enEnemyState_ElectricShock;
		}
	}

	//プレイヤーの近接攻撃の当たり判定
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//SEを再生する
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(32);
			m_se->SetVolume(0.3f);
			m_se->Play(false);

			//Hpを減らす
			m_hp -= 1;

			//被ダメージエフェクトを再生する
			MakeDamageEffect();

			//プレイヤーのMpが40より小さかったら
			if (m_player->m_mp < 40)
			{
				//Mpを増やす
				m_player->m_mp += 5;
			}
			//Hpが0より小さかったら
			if (m_hp <= 0)
			{
				//ダウンステートへ
				m_enemyState = enEnemyState_Down;
			}
			else
			{
				//被ダメージステートへ
				m_enemyState = enEnemyState_ReceiveDamage;
			}
		}
	}

	//プレイヤーの魔法攻撃の当たり判定
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_magic");
	for (auto collision : collisions2)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//SEを再生する
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(32);
			m_se->SetVolume(0.3f);
			m_se->Play(false);

			//Hpを減らす
			m_hp -= 1;

			//被ダメージエフェクトを再生する
			MakeDamageEffect();

			//HPが0になったら。
			if (m_hp == 0)
			{
				//ダウンステートへ
				m_enemyState = enEnemyState_Down;
			}
			//0ではなかったら
			else
			{
				//被ダメージステートへ
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

	//エネミーがタワーへ突撃するベクトル
	Vector3 modelPosition = m_position;
	Vector3 diff_tower = m_tower->GetPosition() - m_position;
	diff_tower.Normalize();

	//移動速度
	m_moveSpeed.x = diff_tower.x * 300.0f;
	m_moveSpeed.z = diff_tower.z * 300.0f;
	
	//重力
	m_moveSpeed.y -= 10000.0f * g_gameTime->GetFrameDeltaTime();

	//座標の更新
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
	//タワーを攻撃できる
	Vector3 diff = m_tower->GetPosition() - m_position;
	if (diff.LengthSq() <= 500.0f * 500.0f)
	{
		return true;
	}
	//タワーを攻撃できない
	return false;
}


const bool Enemy::IsCanPlayerAttack() const
{
	//プレイヤーを攻撃できる
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 400.0f * 400.0f)
	{
		return true;
	}
	//プレイヤーを攻撃できない
	return false;
}


void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機ステートの時
	case enEnemyState_Idle:
		//再生速度
		m_modelRender.SetAnimationSpeed(5.5f);
		//補完時間
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
		//突撃ステートの時
	case enEnemyState_Assult:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//補完時間
		m_modelRender.PlayAnimation(enAnimationClip_Assult, 0.1f);
		break;
		//攻撃ステートの時
	case enEnemyState_Attack:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.4f);
		//補完時間
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//被ダメージステートの時
	case enEnemyState_ReceiveDamage:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.7f);
		//補完時間
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
		//拘束ステートの時
	case enEnemyState_ElectricShock:
		//再生速度
		m_modelRender.SetAnimationSpeed(0.5f);
		//補完時間
		m_modelRender.PlayAnimation(enAnimationClip_ElectricShock, 0.1f);
		break;
		//ダウンステートの時
	case enEnemyState_Down:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.7f);
		//補完時間
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
		//待機ステートの時
	case enEnemyState_Idle:
		//待機ステートのステート遷移処理
		ProcessIdleStateTransition();
		break;
		//突撃ステートの時
	case enEnemyState_Assult:
		//突撃ステートのステート遷移処理
		ProcessAssultStateTransition();
		break;
		//攻撃ステートの時
	case enEnemyState_Attack:
		//攻撃ステートのステート遷移処理
		ProcessAttackStateTransition();
		break;
		//被ダメージステートの時
	case enEnemyState_ReceiveDamage:
		//被ダメージステートのステート遷移処理
		ProcessReceiveDamageTransition();
		break;
		//拘束ステートの時
	case enEnemyState_ElectricShock:
		//拘束ステートのステート遷移処理
		ProcessElectricShockTransition();
		break;
		//ダウンステートの時
	case enEnemyState_Down:
		//ダウンステートのステート遷移処理
		ProcessDownTransition();
		break;
	default:
		break;
	}
}

void Enemy::ProcessCommonStateTransition()
{   
	//プレイヤーへ向かうベクトル
	Vector3 diff_player = m_player->GetPosition() - m_position;
	//タワーへ向かうベクトル
	Vector3 diff_tower = m_tower->GetPosition() - m_position;

	//タワーを攻撃できるなら
	if (IsCanTowerAttack() == true)
	{
		//プレイヤーを攻撃できるなら
		if (IsCanPlayerAttack() == true)
		{
			//プレイヤーへ向かうベクトル
			diff_player.Normalize();
			m_moveSpeed = diff_player * 300.0f;
			int ram = rand() % 100;
			if (ram > 50)
			{
				//攻撃ステートへ
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//待機ステートへ
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}

		else {
			//タワーへ向かうベクトル
			diff_tower.Normalize();
			m_moveSpeed = diff_tower * 300.0f;
			ram = rand() % 100;
			if (ram > 50)
			{
				//攻撃ステートへ
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//魔法攻撃ステートへ
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
	}
	//プレイヤーを攻撃できるなら
	else if (IsCanPlayerAttack() == true)
	{
		//プレイヤーに向かうベクトル
		diff_player.Normalize();
		m_moveSpeed = diff_player * 300.0f;
		int ram = rand() % 100;
		if (ram > 30)
		{
			//攻撃ステートへ
			m_enemyState = enEnemyState_Attack;
			m_isUnderAttack = false;
			return;
		}
		else
		{
			//待機ステートへ
			m_enemyState = enEnemyState_Idle;
			return;
		}
	}
	//プレイヤーもタワーも攻撃できないなら
	else
	{
		//突撃ステートへ
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
