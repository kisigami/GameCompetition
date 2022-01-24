#pragma once

//�N���X�錾
class Tower;

/// <summary>
/// �^���[UI
/// </summary>
class TowerUi:public IGameObject
{
public:
	TowerUi();
	~TowerUi();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// �ϋv�l�̑傫��
	/// </summary>
	void DurabilityScale();

	SpriteRender m_towerIcon;                 //�^���[�A�C�R��
	SpriteRender m_durabilityFrame;           //�ϋv�l�̘g�̉摜
	SpriteRender m_durability;                //�ϋv�l�̉摜
	SpriteRender m_durabilityButtom;          //�ϋv�l�̒�̉摜
	SpriteRender m_durabilityFont;            //�ϋv�l�̕����̉摜
	float        m_durabilityScale = 1.0f;    //�ϋv�l�̉摜�̑傫��
	Tower* m_tower = nullptr;                 //�^���[
};

