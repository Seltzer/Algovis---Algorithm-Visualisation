#ifndef VO_SINGLE_PRINTABLE_H_
#define VO_SINGLE_PRINTABLE_H_


#include "viewableObject.h"



namespace Algovis_Viewer
{

	struct ValueID
	{
		ValueID(const void* address, int time) : address(address), time(time) {}
		bool operator<(const ValueID& rhs) const
		{
			if (time == rhs.time)
				return address < rhs.address;
			return time < rhs.time;
		}
		const void* address;
		int time;
	};

	// TODO: Used by viewer to keep track of past values for playback - may need something fancier?
	// TODO: Move this somewhere better
	// TODO: Only makes sense for primitives?
	/*class DisplayedValue
	{
		ValueID id;
		// TODO: Value?
		std::set<ValueID> contributors;
	};*/

	class VO_SinglePrintable : ViewableObject
	{

	private:
		std::string value;
		// TODO: Keep track of data structure?

		// Keep track of the displayed values which have contributed to the current value
		// If the value has just been displayed, then this is a single element set with the current
		// address and time.
		std::set<ValueID> history;

		void UpdateValue(const std::string& newValue) 
		{ 
			//if (value != newValue) // Update should be displayed, with history, even if the same value is assigned!
			{
				value = newValue; 
				NotifyObservers();
			}
		}

	public:
		VO_SinglePrintable(const void* dsAddress, const std::string& value)
				: ViewableObject(dsAddress), value(value)
		{
		}

		~VO_SinglePrintable() {}

		virtual ViewableObjectType GetType() { return SINGLE_PRINTABLE; }

		virtual void Draw(sf::RenderWindow& renderWindow, sf::Font& font) {}

		std::string GetValue() { return value; }

		std::set<ValueID> GetHistory() { return history; }

		// This is called when the value has been drawn, and therefore any usage of this printable
		// should consider that drawn value it's history.
		void ResetHistory(ValueID drawnValue)
		{
			history.clear();
			history.insert(drawnValue);
		}

		void Assigned(VO_SinglePrintable* source, const std::string& newValue)
		{
			// This printable now has the same history as the one it was assigned from
			// This is true because history only includes items that have been displayed
			history = source->GetHistory();

			UpdateValue(newValue);
		}

		void AssignedUntracked(const void* dsAddress, const std::string& newValue)
		{
			// The only history we have is that the value was untracked, so chuck in an element to represent that
			history.clear();
			history.insert(ValueID(dsAddress, -1)); // Time -1 denotes elements that aren't actually tracked (TODO: That sucks)

			UpdateValue(newValue);
		}

		void Modified(VO_SinglePrintable* source, const std::string& newValue)
		{
			// This printable now includes the history of the printable that modified it, plus it's own history
			const std::set<ValueID>& sourceHistory = source->GetHistory();
			history.insert(sourceHistory.begin(), sourceHistory.end());

			UpdateValue(newValue);
		}

		void ModifiedUntracked(const void* dsAddress, const std::string& newValue)
		{
			// The printable retains it's current history, but notes that it has been modified by something that is not tracked.
			history.insert(ValueID(dsAddress, -1)); // Time -1 denotes elements that aren't actually tracked (TODO: That sucks)

			UpdateValue(newValue);
		}
	};

}





#endif