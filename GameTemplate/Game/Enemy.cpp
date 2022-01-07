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
	//�A�j���[�V�����̓ǂݍ���
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

	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/enemy/renormal/renormal.tkm",
	m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);

	//�G�t�F�N�g�̓ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(11, u"Assets/efk/enemydamage.efk");

	//�T�E���h�̓ǂݍ���
	g_soundEngine->ResistWaveFileBank(32, "Assets/sound/enemydamage.wav");

	//�L�����N�^�[�R���g���[���[
	m_charaCon.Init(
		70.0f,			//���a
		200.0f,			//����
		m_position      //���W
	);

	//���̃{�[����ID���擾
	m_swordBoneId = m_modelRender.FindBoneID(L"sword");

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
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
	//�S������
	Electric();
	//�U������
	Attack();
	//�ˌ�����
	Assult();
	//�����蔻��
	Collision();
	//��]����
	Rotation();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ���
	ManageState();
	
	//���f���̍X�V
	m_modelRender.Update();
}

void Enemy::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ�������
	if (m_enemyState != enEnemyState_Attack)
	{
		//�������Ȃ�
		return;
	}
	//�t���O��true�Ȃ�
	if (m_isUnderAttack == true)
	{
		//�U���̓����蔻����쐬����
		MakeAttackCollision();
	}
}

void Enemy::Electric()
{
	//�S���X�e�[�g�ł͂Ȃ�������
	if (m_enemyState != enEnemyState_ElectricShock)
	{
		//�������Ȃ�
		return;
	}
	//�S���^�C�}�[�����炷
	electrictimer -= g_gameTime->GetFrameDeltaTime();
}

void Enemy::MakeAttackCollision()
{
	//�����蔻����쐬����
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//���W�������O�ɂ���
	collisionPosition += m_forward * 50.0f;
	//����̓����蔻����쐬����
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(20.0f,190.0f, 20.0f));
	//���O��t����
	collisionObject->SetName("enemy_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void Enemy::MakeDamageEffect()
{
	//�G�t�F�N�g���쐬����
	EffectEmitter* m_effectEmitter;
	m_effectEmitter = NewGO<EffectEmitter>(0);
	Vector3 EffectPosition = m_position;
	Vector3 EffectScale = m_scale;
	m_effectEmitter->SetScale(Vector3(0.5f, 1.0f, 1.0f));
	//���W����ɂ���
	EffectPosition.y += 180.0f;
	m_effectEmitter->SetPosition(EffectPosition);
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Init(11);
	//�G�t�F�N�g���Đ�����
	m_effectEmitter->Play();
}


void Enemy::Collision()
{
	//��_���[�W�X�e�[�g���_�E���X�e�[�g��������
	if (m_enemyState == enEnemyState_ReceiveDamage ||
		m_enemyState == enEnemyState_Down)
	{
		//�������Ȃ�
		return;
	}

	//�X�L���̓����蔻��
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("item_thunder");
	for (auto collision : collisions3)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//�S���X�e�[�g��
			m_enemyState = enEnemyState_ElectricShock;
		}
	}

	//�v���C���[�̋ߐڍU���̓����蔻��
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//SE���Đ�����
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(32);
			m_se->SetVolume(0.3f);
			m_se->Play(false);

			//Hp�����炷
			m_hp -= 1;

			//��_���[�W�G�t�F�N�g���Đ�����
			MakeDamageEffect();

			//�v���C���[��Mp��40��菬����������
			if (m_player->m_mp < 40)
			{
				//Mp�𑝂₷
				m_player->m_mp += 5;
			}
			//Hp��0��菬����������
			if (m_hp <= 0)
			{
				//�_�E���X�e�[�g��
				m_enemyState = enEnemyState_Down;
			}
			else
			{
				//��_���[�W�X�e�[�g��
				m_enemyState = enEnemyState_ReceiveDamage;
			}
		}
	}

	//�v���C���[�̖��@�U���̓����蔻��
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_magic");
	for (auto collision : collisions2)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//SE���Đ�����
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(32);
			m_se->SetVolume(0.3f);
			m_se->Play(false);

			//Hp�����炷
			m_hp -= 1;

			//��_���[�W�G�t�F�N�g���Đ�����
			MakeDamageEffect();

			//HP��0�ɂȂ�����B
			if (m_hp == 0)
			{
				//�_�E���X�e�[�g��
				m_enemyState = enEnemyState_Down;
			}
			//0�ł͂Ȃ�������
			else
			{
				//��_���[�W�X�e�[�g��
				m_enemyState = enEnemyState_ReceiveDamage;
			}
		}
	}
}


void Enemy::Assult()
{
	//�ˌ��X�e�[�g�ł͂Ȃ�������
	if (m_enemyState != enEnemyState_Assult)
	{
		//�������Ȃ�
		return;
	}

	//�G�l�~�[���^���[�֓ˌ�����x�N�g��
	Vector3 modelPosition = m_position;
	Vector3 diff_tower = m_tower->GetPosition() - m_position;
	diff_tower.Normalize();

	//�ړ����x
	m_moveSpeed.x = diff_tower.x * 300.0f;
	m_moveSpeed.z = diff_tower.z * 300.0f;
	
	//�d��
	m_moveSpeed.y -= 10000.0f * g_gameTime->GetFrameDeltaTime();

	//���W�̍X�V
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
	//�v���C���[��������
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
	//�v���C���[�������ĂȂ�
	return false;
}

const bool Enemy::IsCanTowerAttack() const
{
	//�^���[���U���ł���
	Vector3 diff = m_tower->GetPosition() - m_position;
	if (diff.LengthSq() <= 500.0f * 500.0f)
	{
		return true;
	}
	//�^���[���U���ł��Ȃ�
	return false;
}


const bool Enemy::IsCanPlayerAttack() const
{
	//�v���C���[���U���ł���
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 400.0f * 400.0f)
	{
		return true;
	}
	//�v���C���[���U���ł��Ȃ�
	return false;
}


void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case enEnemyState_Idle:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(5.5f);
		//�⊮����
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
		//�ˌ��X�e�[�g�̎�
	case enEnemyState_Assult:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�⊮����
		m_modelRender.PlayAnimation(enAnimationClip_Assult, 0.1f);
		break;
		//�U���X�e�[�g�̎�
	case enEnemyState_Attack:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.4f);
		//�⊮����
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//��_���[�W�X�e�[�g�̎�
	case enEnemyState_ReceiveDamage:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.7f);
		//�⊮����
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
		//�S���X�e�[�g�̎�
	case enEnemyState_ElectricShock:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(0.5f);
		//�⊮����
		m_modelRender.PlayAnimation(enAnimationClip_ElectricShock, 0.1f);
		break;
		//�_�E���X�e�[�g�̎�
	case enEnemyState_Down:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.7f);
		//�⊮����
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
		//�ҋ@�X�e�[�g�̎�
	case enEnemyState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessIdleStateTransition();
		break;
		//�ˌ��X�e�[�g�̎�
	case enEnemyState_Assult:
		//�ˌ��X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessAssultStateTransition();
		break;
		//�U���X�e�[�g�̎�
	case enEnemyState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎�
	case enEnemyState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessReceiveDamageTransition();
		break;
		//�S���X�e�[�g�̎�
	case enEnemyState_ElectricShock:
		//�S���X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessElectricShockTransition();
		break;
		//�_�E���X�e�[�g�̎�
	case enEnemyState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessDownTransition();
		break;
	default:
		break;
	}
}

void Enemy::ProcessCommonStateTransition()
{   
	//�v���C���[�֌������x�N�g��
	Vector3 diff_player = m_player->GetPosition() - m_position;
	//�^���[�֌������x�N�g��
	Vector3 diff_tower = m_tower->GetPosition() - m_position;

	//�^���[���U���ł���Ȃ�
	if (IsCanTowerAttack() == true)
	{
		//�v���C���[���U���ł���Ȃ�
		if (IsCanPlayerAttack() == true)
		{
			//�v���C���[�֌������x�N�g��
			diff_player.Normalize();
			m_moveSpeed = diff_player * 300.0f;
			int ram = rand() % 100;
			if (ram > 50)
			{
				//�U���X�e�[�g��
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//�ҋ@�X�e�[�g��
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}

		else {
			//�^���[�֌������x�N�g��
			diff_tower.Normalize();
			m_moveSpeed = diff_tower * 300.0f;
			ram = rand() % 100;
			if (ram > 50)
			{
				//�U���X�e�[�g��
				m_enemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//���@�U���X�e�[�g��
				m_enemyState = enEnemyState_Idle;
				return;
			}
		}
	}
	//�v���C���[���U���ł���Ȃ�
	else if (IsCanPlayerAttack() == true)
	{
		//�v���C���[�Ɍ������x�N�g��
		diff_player.Normalize();
		m_moveSpeed = diff_player * 300.0f;
		int ram = rand() % 100;
		if (ram > 30)
		{
			//�U���X�e�[�g��
			m_enemyState = enEnemyState_Attack;
			m_isUnderAttack = false;
			return;
		}
		else
		{
			//�ҋ@�X�e�[�g��
			m_enemyState = enEnemyState_Idle;
			return;
		}
	}
	//�v���C���[���^���[���U���ł��Ȃ��Ȃ�
	else
	{
		//�ˌ��X�e�[�g��
		m_enemyState = enEnemyState_Assult;
		return;
	}
}

void Enemy::ProcessIdleStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{ 	
		//���̃X�e�[�g�ɑJ�ڂ���
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
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{ 		
		//���̃X�e�[�g�ɑJ�ڂ���
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
		//���̃X�e�[�g�ɑJ�ڂ���	
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
