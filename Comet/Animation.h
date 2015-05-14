#pragma once

#include "Resource.h"

#include <fbxsdk.h>
#include <fbxsdk\fileio\fbxiosettings.h>

#include "math3f.h"

#include <vector>
#include <algorithm>
#include <memory>

#include "Node.h"

namespace Comet
{

	class Key
	{
	public:
		enum INTERPOLATIONMODE { CONSTANT, LINEAR, CUBIC };
		Key(int t, float v) : time(t), value(v), mode(CONSTANT){}
		int Time() const { return time; }
		float Value() const { return value; }
		bool operator<(const Key &k)
		{return (time < k.Time());}
	private:
		int time;
		float value;
		INTERPOLATIONMODE mode;
	};

	class Curve
	{
	public:
		~Curve()
		{
			for (unsigned int i = 0; i < curves.size(); ++i)
				delete curves[i];
		}
		void SetKey(int time, float value)
		{
			timeline.push_back(Key(time, value));
			std::sort(timeline.begin(), timeline.end());
		}
		//TODO: NOT IMPLEMENTED
		void RemoveKey(int time){}

		virtual float Evaluate(float time, Node* node = 0)
		{
			Key a(0,0), b(0,0);
			for (unsigned int i = 0; i < timeline.size(); ++i)
			{
				if (timeline[i].Time() > time)
				{
					b = timeline[i];
					break;
				}
				a = timeline[i];
				b = a;
			}
			if (a.Time() == b.Time())
				return a.Value();
			float valdiff = b.Value() - a.Value();
			float weight = (time - a.Time()) / (float)(b.Time() - a.Time());
			return a.Value() + valdiff * smoothstep(0.0f, 1.0f, weight);
		}

		Curve* GetCurve(int i){ return curves[i]; }
	protected:
		std::vector<Key> timeline;
		std::vector<Curve*> curves;
	};

	class TranslationCurve : public Curve
	{
	public:
		TranslationCurve()
		{
			curves.push_back(new Curve()); curves.push_back(new Curve()); curves.push_back(new Curve());
		}
		float Evaluate(float time, Node* node)
		{
			node->SetPosition(vec3f(curves[0]->Evaluate(time), curves[1]->Evaluate(time), curves[2]->Evaluate(time)));
			return 0.0f;
		}
	};

	class RotationCurve : public Curve
	{
	public:
		RotationCurve()
		{
			curves.push_back(new Curve()); curves.push_back(new Curve()); curves.push_back(new Curve());
		}
		float Evaluate(float time, Node* node)
		{
			float x = curves[0]->Evaluate(time);
			float y = curves[1]->Evaluate(time);
			float z = curves[2]->Evaluate(time);
			float c1 = cos(y * 0.5f);
			float c2 = cos(z * 0.5f);
			float c3 = cos(x * 0.5f);
			float s1 = sin(y * 0.5f);
			float s2 = sin(z * 0.5f);
			float s3 = sin(x * 0.5f);
			quat q;
			q.x = s1*s2*c3+c1*c2*s3;
			q.y = s1*c2*c3+c1*s2*s3;
			q.z = c1*s2*c3-s1*c2*s3;
			q.w = c1*c2*c3-s1*s2*s3;
			node->SetRotation(q);
			return 0.0f;
		}
	};

	class ScaleCurve : public Curve
	{
	public:
		ScaleCurve()
		{
			curves.push_back(new Curve()); curves.push_back(new Curve()); curves.push_back(new Curve());
		}
		float Evaluate(float time, Node* node)
		{
			node->Scale(vec3f(curves[0]->Evaluate(time), curves[1]->Evaluate(time), curves[2]->Evaluate(time)));
			return 0.0f;
		}
	};

	//Animation track resource
	class Animation : public Resource
	{
	public:
						Animation();
						~Animation();

		void			Load(std::string path);
		void			Unload();
		
		//
		void			SetLength(int l){ length = l; }
		int				GetLength(){ return length; }

		template<typename T>
		Curve*			NewCurve()
		{
			curves.push_back(new T());
			return curves[curves.size() - 1];
		}
		int				GetCurveCount(){ return curves.size(); }
		Curve*			GetCurve(int i){ return curves[i]; }
	private:
		std::vector<Curve*>		curves;
		std::vector<std::shared_ptr<Animation>> animations;
		int						length;
		void					_ProcessNode(FbxNode* node, FbxAnimLayer* layer);
		void					_GetKeys(FbxAnimCurveNode* curve, Curve* dst, int n, bool toRadians = false);
	};

}