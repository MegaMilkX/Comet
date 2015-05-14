#include "Node.h"
#include "RenderObject.h"
#include "Renderer.h"

#include <sstream>

namespace Comet
{

	int _nextNodeId()
	{
		static int id;
		return id++;
	}

	//////////////////////////////////////////////////////////////////

	Node::Node()
	{
		parent = 0;
		dirty();
		scale = vec3f(1.0f, 1.0f, 1.0f);
		rotation = quat(0, 0, 0, 1.0f); //That's really important
		layer = 1;	//Default rendering group
	}

	Node::Node(std::string name) : Node()
	{
		this->name = name;
	}

	Node::~Node()
	{
		std::vector<Node*>::iterator it;
		for (it = nodes.begin(); it != nodes.end(); it++)
		{
			delete (*it);
		}
		nodes.clear();
	}

	Node* Node::CreateNode()
	{
		std::ostringstream sstream;
		sstream << "Node_" << _nextNodeId();

		Node* node = new Node(sstream.str());
		node->parent = this;
		nodes.push_back(node);
		return node;
	}

	//Math stuff
	void Node::SetPosition(vec3f pos)
	{
		position = pos;
		dirty();
	}
	void Node::SetRotation(quat rot)
	{
		rotation = rot;
		dirty();
	}

	//TODO
	void Node::Translate(vec3f vec, TSPACE space) //ƒолжно работать правильно, хорошо бы проверить
	{
		if (space == LOCAL)
		{
			vec = toMat3(rotation) * vec;
			position = position + vec;
		}
		else if(space == PARENT)
		{
			if (parent)
				vec = parent->GetTransform() * vec; //This silently takes vec3 as vec4 for multiplication with mat4x4
			position = position + vec;
		}
		else if (space == WORLD)
		{
			position = position + vec;
		}

		dirty();
	}

	void Node::Rotate(float angle, vec3f vec, TSPACE space)	//ќб€зательно проверить и скорее всего убедитьс€ в том что оно не работает правильно
																//ƒа вроде нормально все
	{
		if (space == LOCAL)
		{
			quat q = angleAxis(angle, vec);
			rotation = q * rotation;
			//rotation = normalize(rotation);
		}
		else if (space == PARENT)
		{
			//TODO	
		}
		else if (space == WORLD)
		{
			quat q = angleAxis(angle, vec);
			rotation = rotation * q;
			//rotation = normalize(rotation);
		}
		dirty();
	}
	void Node::Scale(vec3f vec)
	{
		scale = vec;
		dirty();
	}

	vec3f Node::GetPosition()
	{ 
		mat4f transform = GetTransform();
		return transform[3];
	}

	vec3f Node::GetLocalPos()
	{
		return position;
	}

	//TODO
	mat4f Node::GetTransform() //Ќужно проверить
	{
		if (IsDirty())
		{
			if (parent)
				transform = parent->GetTransform() * ((translate(mat4f(1.0f), this->position) * toMat4(rotation) * ::scale(mat4f(1.0f), this->scale)));
			else
				transform = translate(mat4f(1), this->position)*toMat4(rotation)*::scale(mat4f(1), this->scale);
			drt = false;
		}
		return transform;
	}

	//TODO
	mat4f Node::GetLocalTransform()
	{
		return (translate(mat4f(1), this->position) * toMat4(rotation) * ::scale(mat4f(1), this->scale));
	}

	vec3f Node::GetUp()
	{
		mat4f t = GetTransform();
		return vec3f(t[1][0], t[1][1], t[1][2]);
	}
	vec3f Node::GetBack()
	{
		mat4f t = GetTransform();
		return vec3f(t[2][0], t[2][1], t[2][2]);
	}
	vec3f Node::GetRight()
	{
		mat4f t = GetTransform();
		return vec3f(t[0][0], t[0][1], t[0][2]);
	}

	void Node::dirty()
	{
		drt = true;
		for (unsigned int i = 0; i < nodes.size(); ++i)
			nodes[i]->dirty();
	}
	bool Node::IsDirty()
	{return drt;}

};