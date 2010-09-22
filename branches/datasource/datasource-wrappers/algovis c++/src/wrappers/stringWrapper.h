#ifndef STRINGWRAPPER_H_
#define STRINGWRAPPER_H_

#include <STRING>
#include "wrapper.h"


namespace Algovis
{


class StringWrapper : public Wrapper
{

public:
	static const size_t npos = -1;

	StringWrapper();
	StringWrapper(const StringWrapper&);
	StringWrapper(const StringWrapper&, size_t pos, size_t n = npos );
	StringWrapper(const char*);
	StringWrapper(const char*, size_t n);
	StringWrapper(size_t n, char c);
	
	/*
	template<class InputIterator> 
	StringWrapper (InputIterator begin, InputIterator end);*/

	
	std::string AVGetValue() const { return value; }
	void __IAmAStringWrapper();	
	
private:
	std::string value;

	void construct();


};







}


#endif /* VECTORWRAPPER_H_ */
