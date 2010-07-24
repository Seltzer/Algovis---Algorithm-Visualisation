#ifndef COMMON_H_
#define COMMON_H_


#ifdef IMPORTING
	#define DECLSPEC __declspec(dllimport)	
#else
	#define DECLSPEC __declspec(dllexport)	
#endif

#define INVALID -1


namespace Algovis_Viewer
{
	#define DEBUG_VERBOSE

	/* ARBITRARY is a type we don't know how to draw (i.e. most user-defined types.
	 * See the viewableObjects folder for implementations
	 *
	 * TODO: Add a pointer type
	 */
	enum ViewableObjectType { ARRAY, LINKED_LIST, SINGLE_PRINTABLE, ARBITRARY};
}






#endif