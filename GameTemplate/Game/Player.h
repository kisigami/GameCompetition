#pragma once

//�N���X�錾
class Game;
class Enemy;
class IceBall;
class ExplosionBall;
class Tower;

//�v���C���[
class Player : public IGameObject
{
public:
	//�v���C���[�̃X�e�[�g
	enum EnPlayerState {
		enPlayerState_Idle,
		enPlayerState_Run,
		enPlayerState_UseItem,
		enPlayerState_FirstAttack,
		enPlayerState_SecondAttack,
		enPlayerState_ThirdAttack,
		enPlayerState_Cheak,
		enPlayerState_Magic,
		enPlayerState_ReceiveDamage,
		enPlayerState_Win,
		enPlayerState_Down,
	};
	//�A�C�e���̃X�e�[�g
	enum EnEquipState {
		enEquipState_No,
		enEquipState_Heal,
		enEquipState_Thuner,
	};
	//���@�U���X�e�[�g
	enum EnUseMagic {
		enUseMagic_NormalMagic,
		enUseMagic_PowerMagic
	};
public:
	Player();
	~Player();
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
	
	//�傫��
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	
	//��]��ݒ�
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//�������Ԃ��ǂ���
	bool IsEnableMove() const
	{
		return
			m_playerState != enPlayerState_FirstAttack &&
			m_playerState != enPlayerState_SecondAttack &&
			m_playerState != enPlayerState_ThirdAttack &&
			m_playerState != enPlayerState_UseItem &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_Win &&
			m_playerState != enPlayerState_Magic;
	}
//private:/ 	
	void  Hit();
	//�ړ�����
	void Move();
	//��]����
	void Rotation();
	//�����蔻��
	void Collision();
	//�U�����̏���
	void Attack();
	//�U���p�̃R���W����
	void MakeAttackCollision();
	//�U��1�̃G�t�F�N�g
	void MakeFirstSlashingEffect();
	//�U��2�̃G�t�F�N�g
	void MakeSecondSlashingEffect();
	//�U��3�̃G�t�F�N�g
	void MakeThirdSlashingEffect();
	//���@�U�����쐬����
	void MakeMagicBall();
	void MakeExplosionBall();
	//���~�߃A�C�e���i���ʔ͈́j�̃G�t�F�N�g
	void MakeRangeEffect();
	//���~�߃A�C�e���i�g�p���j�̃G�t�F�N�g
	void MakeEnemyStopEffect();
	//���~�߃A�C�e���̃R���W����
	void MakeEnemyStopCollision();
	//�A�C�e���I��
	void ChoiseItem();


	//�A�j���[�V�����̍Đ�
	void PlayAnimation();
	//�e�X�e�[�g�̑J�ڏ���
	void ManageState();
	//�A�j���[�V�������O�C�x���g
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//���ʂ̃X�e�[�g�J�ڏ���
	void ProcessCommonStateTransition();
	//�ҋ@�X�e�[�g�̑J�ڏ���
	void ProcessIdleStateTransition();
	//����X�e�[�g�̑J�ڏ���
	void ProcessRunStateTransition();
	//��_���[�W�X�e�[�g�̑J�ڏ���
	void ProcessDamageStateTransition();
	//1���ڂ̍U���X�e�[�g�̑J�ڏ���
	void ProcessAttackStateTransition();
	//2���ڂ̍U���X�e�[�g�̑J�ڏ���
	void ProcessAttack2StateTransition();
	//3���ڂ̍U���X�e�[�g�̑J�ڏ���
	void ProcessAttack3StateTransition();
	//���@�U���X�e�[�g�̑J�ڏ���
	void ProcessMagicStateTransition();
	//�A�C�e���g�p�X�e�[�g�̑J�ڏ���
	void ProcessUseItemStateTransition();

	void ProcessWinStateTransition();
	void ProcessDownStateTransition();

	//�A�j���[�V�����N���b�v
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Damage,
		enAnimationClip_FirstAttack,
		enAnimationClip_SecondAttack,
		enAnimationClip_ThirdAttack,
		enAnimationClip_Magic,
		enAnimationClip_UseItem,
		enAnimationClip_Win,
		enAnimationClip_Down,
		enAnimationClip_Num
	};
	
	SpriteRender PlayerUseMagic;
	SpriteRender PlayerSubMagic;
	SpriteRender m_itemWaku;
	SpriteRender m_itemStop;
	SpriteRender m_itemHeal;
	ModelRender          m_modelRender;
	SpriteRender         m_sprintRender;
	SpriteRender         m_sprintRender2;
	SpriteRender         m_sprintRender3;
	FontRender           m_fontRender;
	FontRender           m_fontRender2;
	FontRender           m_fontRender3;  
	AnimationClip			m_animationClips[enAnimationClip_Num];//�A�j���[�V�����N���b�v//���f�������_�[
	Vector3              m_position;       
	Vector3              m_scale = Vector3::One;                         //�傫��
	Vector3              m_moveSpeed;                          //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;           //�O//��]
	Quaternion           m_rotation;
	CharacterController  m_charaCon;                           //�L�����N�^�[�R���g���[���[
	EnPlayerState        m_playerState = enPlayerState_Idle;   //�X�e�[�g
	EnEquipState         m_equipState = enEquipState_Heal;
	//�g�����@�U��
	EnUseMagic           m_UseMagic = enUseMagic_NormalMagic;
	bool                 m_isUnderAttack = false;
	int                  m_swordBoneId = -1;
	float                m_hp = 100;
	float               m_mp = 40;
	//0=�ꌂ�ځ@1=2����  2=3����
	int Type = 0;
	//0=�^�C�}�[��~�@1=�^�C�}�[�N��
	int move = 0;
	int move2 = 0;
	//1���ڂ���̌o�ߎ���
	float m_attackTimer = 0.0f;
	//2���ڂ���̌o�ߎ���
	float m_attackTimer2 = 0.0f;
	float timer = 0.0f;
	
	float battleStateTimer = 0.0f;

	int Thundernum = 5;
	int Healnum = 5;
	float m_alpha = 0.0f;
	float m_alpha2 = 0.0f;
	float m_alpha3 = 0.0f;
	EffectEmitter* effectEmitter;
	int a = 0;
	int b = 0;

	Enemy* m_enemy;
	Game* m_game;
	Tower* m_tower;

	int HavePoint = 0;
};
