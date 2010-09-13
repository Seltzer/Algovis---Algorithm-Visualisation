#include <QPainter>
#include <QColor>
#include "boost/foreach.hpp"

#include "dsPrintableActions.h"
#include "dsActions.h"
#include "../../include/registry.h"
#include "historyManager.h"
#include "../viewableObjects/vo_singlePrintable.h"

using namespace std;




namespace Algovis_Viewer
{



//////////////// DS_CreateSP
DS_CreateSP::DS_CreateSP(World* world, ID id, const void* dsAddress, const std::string& value)
		: DS_CreateAction(world, true), id(id), dsAddress(dsAddress), value(value)
{
}

DS_CreateSP::DS_CreateSP(const DS_CreateSP& other)
	: DS_CreateAction(other), id(other.id), dsAddress(other.dsAddress), value(other.value)
{
}

Action* DS_CreateSP::Clone() const
{
	return new DS_CreateSP(*this);
}

void DS_CreateSP::UpdateHistory(HistoryManager& historyManager)
{
	historyManager.AddRecord(id);
	// It is very much debatable what the history should be.
	historyManager.ResetHistory(id);
	historyManager.SetValue(id, value);

	DS_Action::UpdateHistory(historyManager);
}

/*void DS_CreateSP::Complete(bool displayed)
{
	Registry* registry = Registry::GetInstance();

	// Verify that SP hasn't already been registered
	UL_ASSERT(!registry->IsRegistered(id));

	VO_SinglePrintable* newSP = new VO_SinglePrintable(id,dsAddress, world, value);
	registry->Register(id, newSP);

	world->adjustSize();


	UL_ASSERT(registry->IsRegistered(id,SINGLE_PRINTABLE));

}*/


////////////////////// DS_Assigned implementation ////////////////////////////
DS_Assigned::DS_Assigned(World* world, ID dsAssigned, ID dsSource, std::string value, bool tracked)
	: DS_DataFlowAction(world), value(value), dsAssigned(dsAssigned), dsSource(dsSource), tracked(tracked)
{
	//ViewableObject* viewable = (ViewableObject*)subject;
	//subjects.insert(viewable); // May need to keep the original printable pointer?
}

DS_Assigned::DS_Assigned(const DS_Assigned& other)
	: DS_DataFlowAction(other), dsAssigned(other.dsAssigned), dsSource(other.dsSource), value(other.value),
	subject(other.subject), sources(other.sources)
{
}

Action* DS_Assigned::Clone() const 
{ 
	return new DS_Assigned(*this); 
}


void DS_Assigned::UpdateHistory(HistoryManager& historyManager)
{
	if (dsSource != INVALID)
		history = historyManager.GetHistory(dsSource);
	if (historyManager.IsVisible(dsAssigned))
	{
		suppressAnimation = false;
		historyManager.ResetHistory(dsAssigned);
	}
	else
	{
		suppressAnimation = true;
		if (dsSource != INVALID)
			historyManager.SetHistory(dsAssigned, history);
		else
			historyManager.ResetHistory(dsAssigned); // I don't know... seemed like a good idea at the time
	}

	historyManager.SetValue(dsAssigned, value);

	DS_Action::UpdateHistory(historyManager);
}


void DS_Assigned::PrepareToPerform()
{
	//subject->EnableCommunicationWithView(false);

	Registry* registry = Registry::GetInstance();

	UL_ASSERT(registry->IsRegistered(dsAssigned, SINGLE_PRINTABLE));
	subject = registry->GetRepresentation<VO_SinglePrintable>(dsAssigned);
	UL_ASSERT(subject);

	// Set subjectStart to have abs position
	subjectDimensions = QRect(subject->GetPositionInWorld(), subject->size());

	sources = HistoryToSources(history, subject);
}

void DS_Assigned::Perform(float progress, QPainter* painter)
{
	//if (source == NULL)
	//	return;

	float x,y;

	float lerp = 1 - (cos(progress * 3.14159265358) + 1) / 2;
	
	for (unsigned i = 0; i < sources.size(); i++)
	{
		SourceData& source = sources[i];
		if (source.source != NULL) // TODO: Need better value recording for untracked stuff
		{
			if (source.isSibling)
			{
				// Assume source and subject have the same y for now
				x = source.dimensions.x() + lerp * (subjectDimensions.x() - source.dimensions.x());
				//y = sourceDimensions.y() - 5 * sin(3.14 * progress) * (subjectStart.y() - sourceDimensions.y());
				y = source.dimensions.y() - 30 * sin(3.14 * progress);
			}
			else
			{
				x = source.dimensions.x() + lerp * (subjectDimensions.x() - source.dimensions.x());
				y = source.dimensions.y() + lerp * (subjectDimensions.y() - source.dimensions.y());
			}

			painter->setPen(QColor(Qt::white));
			source.source->DrawValue(false, QRect(QPoint(x,y),QSize(source.dimensions.width(), source.dimensions.height())),painter);
		}
	}
		
	//source->DrawWithoutValue(sourceDimensions, painter);
	//subject->DrawWithoutValue(subjectDimensions, painter);

	/* Bulging is commented out until DrawValue can draw to a specified dimension properly

	QRect updatedBounds = subjectStart;
	int bulge = int(progress * 100);

	updatedBounds.Left -= bulge;
	updatedBounds.Right += bulge;
	updatedBounds.Top -= bulge;
	updatedBounds.Bottom += bulge;*/
	
	// MIGRATION
	//subject->SetBoundingBox(updatedBounds);


	//subject->DrawWithoutValue(QRect(
	//subject->DrawWithoutValue(
	//subject->DrawWithoutValue(renderWindow, defaultFont);
	//subject->DrawValue(updatedBounds,renderWindow, defaultFont);


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
	subject->UpdateValue(value, completeTime);
}




////////////////////// DS_Modified implementation ////////////////////////////
DS_Modified::DS_Modified(World* world, ID dsModified, ID dsSource, std::string value, bool tracked)
	: DS_DataFlowAction(world), value(value), dsModified(dsModified), dsSource(dsSource), tracked(tracked)
{
	//ViewableObject* viewable = (ViewableObject*)subject;
	//subjects.insert(viewable); // May need to keep the original printable pointer?
}

DS_Modified::DS_Modified(const DS_Modified& other)
	: DS_DataFlowAction(other), dsModified(other.dsModified), dsSource(other.dsSource), value(other.value),
	subject(other.subject), sources(other.sources)
{
}


Action* DS_Modified::Clone() const
{
	return new DS_Modified(*this);
}

void DS_Modified::UpdateHistory(HistoryManager& historyManager)
{
	// Combine the two histories
	history = historyManager.GetHistory(dsModified);
	if (dsSource != INVALID)
	{
		std::set<ValueID> otherHistory = historyManager.GetHistory(dsSource);
		history.insert(otherHistory.begin(), otherHistory.end());
	}
		
	if (historyManager.IsVisible(dsModified))
	{
		suppressAnimation = false;
		historyManager.ResetHistory(dsModified);
	}
	else
	{
		suppressAnimation = true;
		historyManager.SetHistory(dsModified, history);
	}

	historyManager.SetValue(dsModified, value);

	DS_Action::UpdateHistory(historyManager);
}

void DS_Modified::PrepareToPerform()
{
	//subject->EnableCommunicationWithView(false);

	Registry* registry = Registry::GetInstance();

	UL_ASSERT(registry->IsRegistered(dsModified, SINGLE_PRINTABLE));
	subject = registry->GetRepresentation<VO_SinglePrintable>(dsModified);
	UL_ASSERT(subject);

	// Set subjectStart to have abs position
	subjectDimensions = QRect(subject->GetPositionInWorld(), subject->size());

	// Set up data for all the sources
	sources = HistoryToSources(history, subject);
}

void DS_Modified::Perform(float progress, QPainter* painter)
{
	//if (source == NULL)
	//	return;

	float x,y;

	float lerp = 1 - (cos(progress * 3.14159265358) + 1) / 2;
	
	for (unsigned i = 0; i < sources.size(); i++)
	{
		SourceData& source = sources[i];
		if (source.source != NULL)
		{
			if (source.isSibling)
			{
				// Assume source and subject have the same y for now
				x = source.dimensions.x() + lerp * (subjectDimensions.x() - source.dimensions.x());
				//y = sourceDimensions.y() - 5 * sin(3.14 * progress) * (subjectStart.y() - sourceDimensions.y());
				y = source.dimensions.y() - 30 * sin(3.14 * progress);
			}
			else
			{
				x = source.dimensions.x() + lerp * (subjectDimensions.x() - source.dimensions.x());
				y = source.dimensions.y() + lerp * (subjectDimensions.y() - source.dimensions.y());
			}

			painter->setPen(QColor(Qt::white));
			source.source->DrawValue(false, QRect(QPoint(x,y),QSize(source.dimensions.width(), source.dimensions.height())),painter);
		}
	}
}

void DS_Modified::Complete(bool displayed)
{
	subject->UpdateValue(value, completeTime);
}

/////////////////// DS_HighlightOperands
DS_HighlightOperands::DS_HighlightOperands(World* world, std::vector<ID> operands, ComparisonOps opType)
	: DS_Action(world), opType(opType), operands(operands) 
{
}

DS_HighlightOperands::DS_HighlightOperands(const DS_HighlightOperands& other)
	: DS_Action(other), opType(other.opType), operands(other.operands), operandPtrs(other.operandPtrs), 
				originalBBColour(other.originalBBColour)
{
}

Action* DS_HighlightOperands::Clone() const
{
	return new DS_HighlightOperands(*this);
}

void DS_HighlightOperands::UpdateHistory(HistoryManager& historyManager)
{
	suppressAnimation = true;
	BOOST_FOREACH(ID op, operands)
	{
		if (op != INVALID && historyManager.IsVisible(op)) // found an operand we care about
			suppressAnimation = false;
	}
}
	
void DS_HighlightOperands::PrepareToPerform()
{
	FetchOperandPtrs();
}

bool DS_HighlightOperands::FetchOperandPtrs()
{
	// Check whether we already have them or not
	if (operandPtrs.size() < operands.size())
	{
		Registry* reg = Registry::GetInstance();
	
		BOOST_FOREACH(ID op, operands)
		{
			if (!reg->IsRegistered(op, SINGLE_PRINTABLE))
				return false;

			VO_SinglePrintable* vo = reg->GetRepresentation<VO_SinglePrintable>(op);
			UL_ASSERT(vo);		
		
			// Who cares if this is called multiple times? It's a hack!
			originalBBColour = vo->GetBoundingBoxColour();
			operandPtrs.push_back(vo);
		}
	}

	return true;
}
	
void DS_HighlightOperands::Perform(float progress, QPainter* painter)
{	
	if (!FetchOperandPtrs())
		return;

	BOOST_FOREACH(VO_SinglePrintable* vo, operandPtrs)
	{
		int intensity = (int) (progress * 255);
		vo->SetBoundingBoxColour(QColor(intensity, intensity, intensity));
	}

	QPoint vo1 = operandPtrs[0]->GetPositionInWorld();
	QPoint vo2 = operandPtrs[1]->GetPositionInWorld();

	QString op;
	switch(opType)
	{
		case EQUAL:
			op = "==";
			break;
		case NOT_EQUAL:
			op = "!=";
			break;
		case LESS_THAN:
			op = (vo2.x() - vo1.x() > 0) ? "<" : ">";
			break;
		case LESS_THAN_OR_EQUAL:
			op = (vo2.x() - vo1.x() > 0) ? "<=" : ">=";
			break;
		case MORE_THAN:
			op = (vo2.x() - vo1.x() > 0) ? ">" : "<";
			break;
		case MORE_THAN_OR_EQUAL:
			op = (vo2.x() - vo1.x() > 0) ? ">=" : "<=";
			break;
		default:
			UL_ASSERT(false);
			break;
	}

	int x = (vo1.x() + vo2.x()) / 2;
	int y = vo1.y() - 10;

	const QFont oldFont = painter->font();
	painter->setFont(operandPtrs[0]->font());
	painter->setPen(QColor(255,0,255));
	painter->drawText(QPoint(x,y), op);
	painter->setFont(oldFont);
}

void DS_HighlightOperands::Complete(bool displayed)
{
	BOOST_FOREACH(VO_SinglePrintable* vo, operandPtrs)
		vo->SetBoundingBoxColour(originalBBColour);
}




}
