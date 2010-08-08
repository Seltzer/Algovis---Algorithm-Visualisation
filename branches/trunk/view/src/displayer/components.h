#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include "SFML/Graphics.hpp"


namespace Algovis_Viewer
{


	class Component
	{
	
	public:
		Component();
		Component(sf::FloatRect boundingBox);
	
		/* Returns preferred size or current bounding box if VO is not capable of 
		 * calculating its preferred size. 
		 *
		 * Note that current bounding box can be different to the preferred size (usually when
		 * a parent entity sets the bounding box based on parameters external to this VO.
		 */
		virtual sf::FloatRect GetPreferredSize();
		virtual sf::FloatRect GetBoundingBox();
		
		virtual void SetBoundingBox(sf::FloatRect);

		virtual void SetParentComponent(Component*);

		/* NB: Component may choose to ignore the defaultFont passed to it and draw using
		 *	   its own choice of font.
		 */
		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont) = 0;

	protected:
		sf::FloatRect boundingBox;
		Component* parentComponent;
		
	};


	class Panel : public Component
	{
	
	public:
		Panel() {}
		Panel(sf::FloatRect boundingBox, float backgroundColour[3], float borderColour[3]); 

		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& defaultFont);

	private:
		float backgroundColour[3];
		float borderColour[3];
	};



}












#endif