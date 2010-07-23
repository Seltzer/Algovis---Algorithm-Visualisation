#ifndef UI_ACTION_H_
#define UI_ACTION_H_


#include "../action.h"

namespace Algovis_Viewer
{

	enum UI_ActionType { MoveVO, SuppressVO, ShowVO };


	class UI_Action : public Action
	{

	private:
		UI_ActionType actionType;

	public:

		UI_Action(UI_ActionType actionType, World* world)
			: Action(world)
		{
		}

		UI_Action(const UI_Action& other)
			: Action(other), actionType(other.actionType)
		{
		}

		virtual Action* Clone() const
		{
			return new UI_Action(static_cast<const UI_Action&>(*this));
		}


	};


}






#endif