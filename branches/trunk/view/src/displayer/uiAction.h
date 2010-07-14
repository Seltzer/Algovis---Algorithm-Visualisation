#ifndef UI_ACTION_H_
#define UI_ACTION_H_



namespace Algovis_Viewer
{

	enum UI_ActionType { MoveVO, SuppressVO, ShowVO };


	class UI_Action
	{

	private:
		UI_ActionType actionType;
		unsigned worldCount, uiActionCount;

		// Unused at the moment; do not dereference lest you die!
		World* world; 


	public:
		/* type: Until we implement action-specific subclasses
		 * worldCount: Registry::worldCount at time of action creation
		 * uiActionCount: Registry::uiActionCount for this action
		 * world: Unused at the moment as we only hold one world instance at a time
		 */
		UI_Action(UI_ActionType actionType, unsigned worldCount, unsigned uiActionCount, World* world = NULL)
			: actionType(actionType), worldCount(worldCount), uiActionCount(uiActionCount), world(world)
		{
		}



	};


}






#endif