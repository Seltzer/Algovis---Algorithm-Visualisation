
#include "dsaction.h"
#include "viewAbleObjects/vo_singlePrintable.h"

namespace Algovis_Viewer
{

Action* DS_Action::Clone() const
{
	return new DS_Action(*this);
}

Action* DS_Assigned::Clone() const
{
	return new DS_Assigned(*this);
}

void DS_Assigned::Complete()
{
	subject->Assigned(history, value);
}

//virtual Action* Clone() const;



Action* DS_TestAction::Clone() const
{
	return new DS_TestAction(*this);
}

void DS_TestAction::Complete()
{
	
}




}