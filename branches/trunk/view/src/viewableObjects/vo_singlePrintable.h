#ifndef VO_SINGLE_PRINTABLE_H_
#define VO_SINGLE_PRINTABLE_H_


#include "viewableObject.h"
#include "../actions/dsActions.h"



namespace Algovis_Viewer
{

	class VO_SinglePrintable : public ViewableObject
	{

	public:

		VO_SinglePrintable(const void* dsAddress, World* world,
							const std::string& value, QWidget* parent = NULL );

		virtual ViewableObjectType GetType() const { return SINGLE_PRINTABLE; }

		// QT Overloads
		virtual QSize sizeHint() const;
		virtual void paintEvent(QPaintEvent*);

		// TODO: DrawValue doesn't draw to the specified size yet
		virtual void DrawValue(QRect& desiredBoundingBox, QPainter*);
		virtual void DrawWithoutValue(QRect& desiredBoundingBox, QPainter*);

		virtual void SetupLayout();

		

		std::string GetValue() { return value; }

		std::set<ValueID> GetHistory() { return history; }

		void UpdateValue(const std::string& newValue);

		// This is called when the value has been drawn, and therefore any usage of this printable
		// should consider that drawn value it's history.
		void ResetHistory(ValueID drawnValue);
		void Assigned(std::set<ValueID> history, const std::string& newValue);
		// TODO combine Modified and ModifiedUntracked
		void Modified(VO_SinglePrintable* source, const std::string& newValue);
		void ModifiedUntracked(const void* dsAddress, const std::string& newValue);
	

	private:
		std::string value;
		QString graphicalText;
		QPoint graphicalTextPosition;
	
		// Keep track of the displayed values which have contributed to the current value.
		// If the value has just been displayed, then this is a single element set with the current
		// address and time.
		std::set<ValueID> history;

	};

}





#endif