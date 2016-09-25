#include "Log/Log.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::Log;

const Log::LogLevel Log::Info(0, "info");
const Log::LogLevel Log::Warning(1, "warning");
const Log::LogLevel Log::Error(2, "error");
const Log::LogLevel Log::Critical(3, "critical");

const Log::LogCategory Log::Engine("Engine");
const Log::LogCategory Log::Save("Save");
const Log::LogCategory Log::Import("Import");

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
