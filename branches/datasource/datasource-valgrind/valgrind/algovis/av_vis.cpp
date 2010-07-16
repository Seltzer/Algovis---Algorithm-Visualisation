#define CPP_DEFINE
#include "av_vis.h"

extern "C"
{
   #include "pub_tool_basics.h"
   #include "pub_tool_libcbase.h"
   #include "pub_tool_libcprint.h"
   #include "pub_tool_mallocfree.h"
   
   void* memmove(void* a, void* b, int c)
   {
	  return VG_(memmove)(a, b, c);
   }
   //#define operator new(a)
   void operator delete(void*) { /* TODO */ }
   void* operator new(unsigned len)
   {
	  return VG_(malloc)("av.vis.on.1", len);
   }
}

//#include <map>
//#include <string>
#include <vector>
//#undef memmove

void drawIntVector(void* data); // fwd dcl
void drawDoubleVector(void* data);

std::vector<const char*>* knownTypesPtr;
std::vector<drawFuncType>* drawFuncsPtr;

std::vector<const char*>& getKnownTypes()
{
   if (knownTypesPtr == NULL)
	  knownTypesPtr = new std::vector<const char*>;
   return *knownTypesPtr;
}

std::vector<drawFuncType>& getDrawFuncs()
{
   if (drawFuncsPtr == NULL)
	  drawFuncsPtr = new std::vector<drawFuncType>;
   return *drawFuncsPtr;
}

void registerDrawFunc(const char* typeName, drawFuncType drawFunc)
{
   std::vector<const char*>& knownTypes = getKnownTypes();
   std::vector<drawFuncType>& drawFuncs = getDrawFuncs();
   
   knownTypes.push_back((char*)VG_(strdup)("av.vis.rdf.1", (const Char*)typeName));
   drawFuncs.push_back(drawFunc);
}

void registerDrawFuncs(void)
{
   VG_(printf)("Registering draw functions\n");
   registerDrawFunc("int", &drawIntVector);
   registerDrawFunc("double", &drawDoubleVector);
}

int getTypeId(char* name)
{
   VG_(printf)("Getting type %s\n", name);
   //std::string cppName(name);
   // trim trailing spaces
   //size_t end = cppName.find_last_not_of(' ');
   //if (end != std::string::npos)
	//  cppName = cppName.substr(0, end+1);
   std::vector<const char*>& knownTypes = getKnownTypes();
   int id = 0;
   for (; id < knownTypes.size(); id++)
	  if (VG_(strcmp)((const Char*)knownTypes[id], (const Char*)name) == 0)
		 break;
   if (id == knownTypes.size())
   {
	  VG_(printf)("Type not found!\n");
	  return -1;
   }
   return id;
}

drawFuncType getDrawFunc(int typeId)
{
   VG_(printf)("Draw func for type %d\n", typeId);
   std::vector<drawFuncType>& drawFuncs = getDrawFuncs();
   if (typeId < 0 || typeId >= drawFuncs.size())
   {
	  VG_(printf)("Invalid typeId!\n");
	  return NULL;
   }
   return drawFuncs[typeId];
}

void drawIntVector(void* data)
{
    const std::vector<int>& vec = *((std::vector<int>*)data);
	VG_(printf)("Vector: %p\n", data);
	for (int i = 0; i < vec.size(); i++)
	{
	   VG_(printf)("%d\n", vec[i]);
	}
}

void drawDoubleVector(void* data)
{
    const std::vector<double>& vec = *((std::vector<double>*)data);
	VG_(printf)("Vector: %p\n", data);
	for (int i = 0; i < vec.size(); i++)
	{
	   VG_(printf)("%lf\n", vec[i]);
	}
}
