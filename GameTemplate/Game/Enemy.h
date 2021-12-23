#pragma once

//�N���X�錾
class Player;
class Tower;
class Game;
class GamePoint;

//�G�l�~�[
class Enemy:public IGameObject
{
public:
	//�G�l�~�[�X�e�[�g
	enum EnEnemyState {
		enEnemyState_Idle,
		enEnemyState_Assult,        //�^���[�ˌ�
		enEnemyState_Attack,        //�U��
		enEnemyState_ReceiveDamage,
		enEnemyState_ElectricShock,
		enEnemyState_Down,
	};
public:
	Enemy();
	~Enemy();
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

	//���ւ̈ړ�����
	void Assult();
	void Electric();
	//��]����
	void Rotation();
	//�U������
	void Attack();
	//�v���C���[�Ƃ̓����蔻�菈��
	void Collision();
	//�U���p�̓����蔻����쐬����
	void MakeAttackCollision();
	void MakeDamageEffect();
	//�A�j���[�V�����C�x���g�p�̊֐�
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
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
	//�_�E���X�e�[�g�̑J�ڏ���
	void ProcessDownTransition();

	void ProcessElectricShockTransition();

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
		return
			m_enemyState != enEnemyState_Attack &&
			m_enemyState != enEnemyState_ReceiveDamage &&
			m_enemyState != enEnemyState_ElectricShock &&
			m_enemyState != enEnemyState_Down;
	}

	AnimationClip m_animationClips[enAnimationClip_Num];
	ModelRender m_modelRender;
	FontRender m_timerFont;
	CharacterController	m_charaCon;							
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Vector3	m_scale = Vector3::One;
	Vector3 m_forward = Vector3::AxisZ;
	Quaternion m_rotation;
	bool m_isUnderAttack = false;
	EnEnemyState m_enemyState = enEnemyState_Assult;
	Tower* m_tower = nullptr;
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	int m_swordBoneId = -1;
	int    m_hp =3;
	int ram; 
	float electrictimer = 9.0f;
	EffectEmitter* effectEmitter;
	int m_enemynum = 6;
	Game* m_game = nullptr;
};

