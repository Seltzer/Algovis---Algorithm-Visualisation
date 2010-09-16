#ifndef USER_FUNCTIONS_H
#define USER_FUNCTIONS_H

#include <string>

namespace Algovis { class Wrapper; }


extern bool communicationWithViewEnabled;
extern bool highlightOperandsEnabled;


// Modes
void EnableCommunicationWithView(bool enabled);
void EnableOperandsHighlighting(bool enabled);

// Layout Management
void PlaceNextOnSameLineAsLast();
void PlaceNextTwoOnSameLine();

// Enforcing display
void EnsureNextIsDisplayed();
void EnsureThisIsDisplayed(Algovis::Wrapper*);


void SetCaption(std::string&);
void FlushAllActions();
void Shutdown();


void TestMethod();





#endif