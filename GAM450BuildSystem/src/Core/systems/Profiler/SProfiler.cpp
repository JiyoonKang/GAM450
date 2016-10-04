#include <pch/precompiled.h>
#include "SProfiler.h"

#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <DbgHelp.h>
#include <algorithm>
#include <mmiscapi2.h>

#pragma comment( lib, "dbghelp" )

//GLOBALS EVERYWHERE MUAHAHAHA	
HANDLE g_MainThread;
HANDLE g_CurrProcess;


typedef std::map<std::string, size_t> SampleMap;
typedef std::map<DWORD, PCSTR> ModuleMap;
typedef std::vector<DWORD> AddressList;

struct Sample
{
	std::string name;
	size_t samples;
};

struct UserContext
{
	ModuleMap* moduleMap;
	AddressList* addresses;
};

struct Data
{
	std::string logName;
	SampleMap* sampleMap;
	ModuleMap* moduleMap;
	AddressList* moduleAddresses;
	size_t* currentSampleSize;
	size_t* maxSampleSize;
	size_t* count;
	UserContext* context;
};

SampleMap g_SampleMap;
ModuleMap g_ModuleMap;
AddressList g_ModuleAddresses;

size_t g_CurrentSampleSize;
size_t g_maxSampleSize;
size_t g_Count;

UserContext g_Context;
Data g_Data;




void OutputLog(void* data)
{
	//Open log file for writing
	Data* gData = reinterpret_cast<Data*>(data);
	std::string nextLogFile((gData->logName + std::to_string((*gData->count))) + std::string(".log"));
	std::fstream file(nextLogFile.c_str(), std::ios::out);

	AddressList& modules = *gData->moduleAddresses;
	SampleMap& sampleMap = *gData->sampleMap;
	ModuleMap& moduleMap = *gData->moduleMap;

	std::vector<Sample> samples;
	samples.reserve(100);

	for (auto iterator = sampleMap.begin(); iterator != sampleMap.end(); ++iterator)
	{
		std::string name = iterator->first;
		size_t numSamples = iterator->second;

		if (name.empty() || name == "")
		{
			continue;
		}

		Sample node;
		node.name = name;
		node.samples = numSamples;

		samples.push_back(node);
	}

	auto SortData = [](const Sample& l, const Sample& r)
	{
		return l.samples > r.samples;
	};

	std::sort(samples.begin(), samples.end(), SortData);

	file << "PROJECT RUNTIME PROFILE" << std::endl;
	file << std::endl;


	for (size_t i = 0; i < samples.size(); ++i)
	{
		size_t sampleCount = samples[i].samples;

		float percent = (float(sampleCount) / float(*(gData->maxSampleSize))) * 100.0f;

		file << " " << samples[i].name << " " << percent << "%" << std::endl;
	}

	gData->sampleMap->clear();
}



void CALLBACK TakeSample(UINT timerID, UINT msg, DWORD_PTR user, DWORD_PTR dw1, DWORD_PTR dw2)
{
	SuspendThread(g_MainThread);

	CONTEXT context = {0};
	context.ContextFlags = CONTEXT_i386 | CONTEXT_CONTROL;
	GetThreadContext(g_MainThread, &context);

	DWORD EIP = context.Eip;

	ResumeThread(g_MainThread);

	Data* data = reinterpret_cast<Data*>(user);

	IMAGEHLP_MODULE64 module = { 0 };

	module.SizeOfStruct = sizeof(module);
	BOOL result = SymGetModuleInfo64(g_CurrProcess, EIP, &module);


	struct
	{
		SYMBOL_INFO symbolInfo;
		char buffer[MAX_PATH];
	} symbolData = { 0 };

	symbolData.symbolInfo.SizeOfStruct = sizeof(SYMBOL_INFO);
	symbolData.symbolInfo.MaxNameLen = MAX_PATH;

	DWORD64 symbolOffset = 0;

	result = SymFromAddr(g_CurrProcess, EIP, &symbolOffset, &symbolData.symbolInfo);

	std::string funcName(symbolData.symbolInfo.Name);

	auto iterator = data->sampleMap->find(funcName);

	//If the function name is not registered
	if (iterator == data->sampleMap->end())
	{
		//Register with count of 1
		data->sampleMap->insert(std::pair<std::string, size_t>(funcName, 1));
	}
	else
	{
		//Increment the count of current funciton
		++data->sampleMap->operator[](funcName);
	}

	//Increment the amount of samples taken
	++(*data->currentSampleSize);

	if (*(data->currentSampleSize) >= *(data->maxSampleSize))
	{
		OutputLog(data);
		*data->currentSampleSize = 0;
		++(*data->count);
	}
}

BOOL CALLBACK ModuleEnum(PCSTR moduleName, DWORD64 dllBase, PVOID userContext)
{
	g_ModuleAddresses.push_back(DWORD(dllBase));
	g_ModuleMap.insert(std::pair<DWORD, PCSTR>(DWORD(dllBase), moduleName));
	return TRUE;
}

void StartProfiler(const std::string& logName, size_t numSamples)
{
	g_maxSampleSize = numSamples;
	g_CurrentSampleSize = 0;
	g_Count = 1;

	g_Context.addresses = &g_ModuleAddresses;
	g_Context.moduleMap = &g_ModuleMap;

	//Initializing necessary data struct
	g_Data.logName = logName;
	g_Data.sampleMap = &g_SampleMap;
	g_Data.moduleAddresses = &g_ModuleAddresses;
	g_Data.moduleMap = &g_ModuleMap;
	g_Data.currentSampleSize = &g_CurrentSampleSize;
	g_Data.maxSampleSize = &g_maxSampleSize;
	g_Data.count = &g_Count;
	g_Data.context = &g_Context;
	//Open new thread
	g_CurrProcess = GetCurrentProcess();

	//Initialize symbols
	BOOL result = SymInitialize(g_CurrProcess, NULL, TRUE);

	if (!result)
	{
		__debugbreak();
	}

	result = SymEnumerateModules64(g_CurrProcess, &ModuleEnum, NULL);

	if (!result)
	{
		__debugbreak();
	}

	g_MainThread = OpenThread(
		THREAD_SUSPEND_RESUME |
		THREAD_GET_CONTEXT |
		THREAD_QUERY_INFORMATION,
		0,
		GetCurrentThreadId());

	result = timeSetEvent(1, 1, &TakeSample, DWORD_PTR(&g_Data),
		TIME_PERIODIC | TIME_CALLBACK_FUNCTION | TIME_KILL_SYNCHRONOUS);

	if (!result)
	{
		__debugbreak();
	}
}

