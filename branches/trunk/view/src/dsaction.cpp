#include "dsaction.h"
#include "viewAbleObjects/vo_singlePrintable.h"
#include "../include/registry.h"



namespace Algovis_Viewer
{

int time = 0; // TODO: Seriously?... Seriously guys.

////////////////////// DS_Action implementation ////////////////////////////

Action* DS_Action::Clone() const
{
	return new DS_Action(*this);
}


////////////////////// DS_Assigned implementation ////////////////////////////


DS_Assigned::DS_Assigned(World* world, VO_SinglePrintable* subject, std::set<ValueID> history, std::string value)
	: DS_Action(world), value(value), subject(subject), history(history)
{
	ViewableObject* viewable = (ViewableObject*)subject;
	subjects.insert(viewable); // May need to keep the original printable pointer?

	
	// Set subjectStart to have absolute position
	sf::Vector2f subjectAbsPosition = viewable->GetAbsolutePosition();
	subjectStart = sf::FloatRect(subjectAbsPosition.x, 
								 subjectAbsPosition.y,						 
								 subjectAbsPosition.x + viewable->GetBoundingBox().GetWidth(),
								 subjectAbsPosition.y + viewable->GetBoundingBox().GetHeight());
}

Action* DS_Assigned::Clone() const
{
	return new DS_Assigned(*this);
}

void DS_Assigned::PrepareToPerform()
{
	subject->SetDrawingAgent(this);
}

void DS_Assigned::Perform(float progress, sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
	UL_ASSERT(subject->GetDrawingAgent() == this);

	sf::FloatRect updatedBounds = subjectStart;
	int bulge = int(progress * 100);
	
	updatedBounds.Left -= bulge;
	updatedBounds.Right += bulge;
	updatedBounds.Top -= bulge;
	updatedBounds.Bottom += bulge;
	subject->SetBoundingBox(updatedBounds);

	// Yes, we could add a Draw overload which draws to a specified dimension instead of doing it this way
	// But doing it this way tests DrawWithoutValue and DrawValue
	subject->DrawWithoutValue(renderWindow, defaultFont);
	subject->DrawValue(updatedBounds,renderWindow, defaultFont);


	/*for (std::set<ValueID>::iterator i = history.begin(); i != history.end(); i++)
	{
		Registry* reg = Registry::GetInstance();
		if (i->address != 
		if (reg->IsRegistered(i->address, SINGLE_PRINTABLE))
		{
			VO_SinglePrintable* printable = 
		}
	}*/
}

void DS_Assigned::Complete(bool displayed)
{
	subject->Assigned(history, value);
	
	if (displayed)
	{
		// As we have completed this animation and presumably displayed the history
		// and will reset the current history to be the value we just displayed
		// That way future things will have the just-displayed-element in their history, instead of everything
		// that was used to produce is.
		//int time = Algovis_Viewer::Registry::GetInstance()->GetTime();
		subject->ResetHistory(ValueID(subject->GetDSAddress(), time));

		++time;
		subject->RestorePreviousDrawingAgent();
	}
}



////////////////////// DS_TestAction implementation ////////////////////////////
Action* DS_TestAction::Clone() const
{
	return new DS_TestAction(*this);
}

void DS_TestAction::Perform(float progress, sf::RenderWindow& renderWindow, sf::Font& defaultFont)
{
	// Hacky mock animation which rotates stuff
	glRotatef(progress * 360, 0, 0, 1);
}

void DS_TestAction::Complete(bool displayed)
{
	
}




}