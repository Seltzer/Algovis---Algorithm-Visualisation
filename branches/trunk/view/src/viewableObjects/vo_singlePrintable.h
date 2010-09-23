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
		void UpdateValue(const std::string& newValue);
		void UpdateValue(const std::string& newValue, int time);

		int ModifiedTime();
	
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

		// TODO hack hack
		void UpdateValue(std::string value)
		{
			this->value = value;
		}
	};

}





#endif