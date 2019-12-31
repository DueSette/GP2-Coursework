#include "GameObject.h"

GameObject::GameObject()
	:_mesh(new Mesh()), _texture(nullptr), _shader(new Shader()), _transform(new Transform())
{}

//General startup function that gets all the other classes and files in one place, one overload lets you select shaders and the other uses "regular" ones
void GameObject::initialise(const std::string& meshName, const std::string& textureName, const std::string& vertShader, const std::string& fragShader, glm::vec3 pos, ColliderType collType = ColliderType::NONE)
{
	_mesh->loadModel(meshName);
	_texture = new Texture(textureName);
	_shader->createShaderProgram(vertShader, fragShader);
	_transform->SetPos(pos);
	
	switch (collType)
	{
	case ColliderType::NONE:
		_collider = nullptr;
		break;

	case ColliderType::SPHERE:
		//_collider = &SphereCollider(*_transform->GetPos(), 1);
		break;

	case ColliderType::BOX:
		_collider = &BoxCollider(*_transform->GetPos(), 1, 1, 1);
		break;

	default:
		print("Unknown collider type is being added", DebugMessageTier::ERROR);
		break;
	}
	this->Setup();
}

void GameObject::Setup() //post-initialize
{
	_transform->SetScale(glm::vec3(1, 1, 1));
}

void GameObject::drawProcedure(const Camera& cam) //all the graphics-related operations in one place, camera has to be passed in from the main loop of course
{
	_shader->Bind();
	_shader->update(*_transform, cam);

	_texture->Bind(0);
	_mesh->draw();	
}

glm::vec3 GameObject::getPosition()
{
	return *_transform->GetPos();
}

glm::vec3 GameObject::getRotation()
{
	return *_transform->GetRot();
}

void GameObject::setPosition(glm::vec3 pos)
{
	_transform->SetPos(pos);
}

void GameObject::setRotation(glm::vec3 rot)
{
	_transform->SetRot(rot);
}

void GameObject::setScale(glm::vec3 scale)
{
	_transform->SetScale(scale);
}

void GameObject::translate(glm::vec3 translation)
{
	_transform->SetPos(*_transform->GetPos() + translation);
}

void GameObject::rotate(glm::vec3 rotation)
{
	_transform->SetRot(*_transform->GetRot() + rotation);
}

bool GameObject::AddCollider(ColliderType t)
{
	if (_collider != nullptr) { return false; } //if we already have a collider, don't add another one (might add collider swap later)

	if (t == ColliderType::BOX)
		_collider = &BoxCollider(*_transform->GetPos(), 1, 1, 1);
	//else if (t == ColliderType::SPHERE)
		//_collider = &SphereCollider(*_transform->GetPos(), 1);

	return true;
}

BoxCollider* GameObject::getCollider()
{
	return _collider;
}
