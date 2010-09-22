#ifndef TYPE_INTERFACE_H_
#define TYPE_INTERFACE_H_



#include <string>
#include <iostream>
#include "common.h"




template<typename S, typename T>                                
struct SameClass 
{                                             
    template <typename IDENTICAL, IDENTICAL> 
	struct type_match;                    

    template <typename _1> 
	static char (& func( type_match< S& (T::*)(const T&), &_1::operator = > *))[1];
    
	template <typename   > 
	static char (& func(...))[2];            
    
	static bool const value = sizeof(func<S>(0)) == 1;              
};

// Returns true if c1 and c2 are exactly the same class, not if they're merely polymorphically compatible
template<typename c1, typename c2>
bool IsSameClassAs()
{
	return SameClass<c1,c2>::value == 1;
}

// Function must be directly defined in class T
#define HAS_VOID_VOID_FN(name,funcName)											 \
    template<typename T>														 \
    struct name																	 \
	{																			 \
        template <typename IDENTICAL, IDENTICAL>			     				 \
		struct type_match;														 \
																				 \
		template <typename _1>													 \
		static char (& func( type_match< void (T::*)(), &_1::funcName > *))[1];  \
																				 \
        template <typename   >													 \
		static char (& func(...))[2];											 \
																				 \
        static bool const value = sizeof(func<T>(0)) == 1;						 \
    };

HAS_VOID_VOID_FN(PrimitiveCheck, __IAmAPrimitiveWrapper)
HAS_VOID_VOID_FN(StringCheck, __IAmAStringWrapper)
HAS_VOID_VOID_FN(VecCheck, __IAmAVectorWrapper)
HAS_VOID_VOID_FN(DequeCheck, __IAmADequeWrapper)
HAS_VOID_VOID_FN(MatrixCheck, __IAmAMatrix)	

#undef HAS_VOID_VOID_FN






// The bridge between classes and ViewableObject types
template<class T>
Algovis_Viewer::ViewableObjectType GetVOType()
{
	if ( (PrimitiveCheck<T>::value == 1) || (StringCheck<T>::value == 1) )
		return Algovis_Viewer::SINGLE_PRINTABLE;
	else if ( (VecCheck<T>::value == 1) || (DequeCheck<T>::value == 1) )
		return Algovis_Viewer::ARRAY;
	else if (MatrixCheck<T>::value == 1)
		return Algovis_Viewer::MATRIX;
	else
		return Algovis_Viewer::ARBITRARY;
}




#endif