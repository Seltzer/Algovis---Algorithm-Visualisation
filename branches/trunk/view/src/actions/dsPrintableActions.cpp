#include <QPainter>
#include <QColor>
#include "boost/foreach.hpp"

#include "dsPrintableActions.h"
#include "dsActions.h"
#include "../../include/registry.h"
#include "historyManager.h"
#include "../viewableObjects/vo_singlePrintable.h"

#include "../displayer/world.h"

using namespace std;




namespace Algovis_Viewer
{



//////////////// DS_CreateSP
DS_CreateSP::DS_CreateSP(World* world, ID id, const void* dsAddress, const std::string& value)
		: DS_CreateAction(world, false), id(id), dsAddress(dsAddress), value(value)
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
	myFactory = new VO_SinglePrintableFactory(id,dsAddress, world, value);

	historyManager.AddRecord(id);
	// It is very much debatable what the history should be.
	historyManager.ResetHistory(id);
	historyManager.SetValue(id, value);
	historyManager.SetFactory(id, myFactory);

	DS_Action::UpdateHistory(historyManager);


}

void DS_CreateSP::Complete(bool displayed)
{
	if (createAndDisplayASAP)
	{
		ViewableObject* element = myFactory->Create();

		if (BeingCreatedOnSameLine())
			world->AddViewableOnSameRow(element);
		else
			world->AddViewableOnNewRow(element);
		
		element->adjustSize();
		element->setVisible(true);
		Registry::GetInstance()->Register(id, element);

	}
}


////////////////////// DS_Assigned implementation ////////////////////////////
DS_Assigned::DS_Assigned(World* world, ID dsAssigned, ID dsSource, std::string newValue, bool tracked)
	: DS_DataFlowAction(world), newValue(newValue), dsAssigned(dsAssigned), dsSource(dsSource), tracked(tracked)
{
	//ViewableObject* viewable = (ViewableObject*)subject;
	//subjects.insert(viewable); // May need to keep the original printable pointer?
}

DS_Assigned::DS_Assigned(const DS_Assigned& other)
	: DS_DataFlowAction(other), dsAssigned(other.dsAssigned), dsSource(other.dsSource), 
			oldValue(other.oldValue), newValue(other.newValue),
	subject(other.subject), sources(other.sources), subjectDimensions(other.subjectDimensions)
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

	historyManager.SetValue(dsAssigned, newValue);

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

	if (!completedAtLeastOnce)
	{
		sources = HistoryToSources(history, subject);
		oldValue = subject->GetValue();
	}
	else
	{
		sources = UpdateSources(sources);
	}
}

void DS_Assigned::PrepareToUnperform()
{
	// Pretty similar to PrepareToPerform
	Registry* registry = Registry::GetInstance();

	UL_ASSERT(registry->IsRegistered(dsAssigned, SINGLE_PRINTABLE));
	subject = registry->GetRepresentation<VO_SinglePrintable>(dsAssigned);
	UL_ASSERT(subject);

	// Set subjectStart to have abs position
	subjectDimensions = QRect(subject->GetPositionInWorld(), subject->size());

	// Update, since dimensions and pointers may have changed
	sources = UpdateSources(sources);
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
}

void DS_Assigned::Unperform(float progress, QPainter* painter)
{
	return Perform((1.00f - progress), painter);
}

void DS_Assigned::Complete(bool displayed)
{
	if (!completedAtLeastOnce)
	{
		subject->UpdateValue(newValue, completeTime);
		Action::Complete(displayed);
	}
	else
	{
		subject->UpdateValue(newValue);
	}
}

void DS_Assigned::Uncomplete(bool displayed)
{
	subject->UpdateValue(oldValue);
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
	subject(other.subject), sources(other.sources), subjectDimensions(other.subjectDimensions)
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

void DS_HighlightOperands::PrepareToUnperform()
{
	// Pointers could be dead by now, so refetch them
	operandPtrs.clear();
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

	if (progress > 1.0f) 
		progress = 1.0f;
	else if (progress < 0.0f)
		progress = 0.0f;

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

	//painter->drawLine(vo1, QPoint(x,y));
	//painter->drawLine(vo2, QPoint(x,y));
	painter->setFont(oldFont);
}

void DS_HighlightOperands::Unperform(float progress, QPainter* painter)
{	
	return Perform(progress, painter);
}

void DS_HighlightOperands::Complete(bool displayed)
{
	BOOST_FOREACH(VO_SinglePrintable* vo, operandPtrs)
		vo->SetBoundingBoxColour(originalBBColour);
}

void DS_HighlightOperands::Uncomplete(bool displayed)
{
	return Complete(displayed);
}



}
