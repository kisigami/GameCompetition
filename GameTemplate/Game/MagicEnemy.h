#pragma once

class Player;
class Tower;
class IceBall;
class Game;


class MagicEnemy: public IGameObject
{
public:
	//�G�l�~�[�X�e�[�g
	enum EnMagicEnemyState {
		enMagicEnemyState_Idle,
		enMagicEnemyState_Assult,        //�^���[�ˌ�
		enMagicEnemyState_Attack,        //�U��
		enMagicEnemyState_ReceiveDamage,
		enMagicEnemyState_ElectricShock,
		enMagicEnemyState_Down,
	};
public:
	MagicEnemy();
	~MagicEnemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//���W��ݒ�
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//���W���擾
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	//�傫����ݒ�
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	//��]��ݒ�
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	//HP��ݒ�
	void SetHP(const int hp)
	{
		m_hp = hp;
	}

	void UpdateInstansingData(
		const Vector3& pos,
		const Quaternion& rot, const Vector3& scale
	) {
		m_modelRender.UpdateInstancingData(pos, rot, scale);
	}
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	
	//void ItemThunderEffect();
	//���ւ̈ړ�����
	void Assult();
	//��]����
	void Rotation();
	//�U������
	void MakeMagicBall();
	void MakeDamageEffect();
	void Electric();
	//�v���C���[�Ƃ̓����蔻�菈��
	void Collision();
	//�A�j���[�V�����̍Đ�
	void PlayAnimation();
	//�e�X�e�[�g�̑J�ڏ���
	void ManageState();
	//���ʂ̃X�e�[�g�J�ڏ���
	void ProcessCommonStateTransition();
	//�ҋ@�X�e�[�g�̑J�ڏ���
	void ProcessIdleStateTransition();
	//�ˌ��X�e�[�g�̑J�ڏ���
	void ProcessAssultStateTransition();
	//�U���X�e�[�g�̑J�ڏ���
	void ProcessAttackStateTransition();
	//��_���[�W�X�e�[�g�̑J�ڏ���
	void ProcessReceiveDamageTransition();
	void ProcessElectricShockStateTransition();
	//�_�E���X�e�[�g�̑J�ڏ���
	void ProcessDownTransition();
	//�v���C���[�𔭌�������
	const bool SearchPlayer() const;
	//�v���C���[���U���ł��邩
	const bool IsCanPlayerAttack() const;
	//�����U���ł��邩
	const bool IsCanTowerAttack() const;


	enum EnAnimationClip {    	//�A�j���[�V�����N���b�v
		enAnimationClip_Idle,  //�ҋ@�A�j���[�V����
		enAnimationClip_Assult, //�ˌ��A�j���[�V����
		enAnimationClip_Attack, //�U���A�j���[�V����
		enAnimationClip_ReceiveDamage,
		enAnimationClip_ElectricShock,
		enAnimationClip_Down,
		enAnimationClip_Num,	//�A�j���[�V�����̐�
	};
	//�ړ��ł��邩
	bool IsEnableMove() const
	{
		//�U�����͈ړ����Ȃ�
		return
			m_enemyState != enMagicEnemyState_Attack &&
			m_enemyState != enMagicEnemyState_ReceiveDamage &&
			m_enemyState != enMagicEnemyState_ElectricShock &&
			m_enemyState != enMagicEnemyState_Down;
	}
	AnimationClip m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v
	ModelRender m_modelRender;
	FontRender m_fontRender;
	CharacterController	m_charaCon;									//�L�����R��
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Vector3	m_scale = Vector3::One;
	Vector3 m_forward = Vector3::AxisZ;
	Quaternion m_rotation;
	bool m_isUnderAttack = false;
	EnMagicEnemyState m_enemyState = enMagicEnemyState_Assult;
	Tower* m_tower = nullptr;
	Player* m_player = nullptr;
	int    m_hp = 2;
	float electrictimer = 9.0f;
	int m_MagicEnemynum = 3;
	Game* m_game = nullptr;
};

