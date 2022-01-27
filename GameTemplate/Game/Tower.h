#pragma once

//�N���X�錾
class Game;         //�Q�[��
class GameOver;     //�Q�[���I�[�o�[

/// <summary>
/// �^���[
/// </summary>
class Tower : public IGameObject
{
public:
	/// <summary>
	/// �^���[�X�e�[�g
	/// </summary>
	enum EnTowerState {
		enTowerState_Idle,
		enTowerState_DamageInvalid,
	};
public:
	/// <summary>
	/// �^���[�j��X�e�[�g
	/// </summary>
	enum EnBreakState {
		enBreakState_NoBreak,
		enBreakState_HelfBreak,
		enBreakState_AllBreak,
	};
public:
	Tower();
	~Tower();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// �傫����ݒ�
	/// </summary>
	/// <param name="scale">�傫��</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	///�ϋv�l��ݒ�
	/// </summary>
	/// <param name="durability">�ϋv�l</param>
	void SetDurability(const float& durability)
	{
		m_durability = durability;
	}
	/// <summary>
	/// �ϋv�l���擾
	/// </summary>
	/// <returns>�ϋv�l</returns>
	const float& GetDurability() const
	{
		return m_durability;
	}
private:
	/// <summary>
	/// �G�̍U���̓����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	/// �_���[�W��������
	/// </summary>
	void DamageInvalid();
	/// <summary>
	/// �j��G�t�F�N�g
	/// </summary>
	void BreakEffect();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();

	CollisionObject m_collisionObject;
	SpriteRender m_spriteRender;
	FontRender m_fontRender;
	FontRender m_fontRender2;
	ModelRender m_modelRender;
	ModelRender m_modelRender2;
	ModelRender m_modelRender3;
	Vector3 m_position;
	Vector3 m_scale;
	Vector3 m_moveSpeed;
	Quaternion m_rotation;
	PhysicsStaticObject m_physicsStaticObject;
	CharacterController m_charaCon;
	Game* m_game = nullptr;
	GameOver* m_gameOver;
	bool m_towerBreakFrag = false;
	float damageinvalidtimer = 1.0f;
	EnTowerState m_towerState = enTowerState_Idle;
	EffectEmitter* effectEmitter;
	EnBreakState m_BreakState = enBreakState_NoBreak;
	float m_durability = 200.0f;
};

