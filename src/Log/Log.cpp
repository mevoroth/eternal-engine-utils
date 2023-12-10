#include "Log/Log.hpp"

namespace Eternal
{
	namespace LogSystem
	{
		const Log::LogLevel Log::Info(0, "info");
		const Log::LogLevel Log::Warning(1, "warning");
		const Log::LogLevel Log::Error(2, "error");
		const Log::LogLevel Log::Critical(3, "critical");

		const Log::LogCategory Log::Graphics("Graphics");
		const Log::LogCategory Log::Shaders("Shaders");
		const Log::LogCategory Log::Engine("Engine");
		const Log::LogCategory Log::Memory("Memory");
		const Log::LogCategory Log::Save("Save");
		const Log::LogCategory Log::Import("Import");
		const Log::LogCategory Log::Components("Components");

		Log* Log::_Instance = nullptr;

		Log::Log()
		{
			_InitialTime = Eternal::Time::Timer::Get()->GetTimeMicroSeconds();
		}

		void Log::Initialize(_In_ Log* InLog)
		{
			ETERNAL_ASSERT(!_Instance);
			ETERNAL_ASSERT(InLog);
			_Instance = InLog;
		}

		Log* Log::Get()
		{
			ETERNAL_ASSERT(_Instance);
			return _Instance;
		}

		TimeMicroSecondsT Log::GetElaspedTime() const
		{
			return Eternal::Time::Timer::Get()->GetTimeMicroSeconds() - _InitialTime;
		}
	}
	
	void LogWrite(_In_ const LogSystem::Log::LogLevel& InLevel, _In_ const LogSystem::Log::LogCategory& InCategory, _In_ const string& InMessage)
	{
		if (LogSystem::Log::Get())
			LogSystem::Log::Get()->Write(InLevel, InCategory, InMessage);
	}

	
	const LogSystem::Log::LogLevel& LogInfo				= LogSystem::Log::Info;
	const LogSystem::Log::LogLevel& LogWarning			= LogSystem::Log::Warning;
	const LogSystem::Log::LogLevel& LogError			= LogSystem::Log::Error;
	const LogSystem::Log::LogLevel& LogCritical			= LogSystem::Log::Critical;

	const LogSystem::Log::LogCategory& LogGraphics		= LogSystem::Log::Graphics;
	const LogSystem::Log::LogCategory& LogShaders		= LogSystem::Log::Shaders;
	const LogSystem::Log::LogCategory& LogEngine		= LogSystem::Log::Engine;
	const LogSystem::Log::LogCategory& LogMemory		= LogSystem::Log::Memory;
	const LogSystem::Log::LogCategory& LogSave			= LogSystem::Log::Save;
	const LogSystem::Log::LogCategory& LogImport		= LogSystem::Log::Import;
	const LogSystem::Log::LogCategory& LogComponents	= LogSystem::Log::Components;
}
