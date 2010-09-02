#ifndef USER_FUNCTIONS_H
#define USER_FUNCTIONS_H

#include <string>


extern bool drawingEnabled;
extern bool highlightOperandsEnabled;

// Internal to DS
void EnableDrawing(bool enabled);
void EnableOperandsHighlighting(bool enabled);

// Result in Registry invocations
void SetCaption(std::string&);
void FlushAllActions();
void Shutdown();
void TestMethod();





#endif