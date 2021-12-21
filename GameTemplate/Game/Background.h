#pragma once

class Background:public IGameObject
{
public:
	Background();
	~Background();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

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

private:
	ModelRender          m_modelRender;                        //���f�������_�[
	Vector3              m_position;                           //���W
	Vector3              m_scale;
	Quaternion           m_rotation;
	PhysicsStaticObject	 m_physicsStaticObject;
};

