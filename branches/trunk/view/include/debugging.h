#ifndef DEBUGGING_H_
#define DEBUGGING_H_



namespace Algovis_Viewer
{
	/* For debugging events which aren't covered by the others below
	 * 
	 * 0 = Off
	 * 1 = Basic general debugging
	 * 2 = Same as 1, plus Registry/Displayer construction/destruction and DllMain invocations
	 */
	#define DEBUG_GENERAL_LEVEL 1


	/* 0 = Off
	 * 1 = Basic threading debugging
	 * 2 = Same as 1 plus post-locking synchronisation debug
	 * 3 = Same as 2 plus pre-locking synchronisation debug
	 * 4 = Implements a strict locking mechanism whereby recursive lock attempts block
	 */
	#define DEBUG_THREADING_LEVEL 0

	/* 0 = Off
	 * 1 = Notify when a DS object is deregistered
	 * 2 = Same as 1 plus notification when VO_Arrays are registered
	 * 3 = Same as 2 plus notification when VO_SinglePrintables are registered
	 */
	#define DEBUG_REGISTRATION_LEVEL 0

	/* 0 = Off
	 * 1 = Basic action debugging
	 * 2 = Verbose data about Registry::AddActionToBuffer
	 */
	#define DEBUG_ACTION_LEVEL 0

	// For changes made to specific VOs (excluding reg/dereg which is covered by DEBUG_REGISTRATION_LEVEL)
	//#define DEBUG_SP_CHANGES
	//#define DEBUG_ARRAY_CHANGES
}






#endif