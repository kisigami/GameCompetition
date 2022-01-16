#include "stdafx.h"
#include "PlayerAttackEnemy.h"

#include "Game.h"
#include "Player.h"

#include "collision/CollisionObject.h"

namespace
{
	const float GRAVITY = 6000.0f;               //�d��
	const float MOVE_SPEED = 300.0f;             //�ړ����x
	const float PLAYER_ATTACK_RANGE = 500.0f;    //�v���C���[���U���ł��鋗��
	const float RESTRAINT_TIME = 8.0f;           //�S������
}

PlayerAttackEnemy::PlayerAttackEnemy()
{

}

PlayerAttackEnemy::~PlayerAttackEnemy()
{

}																																																																																																										

bool PlayerAttackEnemy::Start()
{
	//�A�j���[�V�����̓ǂݍ���
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/playerattackenemy/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Assault].Load("Assets/animData/playerattackenemy/run.tka");
	m_animationClips[enAnimationClip_Assault].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/playerattackenemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveDamage].Load("Assets/animData/playerattackenemy/damage.tka");
	m_animationClips[enAnimationClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/playerattackenemy/down.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_ReceiveRestraint].Load("Assets/animData/playerattackenemy/electric.tka");
	m_animationClips[enAnimationClip_ReceiveRestraint].SetLoopFlag(false);

	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/enemy/playerattackenemy/playerattackenemy.tkm",
		m_animationClips,
		enAnimationClip_Num,
		enModelUpAxisZ
	);

	//���W�A��]�A�傫����ݒ�
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

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

	//SE���Đ�����
	g_soundEngine->ResistWaveFileBank(32, "Assets/sound/enemydamage.wav");

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	return true;
}

void PlayerAttackEnemy::Update()
{
	//�ړ�����
	Move();
	//��]����
	Rotation();
	//�U������
	Attack();
	//�����蔻��
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

void PlayerAttackEnemy::Move()
{
	//�ˌ��X�e�[�g�ł͂Ȃ�������
	if (m_playerattackenemyState != enPlayerAttackEnemyState_Assault)
	{
		//�������Ȃ�
		return;
	}

	//�d��
	m_moveSpeed.y -= 10000.0f * g_gameTime->GetFrameDeltaTime();

	//�v���C���[�ւ̓ˌ�����
	Vector3 diff = m_player->GetPosition() - m_position;
	diff.Normalize();
	//�ړ����x
	m_moveSpeed.x = diff.x * 300.0f;
	m_moveSpeed.z = diff.z * 300.0f;

	//���W�̍X�V
	m_modelRender.SetPosition(m_position);
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void PlayerAttackEnemy::Rotation()
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

void PlayerAttackEnemy::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ�������
	if (m_playerattackenemyState != enPlayerAttackEnemyState_Attack)
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

void PlayerAttackEnemy::MakeAttackCollision()
{
	//�����蔻����쐬����
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	//���W�������O�ɂ���
	collisionPosition += m_forward * 50.0f;
	//����̓����蔻����쐬����
	collisionObject->CreateBox(collisionPosition,
		Quaternion::Identity,
		Vector3(170.0f, 20.0f, 20.0f));
	//�R���W�����ɖ��O��t����
	collisionObject->SetName("enemy_attack");
	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
}

void PlayerAttackEnemy::Collision()
{
	//��_���[�W�X�e�[�g���_�E���X�e�[�g��������
	if (m_playerattackenemyState == enPlayerAttackEnemyState_ReceiveDamage ||
		m_playerattackenemyState == enPlayerAttackEnemyState_Down)
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
			//�S���X�e�[�g��
			m_playerattackenemyState = enPlayerAttackEnemyState_ReceiveRestraint;
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
				//�v���C���[��Mp��5���₷
				m_player->m_mp += 5;
			}
			//Hp��0��������
			if (m_hp == 0)
			{
				//�_�E���X�e�[�g��
				m_playerattackenemyState = enPlayerAttackEnemyState_Down;
			}
			//Hp��0�ł͂Ȃ�������
			else
			{
				//��_���[�W�X�e�[�g��
				m_playerattackenemyState = enPlayerAttackEnemyState_ReceiveDamage;
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
			//Hp��0��������
			if (m_hp == 0)
			{
				//�_�E���X�e�[�g��
				m_playerattackenemyState = enPlayerAttackEnemyState_Down;
			}
			//Hp��0�ł͂Ȃ�������
			else 
			{
				//��_���[�W�X�e�[�g��
				m_playerattackenemyState = enPlayerAttackEnemyState_ReceiveDamage;
			}
		}
	}
}

void PlayerAttackEnemy::Restraint()
{
	//�S���X�e�[�g�ł͂Ȃ�������
	if (m_playerattackenemyState != enPlayerAttackEnemyState_ReceiveRestraint)
	{
		//�������Ȃ�
		return;
	}
	//�S���^�C�}�[�����炷
	electrictimer -= g_gameTime->GetFrameDeltaTime();
}

void PlayerAttackEnemy::MakeDamageEffect()
{
	//�G�t�F�N�g���쐬����
	EffectEmitter* m_effectEmitter;
	m_effectEmitter = NewGO<EffectEmitter>(0);
	Vector3 EffectPosition = m_position;
	Vector3 EffectScale = m_scale;
	//���W��������ɂ���
	EffectPosition.y += 180.0f;
	m_effectEmitter->SetPosition(EffectPosition);
	//�傫����ݒ肷��
	m_effectEmitter->SetScale(Vector3(0.5f, 1.0f, 1.0f));
	//�G�t�F�N�g���Đ�����
	m_effectEmitter->Init(11);
	m_effectEmitter->Play();
}

void PlayerAttackEnemy::PlayAnimation()
{
	switch (m_playerattackenemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case enPlayerAttackEnemyState_Idle:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�ҋ@�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.1f);
		break;
		//�ˌ��X�e�[�g�̎�
	case enPlayerAttackEnemyState_Assault:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�ˌ��A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Assault, 0.1f);
		break;
		//�U���X�e�[�g�̎�
	case enPlayerAttackEnemyState_Attack:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�U���A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.4f);
		break;
		//��_���[�W�X�e�[�g�̎�
	case enPlayerAttackEnemyState_ReceiveDamage:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.5f);
		//��_���[�W�A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveDamage, 0.1f);
		break;
		//�S���X�e�[�g�̎�
	case enPlayerAttackEnemyState_ReceiveRestraint:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(0.5f);
		//�S���A�j���[�V�������Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_ReceiveRestraint, 0.1f);
		break;
		//�_�E���X�e�[�g�̎�
	case enPlayerAttackEnemyState_Down:
		//�Đ����x
		m_modelRender.SetAnimationSpeed(1.0f);
		//�_�E���X�e�[�g���Đ�����
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void PlayerAttackEnemy::ManageState()
{
	switch (m_playerattackenemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case enPlayerAttackEnemyState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessIdleStateTransition();
		break;
		//�ˌ��X�e�[�g�̎�
	case enPlayerAttackEnemyState_Assault:
		//�ˌ��X�e�[�g�̃X�e�[�g�J�ڏ���
		ProcessAssultStateTransition();
		break;
	case enPlayerAttackEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	case enPlayerAttackEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
	case enPlayerAttackEnemyState_ReceiveRestraint:
		ProcessReceiveRestraintStateTransition();
		break;
	case enPlayerAttackEnemyState_Down:
		ProcessDownStateTransition();
		break;
	default:
		break;
	}
}

void PlayerAttackEnemy::ProcessCommonStateTransition()
{
	//�G�l�~�[�̓v���C���[�ւ̍U����D�悷��
	Vector3 diff_player = m_player->GetPosition() - m_position;
	//Vector3 diff_tower = m_tower->GetPosition() - m_position;

	//�v���C���[�ւ̍U�����ł��Ȃ�������������
	if (IsCanPlayerAttack() == false)
	{
		//�v���C���[��ǂ�������
		m_playerattackenemyState = enPlayerAttackEnemyState_Assault;
		return;
	}
	//�U���ł��鋗���Ȃ�
	else
	{
		//�v���C���[������
		diff_player.Normalize();
		m_moveSpeed = diff_player * 300.0f;
		int ram = rand() % 100;
		if (ram > 80)
		{
			//�U������
			m_playerattackenemyState = enPlayerAttackEnemyState_Attack;
			m_isUnderAttack = false;
			return;
		}
		else
		{
			//�ҋ@����
			m_playerattackenemyState = enPlayerAttackEnemyState_Idle;
			return;
		}
	}
}

void PlayerAttackEnemy::ProcessReceiveRestraintStateTransition()
{
	if (electrictimer <= 0.0f)
	{
		ProcessCommonStateTransition();
		electrictimer = 9.0f;
		return;
	}
}

void PlayerAttackEnemy::ProcessIdleStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	//if (m_modelRender.IsPlayingAnimation() == false)
	//{ 		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
		return;
	//}
}

void PlayerAttackEnemy::ProcessAssultStateTransition()
{
	//�U���͈͂ɓ��A�v���C���[������Ȃ�
	//if (IsCanPlayerAttack() == true || IsCanTowerAttack() == true)
	//{
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
		return;
	//}
}

void PlayerAttackEnemy::ProcessAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
		return;
	}
}

void PlayerAttackEnemy::ProcessReceiveDamageStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (electrictimer < 9.0f)
		{
			m_playerattackenemyState = enPlayerAttackEnemyState_ReceiveRestraint;
			return;
		}
		else {
			ProcessCommonStateTransition();
			return;
		}
	}
}

void PlayerAttackEnemy::ProcessDownStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���
		DeleteGO(this);
		m_game->m_DeadEnemynum++;
		return;
	}
}

void PlayerAttackEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

const bool PlayerAttackEnemy::IsCanPlayerAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	if (diff.LengthSq() <= 300.0f * 300.0f)
	{
		return true;
	}
	return false;
}

void PlayerAttackEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);

}
