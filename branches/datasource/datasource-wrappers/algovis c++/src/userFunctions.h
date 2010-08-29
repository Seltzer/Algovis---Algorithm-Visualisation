#ifndef USER_FUNCTIONS_H
#define USER_FUNCTIONS_H


extern bool drawingEnabled;

/* Use with caution:
 *
 * Drawing is disabled by default. Theoretically if drawing is never enabled during an execution run, 
 * the view DLL should never be loaded. This doesn't really work though.
 *
 */
void EnableDrawing(bool enabled);

void Shutdown();

void TestMethod();





#endif