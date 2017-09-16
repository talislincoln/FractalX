#pragma once
#ifndef _LOGGER_H
#define _LOGGER_H

#include "IManager.h"

namespace fractal
{
	enum LOG_TYPES : unsigned __int8
	{
		LOG_INFO = 0,
		LOG_WARNING,
		LOG_ERROR
	};

	namespace fcore
	{
		class LogManager : public IManager<LogManager>
		{
		private:
			LOG_TYPES m_logLevel;

		public:

			LogManager () : IManager<LogManager> (), m_logLevel(LOG_TYPES::LOG_INFO) { /* empty */ }
			~LogManager () { /* empty */ }

			bool Init () override;
			bool Shutdown () override;
			void Update () override {};

			void SetLogLevel (LOG_TYPES newLevel);

			void Log (const LOG_TYPES MsgType, const FString& message, const FString& filename = _TEXT(__FILE__), const int& line = __LINE__);

			void LogInfo (const FString message, const FString& filename = _TEXT (__FILE__), const int& line = __LINE__);
			void LogWarning (const FString message, const FString& filename = _TEXT (__FILE__), const int& line = __LINE__);
			void LogError (const FString message, const FString& filename = _TEXT (__FILE__), const int& line = __LINE__);

			LogManager (const LogManager&) = delete;
			LogManager (LogManager&&) = delete;
		};
	}
}

#endif