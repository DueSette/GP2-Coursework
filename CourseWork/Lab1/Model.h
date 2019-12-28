#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "obj_loader.h"
#include <iostream>
#include "Utilities.h"
#include "stb_image.h"
#include "Transform.h"
#include <cassert>
/*
	This file contains the data regarding collision shapes, vertices, meshes and textures, all in one place for better readability
*/

//===================================  VERTEX  =============================
struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
		:position(pos), textureUV(texCoord), normal(normal)		//member initializer list makes the process more efficient
	{
	}

	//Since Vertex is a very small struct with simple code, we declare AND define the methods in the .h file
	glm::vec3* getPosition() { return &position; }
	glm::vec2* getTexCoord() { return &textureUV; }
	glm::vec3* getNormal() { return &normal; }

private:
	glm::vec3 normal;
	glm::vec2 textureUV;
	glm::vec3 position;
};

//===================================  COLLISION  =============================
enum ColliderType {NONE, SPHERE, BOX }; //useful for scaling and pattern matching

//base class, never used directly
class Collider
{
public:
	//we make every collider able to return a vec3 of its size for compatibility
	virtual glm::vec3 getSize()
	{
		std::cout << "wrong";
		return VECTOR_ZERO;
	}
};

class SphereCollider : public Collider
{
public:
	SphereCollider() {}
	inline SphereCollider(const glm::vec3& position, float radius)
		:radius(radius)
	{
		print("Created Sphere Collider", DebugMessageTier::INFO);
	}

	glm::vec3 getSize() = 0;

	float* getRadius() { return &radius; }
	void setRadius(float newRadius) { radius = newRadius; }

private:
	float radius = 0;
};

class BoxCollider : public Collider
{
public:
	inline BoxCollider(const glm::vec3& position, float x, float y, float z)
		:sizeX(x), sizeY(y), sizeZ(z)
	{
		print("Created Box Collider", DebugMessageTier::INFO);
	}

	glm::vec3 getSize()
	{
		std::cout << "rite";
		return glm::vec3(sizeX, sizeY, sizeZ);
	}

	inline void setSize(float x = 0 , float y = 0 , float z = 0 )
	{
		sizeX = x;
		sizeY = y;
		sizeZ = z;
	}

	//we set this to default -100 because it's safe to assume we will never manually call it with such parameters
	inline void scaleSize(float x = -100, float y = -100, float z = -100) 
	{
		if(sizeX != -100)
			sizeX *= x;
		if (sizeY != -100)
			sizeY *= y;
		if (sizeZ != -100)
			sizeZ *= z;
	}

private:
	float sizeX, sizeY, sizeZ;
};
//===================================  TEXTURE  =============================

class Texture
{
public:
	Texture(const std::string& fileName);

	void Bind(unsigned int unit); // bind upto 32 textures

	~Texture();

private:
	GLuint textureHandler;
};

//===================================  MESH  =============================

class Mesh
{
public:
	Mesh();
	~Mesh();

	void draw();
	void initWithoutModel(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices); //draws a hardcoded mesh (not from model)
	void loadModel(const std::string& filename);
	void initWithModel(const IndexedModel& model); //always use this

private:
	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS]; // create our array of buffers
	unsigned int drawCount; //how much of the vertexArrayObject do we want to draw
};
