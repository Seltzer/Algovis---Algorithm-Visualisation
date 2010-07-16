#ifndef AV_VIS_H
#define AV_VIS_H

#ifdef CPP_DEFINE

extern "C"
{

#endif //CPP_DEFINE

void registerDrawFuncs(void);

int getTypeId(char* name);

typedef void (*drawFuncType)(void*);

drawFuncType getDrawFunc(int typeId);

//void drawVector(void* data);

#ifdef CPP_DEFINE

}

#endif //CPP_DEFINE

#endif //AV_VIS_H
