#include "Animator.h"
#include "Node.h"

namespace Comet
{

	Animator::Animator() : time(0), anim(0)
	{
	}


	Animator::~Animator()
	{
	}

	void Animator::_updateData()
	{
		int nCurves = anim->GetCurveCount();
		for (int i = 0; i < nCurves; ++i)
		{
			Curve* c = anim->GetCurve(i);
			c->Evaluate(time, node);
		}
	}

}