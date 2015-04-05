#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <set>
#include <map>

#include "Component.h"

namespace Comet
{
	class RenderObject;
	class Renderable;
	class Renderer;

	class Node : public Component
	{
	public:
		Node();
		~Node();

		Node* CreateNode();
		void Attach(Node* n);
		void Attach(RenderObject* obj);
		void DestroyNode(Node* n);
		void Detach(Node* n);
		void Detach(RenderObject* obj);
		void SetParent(Node* n);

		void SetPosition(glm::vec3 pos);
		void SetRotation(glm::quat rot);
		void Translate(glm::vec3 vec, int space);
		void Rotate(float angle, glm::vec3 vec, int space);
		void Scale(glm::vec3 vec);

		glm::vec3 GetPosition();
		glm::vec3 GetLocalPos();
		glm::quat GetRotation(){ return rotation; }
		glm::vec3 GetScale(){ return scale; }
		glm::mat4 GetTransform();
		glm::mat4 GetLocalTransform();
		glm::vec3 GetUp();
		glm::vec3 GetBack();
		glm::vec3 GetRight();

		std::string GetName(){ return name; }
		void SetName(std::string n){ name = n; }

		void SetLayer(unsigned int l){ layer = l; }
		unsigned int GetLayer(){ return layer; }

		std::set<Node*> GetNodes(){ return nodes; }

		void dirty();
		bool IsDirty();

		friend Renderer;
	private:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 rotEuler;
		glm::vec3 scale;
		glm::mat4 transform;
		std::string name;

		Node* parent;
		std::set<Node*> nodes;
		std::set<RenderObject*> objects;

		unsigned int layer;

		bool drt;
	};

};