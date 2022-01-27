#include "stdafx.h"
#include "Enemy.h"

#include "Game.h"
#include "Player.h"
#include "Tower.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float GRAVITY = 6000.0f;             //重力
	const float MOVE_SPEED = 300.0f;            //移動速度
	const float TOWER_ATTACK_RANGE = 500.0f;     //タワーを攻撃できる距離
	const float PLAYER_ATTACK_RANGE = 500.0f;    //プレイヤーを攻撃できる距離
	const float RESTRAINT_TIME = 8.0f;           //拘束時間
}

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
	m_animationClips[enAnimationClip_ReceiveRestraint].Load("Assets/animData/renormal/electric.tka");
	m_animationClips[enAnimationClip_ReceiveRestraint].SetLoopFlag(false);

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/enemy/renormal/renormal.tkm",
	m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);

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

	//エフェクトの読み込み
	EffectEngine::GetInstance()->ResistEffect(10, u"Assets/efk/enemydamage.efk");
	//サウンドの読み込み
	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/enemydamage.wav");

	m_player = FindGO<Player>("player");
	m_tower = FindGO<Tower>("tower");
	m_game = FindGO<Game>("game");
	return true;
}

void Enemy::Update()
{
	//移動処理
	Move();
	//回転処理
	Rotation();
	//攻撃処理
	Attack();
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

void Enemy::Move()
{
	//突撃ステートではなかったら
	if (m_enemyState != enEnemyState_Assault)
	{
		//何もしない
		return;
	}

	//重力
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();

	//タワーへの突撃処理
	Vector3 diff_tower = m_tower->GetPosition() - GetPosition();
	diff_tower.Normalize();
	//移動速度
	m_moveSpeed.x = diff_tower.x * MOVE_SPEED;
	m_moveSpeed.z = diff_tower.z * MOVE_SPEED;

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
		Vector3(20.0f, 190.0f, 20.0f));
	//コリジョンに名前を付ける
	collisionObject->SetName("enemy_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void Enemy::Collision()
{
	//被ダメージステートかダウンステートだったら
	if (m_enemyState == enEnemyState_ReceiveDamage || m_enemyState == enEnemyState_Down)
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
			//拘束ステートへ
			m_enemyState = enEnemyState_ReceiveRestraint;
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
			m_se->Init(8);
			m_se->SetVolume(0.3f);
			m_se->Play(false);
			//Hpを1減らす
			m_hp -= 1;
			//被ダメージエフェクトを再生する
			MakeDamageEffect();
			//プレイヤーのMpが40より小さかったら
			if (m_player->GetMp() < 40)
			{
				float playermp = m_player->GetMp();
				//プレイヤーのMpを5増やす
				playermp += 5;
				//プレイヤーのMPを設定する
				m_player->SetMp(playermp);
			}
			//HPが0だったら
			if (m_hp == 0)
			{
				//ダウンステートへ
				m_enemyState = enEnemyState_Down;
			}
			//HPが0ではなかったら
			else
			{
				//被ダメージステートへ
				m_enemyState = enEnemyState_ReceiveDamage;
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
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(8);
			se->SetVolume(0.3f);
			se->Play(false);
			//プレイヤーのHpを1減らす
			m_hp -= 1;
			//被ダメージエフェクトを再生する
			MakeDamageEffect();
			//Hpが0だったら
			if (m_hp == 0)
			{
				//ダウンステートへ
				m_enemyState = enEnemyState_Down;
			}
			//Hpが0ではなかったら
			else
			{
				//被ダメージステートへ
				m_enemyState = enEnemyState_ReceiveDamage;
			}
		}
	}
}

void Enemy::Restraint()
{
	//拘束ステートではなかったら
	if (m_enemyState != enEnemyState_ReceiveRestraint)
	{
		//何もしない
		return;
	}
	//拘束タイマーを減らす
	m_restraintTimer -= g_gameTime->GetFrameDeltaTime();
}

void Enemy::MakeDamageEffect()
{
	//エフェクトを作成する
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	Vector3 EffectPosition = m_position;
	Vector3 EffectScale = m_scale;
	//座標を少し上にする
	EffectPosition.y += 180.0f;
	effectEmitter->SetPosition(EffectPosition);
	//大きさを設定する
	effectEmitter->SetScale(Vector3(0.5f, 1.0f, 1.0f));
	//エフェクトを再生する
	effectEmitter->Init(10);
	effectEmitter->Play();
}

void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機ステートの時
	case enEnemyState_Idle:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//待機アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
		//突撃ステートの時
	case enEnemyState_Assault:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.0f);
		//突撃アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Assult, 0.1f);
		break;
		//攻撃ステートの時
	case enEnemyState_Attack:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.4f);
		//攻撃アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//被ダメージステートの時
	case enEnemyState_ReceiveDamage:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.7f);
		//被ダメージアニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
		//拘束ステートの時
	case enEnemyState_ReceiveRestraint:
		//再生速度
		m_modelRender.SetAnimationSpeed(0.5f);
		//拘束アニメーションの再生
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveRestraint, 0.1f);
		break;
		//ダウンステートの時
	case enEnemyState_Down:
		//再生速度
		m_modelRender.SetAnimationSpeed(1.7f);
		//ダウンアニメーションの再生
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
	case enEnemyState_Assault:
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
		ProcessReceiveDamageStateTransition();
		break;
		//拘束ステートの時
	case enEnemyState_ReceiveRestraint:
		//拘束ステートのステート遷移処理
		ProcessReceiveRestraintStateTransition();
		break;
		//ダウンステートの時
	case enEnemyState_Down:
		//ダウンステートのステート遷移処理
		ProcessDownStateTransition();
		break;
	default:
		break;
	}
}

void Enemy::ProcessCommonStateTransition()
{   
	//プレイヤーへ向かうベクトル
	Vector3 diff_player = m_player->GetPosition() - GetPosition();
	//正規化
	diff_player.Normalize();

	//プレイヤーを攻撃できるなら
	if (IsCanPlayerAttack() == true)
	{
		m_moveSpeed = diff_player * MOVE_SPEED;
		int ram = rand() % 100;
		if (ram > 50)
		{
			//攻撃ステートへ
			m_enemyState = enEnemyState_Attack;
			//攻撃の当たり判定を無効にする
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

	//タワーを攻撃できるなら
	if (IsCanTowerAttack() == true)
	{
		int m_ram = rand() % 100;
		if (m_ram > 50)
		{
			//攻撃ステートへ
			m_enemyState = enEnemyState_Attack;
			//攻撃の当たり判定を無効にする
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

	//攻撃できないなら
	else
	{
		//突撃ステートへ
		m_enemyState = enEnemyState_Assault;
		return;
	}
}

void Enemy::ProcessIdleStateTransition()
{
	//他のステートに遷移する
	ProcessCommonStateTransition();
	return;
}

void Enemy::ProcessAssultStateTransition()
{
	//他のステートに遷移する
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

void Enemy::ProcessReceiveDamageStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//拘束タイマーが減っていた
		if (m_restraintTimer < RESTRAINT_TIME)
		{
			//拘束ステートへ
			m_enemyState = enEnemyState_ReceiveRestraint;
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

void Enemy::ProcessReceiveRestraintStateTransition()
{
	//拘束タイマーが０以下だったら
	if (m_restraintTimer <= 0.0f)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
		//拘束タイマーを初期化
		m_restraintTimer = RESTRAINT_TIME;
		return;
	}
}

void Enemy::ProcessDownStateTransition()
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

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//攻撃の当たり判定を有効にする
		m_isUnderAttack = true;
	}

	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//攻撃の当たり判定を無効にする
		m_isUnderAttack = false;
	}
}

const bool Enemy::IsCanTowerAttack() const
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


const bool Enemy::IsCanPlayerAttack() const
{
	//プレイヤーを攻撃できる
	Vector3 diff = m_player->GetPosition() - m_position;
	//プレイヤーとの距離が400.0f以下だったら
	if (diff.LengthSq() <= PLAYER_ATTACK_RANGE * PLAYER_ATTACK_RANGE)
	{
		return true;
	}
	//プレイヤーを攻撃できない
	return false;
}

void Enemy::Render(RenderContext& rc)
{
	//モデルの描画処理
	m_modelRender.Draw(rc);
}
