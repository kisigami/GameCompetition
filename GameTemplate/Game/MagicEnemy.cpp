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
	const float GRAVITY = 6000.0f;               //�d��
	const float MOVE_SPEED = 300.0f;             //�ړ����x
	const float TOWER_ATTACK_RANGE = 1000.0f;     //�^���[���U���ł��鋗��
	const float PLAYER_ATTACK_RANGE = 500.0f;    //�v���C���[���U���ł��鋗��
	const float RESTRAINT_TIME = 8.0f;           //�S������
}

MagicEnemy::MagicEnemy()
{

}

MagicEnemy::~MagicEnemy()
{

}

bool MagicEnemy::Start()
{
	//�A�j���[�V�����̓ǂݍ���
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

	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/enemy/magic/magicenemy.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);
	//���W�A��]�A�傫���̍X�V
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	//�L�����N�^�[�R���g���[���[
	m_charaCon.Init(
		70.0f,			//���a
		200.0f,			//����
		m_position      //���W
	);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//SE�̓ǂݍ���
	g_soundEngine->ResistWaveFileBank(32, "Assets/sound/enemydamage.wav");

	m_player = FindGO<Player>("player");
	m_tower = FindGO<Tower>("tower");
	m_game = FindGO<Game>("game");
	return true;
}

void MagicEnemy::Update()
{
	//�ړ�����
	Move();
	//��]����
	Rotation();
	//�����蔻��̏���
	Collision();
	//�S������
	Restraint();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ���
	ManageState();

	//���f���̍X�V
	m_modelRender.Update();
}

void MagicEnemy::Move()
{
	//�ˌ��X�e�[�g�ł͂Ȃ�������
	if (m_magicenemyState != enMagicEnemyState_Assault)
	{
		//�������Ȃ�
		return;
	}

	//�d��
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();

	//�^���[�ւ̈ړ�����
	Vector3 diff = m_tower->GetPosition() - m_position;
	diff.Normalize();
	//�ړ����x
	m_moveSpeed.x = diff.x * MOVE_SPEED;
	m_moveSpeed.z = diff.z * MOVE_SPEED;

	//���W�̍X�V
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
	//���@�U�����쐬����
	MagicBall* magicBall = NewGO<MagicBall>(0);
	Vector3 magicBallPosition = m_position;
	//���W��������ɂ���
	magicBallPosition.y += 55.0f;
	//���W�������O�ɂ���
	magicBallPosition += m_forward * 150.0f;
	magicBall->SetPosition(magicBallPosition);
	magicBall->SetRotation(m_rotation);
	//�g�p�҂��}�W�b�N�G�l�~�[�ɂ���
	magicBall->SetEnUser(MagicBall::enUser_Enemy);
}

void MagicEnemy::Collision()
{
	//��_���[�W�X�e�[�g�X�e�[�g���_�E���X�e�[�g��������
	if (m_magicenemyState == enMagicEnemyState_ReceiveDamage || m_magicenemyState == enMagicEnemyState_Down)
	{
		//�������Ȃ�
		return;
	}

	//�v���C���[�̍S�����@�̓����蔻��
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("item_thunder");
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			m_magicenemyState = enMagicEnemyState_ReceiveRestraint;
		}
	}

	//�v���C���[�̋ߐڍU���̓����蔻��
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack");
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
			//Hp��1���炷
			m_hp -= 1;
			//��_���[�W�G�t�F�N�g���Đ�����
			MakeDamageEffect();
			//�v���C���[��Mp��40�ȉ���������
			if (m_player->m_mp < 40)
			{
				//�v���C���[��Mp��10���₷
				m_player->m_mp += 10;
			}

			//Hp��0�ɂȂ�����
			if (m_hp <= 0)
			{
				//�_�E���X�e�[�g��
				m_magicenemyState = enMagicEnemyState_Down;
			}
			//Hp��0�ł͂Ȃ�������
			else
			{
				//��_���[�W�X�e�[�g��
				m_magicenemyState = enMagicEnemyState_ReceiveDamage;
			}
		}
	}

	//�v���C���[�̖��@�U���̓����蔻��
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("player_magic");
	for (auto collision : collisions3)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//SE���Đ�����
			SoundSource* m_se = NewGO<SoundSource>(0);
			m_se->Init(32);
			m_se->SetVolume(0.3f);
			m_se->Play(false);
			//Hp��1���炷
			m_hp -= 1;
			//HP��0�ɂȂ�����
			if (m_hp == 0)
			{
				//�_�E���X�e�[�g��
				m_magicenemyState = enMagicEnemyState_Down;
			}
			//Hp��0�ł͂Ȃ�������
			else
			{
				//��_���[�W�X�e�[�g��
				m_magicenemyState = enMagicEnemyState_ReceiveDamage;
			}
		}
	}
}

void MagicEnemy::Restraint()
{
	//�S���X�e�[�g�ł͂Ȃ�������
	if (m_magicenemyState != enMagicEnemyState_ReceiveRestraint)
	{
		//�������Ȃ�
		return;
	}
	//�S���^�C�}�[�����炷
	m_restraintTimer -= g_gameTime->GetFrameDeltaTime();
}

void MagicEnemy::MakeDamageEffect()
{
	//�G�t�F�N�g���쐬����
	EffectEmitter* m_effectEmitter;
	m_effectEmitter = NewGO<EffectEmitter>(0);
	Vector3 m_EffectPosition = m_position;
	Vector3 m_EffectScale = m_scale;
	//���W��������ɂ���
	m_EffectPosition.y += 180.0f;
	m_effectEmitter->SetPosition(m_EffectPosition);
	//�傫����ݒ肷��
	m_effectEmitter->SetScale(Vector3(0.5f, 1.0f, 1.0f));
	//�G�t�F�N�g���Đ�����
	m_effectEmitter->Init(11);
	m_effectEmitter->Play();
}

void MagicEnemy::PlayAnimation()
{
	switch (m_magicenemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case enMagicEnemyState_Idle:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�ҋ@�A�j���[�V�����̍Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
		//�ˌ��X�e�[�g�̎�
	case enMagicEnemyState_Assault:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�ˌ��A�j���[�V�����̍Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Assault, 0.1f);
		break;
		//�U���X�e�[�g�̎�
	case enMagicEnemyState_Attack:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�U���A�j���[�V�����̍Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//��_���[�W�X�e�[�g�̎�
	case enMagicEnemyState_ReceiveDamage:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.7f);
		//��_���[�W�A�j���[�V�����̍Đ�
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
		//�S���X�e�[�g�̎�
	case enMagicEnemyState_ReceiveRestraint:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(0.5f);
		//�S���A�j���[�V�����̍Đ�
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveRestraint, 0.1f);
		break;
		//�_�E���X�e�[�g�̎�
	case enMagicEnemyState_Down:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�_�E���A�j���[�V�����̍Đ�
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
		//�ҋ@�X�e�[�g�̎�
	case enMagicEnemyState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessIdleStateTransition();
		break;
		//�ˌ��X�e�[�g�̎�
	case enMagicEnemyState_Assault:
		//�ˌ��X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessAssultStateTransition();
		break;
		//�U���X�e�[�g�̎�
	case enMagicEnemyState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎�
	case enMagicEnemyState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessReceiveDamageStateTransition();
		break;
		//�S���X�e�[�g�̎�
	case enMagicEnemyState_ReceiveRestraint:
		//�S���X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessReceiveRestraintStateTransition();
		break;
		//�_�E���X�e�[�g�̎�
	case enMagicEnemyState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessDownStateTransition();
		break;
	default:
		break;
	}
}

void MagicEnemy::ProcessCommonStateTransition()
{
	//�v���C���[�֌������x�N�g��
	Vector3 diff_player = m_player->GetPosition() - m_position;
	//�x�N�g���̐��K��
	diff_player.Normalize();

	//�v���C���[���U���ł���Ȃ�
	if (IsCanPlayerAttack() == true)
	{
		m_moveSpeed = diff_player * MOVE_SPEED;
		int ram = rand() % 100;
		if (ram > 30)
		{
			//�U���X�e�[�g��
			m_magicenemyState = enMagicEnemyState_Attack;
			return;
		}
		else
		{
			//�ҋ@�X�e�[�g��
			m_magicenemyState = enMagicEnemyState_Idle;
			return;
		}
	}

	if (IsCanTowerAttack() == true)
	{
		int ram = rand() % 100;
		if (ram > 30)
		{
			//�U���X�e�[�g��
			m_magicenemyState = enMagicEnemyState_Attack;
			return;
		}
		else
		{
			//�ҋ@�X�e�[�g��
			m_magicenemyState = enMagicEnemyState_Idle;
			return;
		}
	}

	//�U���ł��Ȃ��Ȃ�
	else
	{
		//�ˌ��X�e�[�g��
		m_magicenemyState = enMagicEnemyState_Assault;
		return;
	}
}

void MagicEnemy::ProcessIdleStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
	return;
}

void MagicEnemy::ProcessAssultStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
	return;
}

void MagicEnemy::ProcessAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
		return;
	}
}

void MagicEnemy::ProcessReceiveDamageStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�S���^�C�}�[�������Ă�����
		if (m_restraintTimer < RESTRAINT_TIME)
		{
			//�S���X�e�[�g��
			m_magicenemyState = enMagicEnemyState_ReceiveRestraint;
			return;
		}
		else
		{
			//���̃X�e�[�g�ɑJ�ڂ���
			ProcessCommonStateTransition();
			return;
		}
	}
}

void MagicEnemy::ProcessReceiveRestraintStateTransition()
{
	//�S���^�C�}�[��0.0f�ȉ���������
	if (m_restraintTimer <= 0.0f)
	{
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
		//�S���^�C�}�[��������
		m_restraintTimer = RESTRAINT_TIME;
		return;
	}
}

void MagicEnemy::ProcessDownStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�G���S���𑝂₷
		m_game->m_DeadEnemynum++;
		//���g���폜����
		DeleteGO(this);
		return;
	}
}

void MagicEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	if (wcscmp(eventName, L"magic_attack") == 0)
	{
		//���@�U�����쐬����
		MakeMagicBall();
	}
}

const bool MagicEnemy::IsCanTowerAttack() const
{
	//�^���[���U���ł���
	Vector3 diff = m_tower->GetPosition() - m_position;
	if (diff.LengthSq() <= TOWER_ATTACK_RANGE * TOWER_ATTACK_RANGE)
	{
		return true;
	}
	//�^���[���U���ł��Ȃ�
	return false;
}


const bool MagicEnemy::IsCanPlayerAttack() const
{
	//�v���C���[���U���ł���
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= PLAYER_ATTACK_RANGE * PLAYER_ATTACK_RANGE)
	{
		//�n�ʂɑ������Ă�����
		if (m_charaCon.IsOnGround() == true)
		{
			return true;
		}
	}
	//�v���C���[���U���ł��Ȃ�
	return false;
}

void MagicEnemy::Render(RenderContext& rc)
{
	//���f���̕`�揈��
	m_modelRender.Draw(rc);
}
