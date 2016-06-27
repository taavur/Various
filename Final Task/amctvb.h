unsigned int __stdcall InitCommunication(unsigned short& pnChannels);
unsigned int __stdcall StopCommunication();
unsigned int __stdcall ParamReadReal(unsigned short nChannel, unsigned short nNode, unsigned short nGroup, unsigned short nIndex, float& pfValue);
