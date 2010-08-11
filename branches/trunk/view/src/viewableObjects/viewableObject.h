#ifndef VIEWABLE_OBJECT_H_
#define VIEWABLE_OBJECT_H_

#include <set>
#include "../../include/common.h"
#include "../displayer/components.h"



/* ViewableObject abstract class - represents a viewable object drawn with a bounding box
 */
namespace Algovis_Viewer
{

	class World;
	class DS_Action;
	class ViewableObject;


	class ViewableObject : public Component
	{

	protected:
		const void* dsAddress;
		
		World* world;
				
		float boundingBoxColour[3];
		sf::Font* font;
		bool suppressed;

		
		ViewableObject(const void* dsAddress, World* world); 

	public:
		virtual ~ViewableObject();

		virtual ViewableObjectType GetType() = 0;

		// Optional method for drawing VO's value at a specified absolute position with specified dimensions
		virtual void DrawValue(sf::FloatRect&, sf::RenderWindow&, sf::Font& defaultFont) {}

		// Optional method for drawing a VO exactly like Draw() but without its value
		virtual void DrawWithoutValue(sf::RenderWindow&, sf::Font& defaultFont) {}
		



		// RGB args are defined [0,1]
		virtual void SetBoundingBoxColour(float r, float g, float b);

		const void* GetDSAddress() { return dsAddress; }
	};

	

}



#endif