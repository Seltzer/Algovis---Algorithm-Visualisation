#ifndef VO_SINGLE_PRINTABLE_H_
#define VO_SINGLE_PRINTABLE_H_


#include "viewableObject.h"
#include "../actions/dsActions.h"



namespace Algovis_Viewer
{

	class VO_SinglePrintable : public ViewableObject
	{

	public:

		VO_SinglePrintable(ID id, const void* dsAddress, World* world,
							const std::string& value, QWidget* parent = NULL );

		~VO_SinglePrintable();

		virtual ViewableObjectType GetType() const { return SINGLE_PRINTABLE; }

		// QT Overloads
		virtual QSize sizeHint() const;
		virtual void paintEvent(QPaintEvent*);

		// TODO: DrawValue doesn't draw to the specified size yet
		virtual void DrawValue(bool includingChildren, const QRect&, QPainter*);
		virtual void DrawBoundingBox(const QRect& desiredBoundingBox, QPainter*);

		
		

		std::string GetValue() { return value; }

		//std::set<ValueID> GetHistory() { return history; }

		void UpdateValue(const std::string& newValue, int time);

		void UpdateValueHack(const std::string& newValue);

		int ModifiedTime() { return modifiedTime; }

		// This is called when the value has been drawn, and therefore any usage of this printable
		// should consider that drawn value it's history.
		//void ResetHistory(ValueID drawnValue);
		//void Assigned(std::set<ValueID> history, const std::string& newValue);
		// TODO combine Modified and ModifiedUntracked
		//void Modified(VO_SinglePrintable* source, const std::string& newValue);
		//void ModifiedUntracked(const void* dsAddress, const std::string& newValue);
	
	private:
		std::string value;
		mutable QString graphicalText;
		mutable QPoint graphicalTextPosition;

		int modifiedTime;
	};

	class VO_SinglePrintableFactory : public ViewableObjectFactory
	{
	protected:
		std::string value;
	public:
		VO_SinglePrintableFactory(ID id, const void* dsAddress, World* world,
							const std::string& value, QWidget* parent = NULL)
			: ViewableObjectFactory(id, dsAddress, world, parent), value(value)
		{}
		virtual ViewableObject* Create();
	};

}





#endif