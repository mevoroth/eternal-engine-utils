#include "Log/Log.hpp"

using namespace Eternal::Log;

const Log::LogLevel Log::Info(0, "info");
const Log::LogLevel Log::Warning(1, "warning");
const Log::LogLevel Log::Error(2, "error");
const Log::LogLevel Log::Critical(3, "critical");

const Log::LogCategory Log::Engine("Engine");
const Log::LogCategory Log::Save("Save");
const Log::LogCategory Log::Import("Import");
const Log::LogCategory Log::Graphics("Graphics");

Log* Log::_Inst = nullptr;

Log::Log()
{
	_InitialTime = Eternal::Time::Time::Get()->GetTimeMicroSeconds();
}

void Log::Initialize(Log* LogObj)
{
	ETERNAL_ASSERT(!_Inst);
	ETERNAL_ASSERT(LogObj);
	_Inst = LogObj;
}

Log* Log::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

TimeMicroSecondsT Log::GetElaspedTime() const
{
	return Eternal::Time::Time::Get()->GetTimeMicroSeconds() - _InitialTime;
}

namespace Eternal
{
	void LogWrite(_In_ const Log::Log::LogLevel& Level, _In_ const Log::Log::LogCategory& Category, _In_ const string& Message)
	{
		if (Log::Log::Get()) // REMOVE
			Log::Log::Get()->Write(Level, Category, Message);
	}

	
	const Log::Log::LogLevel& LogInfo		= Log::Log::Info;
	const Log::Log::LogLevel& LogWarning	= Log::Log::Warning;
	const Log::Log::LogLevel& LogError		= Log::Log::Error;
	const Log::Log::LogLevel& LogCritical	= Log::Log::Critical;

	const Log::Log::LogCategory& LogGraphics	= Log::Log::Graphics;
	const Log::Log::LogCategory& LogEngine		= Log::Log::Engine;
	const Log::Log::LogCategory& LogSave		= Log::Log::Save;
	const Log::Log::LogCategory& LogImport		= Log::Log::Import;
}
