#include "stdafx.h"
#include "MagicEnemy.h"

#include "Game.h"
#include "Player.h"
#include "Tower.h"
#include "MagicBall.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float GRAVITY = 6000.0f;               //重力
	const float MOVE_SPEED = 300.0f;             //移動速度
	const float TOWER_ATTACK_RANGE = 1000.0f;     //タワーを攻撃できる距離
	const float PLAYER_ATTACK_RANGE = 500.0f;    //プレイヤーを攻撃できる距離
	const float RESTRAINT_TIME = 8.0f;           //拘束時間
}

MagicEnemy::MagicEnemy()
{

}

MagicEnemy::~MagicEnemy()
{

}

bool MagicEnemy::Start()
{
	//アニメーションの読み込み
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/magic/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Assault].Load("Assets/animData/magic/run.tka");
	m_animationClips[enAnimationClip_Assault].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/magic/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveDamage].Load("Assets/animData/magic/damage.tka");
	m_animationClips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/magic/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveRestraint].Load("Assets/animData/magic/eletric.tka");
	m_animationClips[enAnimationClip_ReceiveRestraint].SetLoopFlag(false);

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/enemy/magic/magicenemy.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	//座標、回転、大きさの更新
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	//キャラクターコントローラー
	m_charaCon.Init(
		70.0f,			//半径
		200.0f,			//高さ
		m_position      //座標
	);

	//アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//SEの読み込み
	g_soundEngine->ResistWaveFileBank(32, "Assets/sound/enemydamage.wav");

	m_player = FindGO<Player>("player");
	m_tower = FindGO<Tower>("tower");
	m_game = FindGO<Game>("game");
	return true;
}

void MagicEnemy::Update()
{
	//移動処理
	Move();
	//回転処理
	Rotation();
	//当たり判定の処理
	Collision();
	//拘束処理
	Restraint();
	//アニメーションの再生
	PlayAnimation();
	//ステートの遷移処理
	ManageState();

	//モデルの更新
	m_modelRender.Update();
}

void MagicEnemy::Move()
{
	//突撃ステートではなかったら
	if (m_magicenemyState != enMagicEnemyState_Assault)
	{
		//何もしない
		return;
	}

	//重力
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();

	//タワーへの移動処理
	Vector3 diff = m_tower->GetPosition() - m_position;
	diff.Normalize();
	//移動速度
	m_moveSpeed.x = diff.x * MOVE_SPEED;
	m_moveSpeed.z = diff.z * MOVE_SPEED;

	//座標の更新
	m_modelRender.SetPosition(m_position);
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void MagicEnemy::Rotation()
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

void MagicEnemy::MakeMagicBall()
{
	//魔法攻撃を作成する
	MagicBall* magicBall = NewGO<MagicBall>(0);
	Vector3 magicBallPosition = m_position;
	//座標を少し上にする
	magicBallPosition.y += 55.0f;
	//座標を少し前にする
	magicBallPosition += m_forward * 150.0f;
	magicBall->SetPosition(magicBallPosition);
	magicBall->SetRotation(m_rotation);
	//使用者をマジックエネミーにする
	magicBall->SetEnUser(MagicBall::enUser_Enemy);
}

void MagicEnemy::Collision()
{
	//被ダメージステートステートかダウンステートだったら
	if (m_magicenemyState == enMagicEnemyState_ReceiveDamage || m_magicenemyState == enMagicEnemyState_Down)
	{
		//何もしない
		return;
	}

	//プレイヤーの拘束魔法の当たり判定
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("item_thunder");
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			m_magicenemyState = enMagicEnemyState_ReceiveRestraint;
		}
	}

	//プレイヤーの近接攻撃の当たり判定
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack");
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
			//Hpを1減らす
			m_hp -= 1;
			//被ダメージエフェクトを再生する
			MakeDamageEffect();
			//プレイヤーのMpが40以下だったら
			if (m_player->m_mp < 40)
			{
				//プレイヤーのMpを10増やす
				m_player->m_mp += 10;
			}

			//Hpが0になったら
			if (m_hp <= 0)
			{
				//ダウンステートへ
				m_magicenemyState = enMagicEnemyState_Down;
			}
			//Hpが0ではなかったら
			else
			{
				//被ダメージステートへ
				m_magicenemyState = enMagicEnemyState_ReceiveDamage;
			}
		}
	}

	//プレイヤーの魔法攻撃の当たり判定
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("player_magic");
	for (auto collision : collisions3)
	{
		//コリジョンとキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//SEを再生する
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(32);
			m_se->SetVolume(0.3f);
			m_se->Play(false);
			//Hpを1減らす
			m_hp -= 1;
			//HPが0になったら
			if (m_hp == 0)
			{
				//ダウンステートへ
				m_magicenemyState = enMagicEnemyState_Down;
			}
			//Hpが0ではなかったら
			else
			{
				//被ダメージステートへ
				m_magicenemyState = enMagicEnemyState_ReceiveDamage;
			}
		}
	}
}

void MagicEnemy::Restraint()
{
	//拘束ステートではなかったら
	if (m_magicenemyState != enMagicEnemyState_ReceiveRestraint)
	{
		//何もしない
		return;
	}
	//拘束タイマーを減らす
	m_restraintTimer -= g_gameTime->GetFrameDeltaTime();
}

void MagicEnemy::MakeDamageEffect()
{
	//エフェクトを作成する
	EffectEmitter* m_effectEmitter;
	m_effectEmitter = NewGO<EffectEmitter>(0);
	Vector3 m_EffectPosition = m_position;
	Vector3 m_EffectScale = m_scale;
	//座標を少し上にする
	m_EffectPosition.y += 180.0f;
	m_effectEmitter->SetPosition(m_EffectPosition);
	//大きさを設定する
	m_effectEmitter->SetScale(Vector3(0.5f, 1.0f, 1.0f));
	//エフェクトを再生する
	m_effectEmitter->Init(11);
	m_effectEmitter->Play();
}

void MagicEnemy::PlayAnimation()
{
	switch (m_magicenemyState)
	{
		//待機ステートの時
	case enMagicEnemyState_Idle:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//待機アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
		//突撃ステートの時
	case enMagicEnemyState_Assault:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//突撃アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Assault, 0.1f);
		break;
		//攻撃ステートの時
	case enMagicEnemyState_Attack:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//攻撃アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//被ダメージステートの時
	case enMagicEnemyState_ReceiveDamage:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.7f);
		//被ダメージアニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
		//拘束ステートの時
	case enMagicEnemyState_ReceiveRestraint:
		//再生速度
		m_modelRender.SetAnimationSpeed(0.5f);
		//拘束アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveRestraint, 0.1f);
		break;
		//ダウンステートの時
	case enMagicEnemyState_Down:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//ダウンアニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void MagicEnemy::ManageState()
{
	switch (m_magicenemyState)
	{
		//待機ステートの時
	case enMagicEnemyState_Idle:
		//待機ステートのステート遷移処理
		ProcessIdleStateTransition();
		break;
		//突撃ステートの時
	case enMagicEnemyState_Assault:
		//突撃ステートのステート遷移処理
		ProcessAssultStateTransition();
		break;
		//攻撃ステートの時
	case enMagicEnemyState_Attack:
		//攻撃ステートのステート遷移処理
		ProcessAttackStateTransition();
		break;
		//被ダメージステートの時
	case enMagicEnemyState_ReceiveDamage:
		//被ダメージステートのステート遷移処理
		ProcessReceiveDamageStateTransition();
		break;
		//拘束ステートの時
	case enMagicEnemyState_ReceiveRestraint:
		//拘束ステートのステート遷移処理
		ProcessReceiveRestraintStateTransition();
		break;
		//ダウンステートの時
	case enMagicEnemyState_Down:
		//ダウンステートのステート遷移処理
		ProcessDownStateTransition();
		break;
	default:
		break;
	}
}

void MagicEnemy::ProcessCommonStateTransition()
{
	//プレイヤーへ向かうベクトル
	Vector3 diff_player = m_player->GetPosition() - m_position;
	//ベクトルの正規化
	diff_player.Normalize();

	//プレイヤーを攻撃できるなら
	if (IsCanPlayerAttack() == true)
	{
		m_moveSpeed = diff_player * MOVE_SPEED;
		int ram = rand() % 100;
		if (ram > 30)
		{
			//攻撃ステートへ
			m_magicenemyState = enMagicEnemyState_Attack;
			return;
		}
		else
		{
			//待機ステートへ
			m_magicenemyState = enMagicEnemyState_Idle;
			return;
		}
	}

	if (IsCanTowerAttack() == true)
	{
		int ram = rand() % 100;
		if (ram > 30)
		{
			//攻撃ステートへ
			m_magicenemyState = enMagicEnemyState_Attack;
			return;
		}
		else
		{
			//待機ステートへ
			m_magicenemyState = enMagicEnemyState_Idle;
			return;
		}
	}

	//攻撃できないなら
	else
	{
		//突撃ステートへ
		m_magicenemyState = enMagicEnemyState_Assault;
		return;
	}
}

void MagicEnemy::ProcessIdleStateTransition()
{
	//他のステートに遷移する
	ProcessCommonStateTransition();
	return;
}

void MagicEnemy::ProcessAssultStateTransition()
{
	//他のステートに遷移する
	ProcessCommonStateTransition();
	return;
}

void MagicEnemy::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
		return;
	}
}

void MagicEnemy::ProcessReceiveDamageStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//拘束タイマーが減っていたら
		if (m_restraintTimer < RESTRAINT_TIME)
		{
			//拘束ステートへ
			m_magicenemyState = enMagicEnemyState_ReceiveRestraint;
			return;
		}
		else
		{
			//他のステートに遷移する
			ProcessCommonStateTransition();
			return;
		}
	}
}

void MagicEnemy::ProcessReceiveRestraintStateTransition()
{
	//拘束タイマーが0.0f以下だったら
	if (m_restraintTimer <= 0.0f)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
		//拘束タイマーを初期化
		m_restraintTimer = RESTRAINT_TIME;
		return;
	}
}

void MagicEnemy::ProcessDownStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//敵死亡数を増やす
		m_game->m_DeadEnemynum++;
		//自身を削除する
		DeleteGO(this);
		return;
	}
}

void MagicEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"magic_attack") == 0)
	{
		//魔法攻撃を作成する
		MakeMagicBall();
	}
}

const bool MagicEnemy::IsCanTowerAttack() const
{
	//タワーを攻撃できる
	Vector3 diff = m_tower->GetPosition() - m_position;
	if (diff.LengthSq() <= TOWER_ATTACK_RANGE * TOWER_ATTACK_RANGE)
	{
		return true;
	}
	//タワーを攻撃できない
	return false;
}


const bool MagicEnemy::IsCanPlayerAttack() const
{
	//プレイヤーを攻撃できる
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= PLAYER_ATTACK_RANGE * PLAYER_ATTACK_RANGE)
	{
		//地面に足がついていたら
		if (m_charaCon.IsOnGround() == true)
		{
			return true;
		}
	}
	//プレイヤーを攻撃できない
	return false;
}

void MagicEnemy::Render(RenderContext& rc)
{
	//モデルの描画処理
	m_modelRender.Draw(rc);
}
