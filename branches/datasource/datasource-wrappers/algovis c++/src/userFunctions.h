#ifndef USER_FUNCTIONS_H
#define USER_FUNCTIONS_H

#include <string>

namespace Algovis
{
	class Wrapper;
}


extern bool communicationWithViewEnabled;
extern bool highlightOperandsEnabled;

// Internal to DS
void EnableCommunicationWithView(bool enabled);
void EnableOperandsHighlighting(bool enabled);

// Result in Registry invocations
void PlaceNextOnSameLineAsLast();
void PlaceNextTwoOnSameLine();
void EnsureNextIsDisplayed();
void EnsureThisIsDisplayed(Algovis::Wrapper*);


void SetCaption(std::string&);
void FlushAllActions();
void Shutdown();
void TestMethod();





#endif