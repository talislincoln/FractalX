#include <FractalPCH.h>
#include <core\managers\Logger.h>

namespace fractal
{
	namespace fcore
	{
		bool Logger::Init ()
		{

			return true;
		}

		bool Logger::Shutdown ()
		{

			return true;
		}

		void Logger::SetLogLevel (LOG_TYPES newLevel)
		{
			m_logLevel = newLevel;
		}

		void Logger::Log (const LOG_TYPES MsgType, const FString& message, const FString& filename, const int& line)
		{
			if (MsgType >= m_logLevel)
			{
				switch (MsgType)
				{
				case LOG_TYPES::LOG_INFO:
					fcout << "INFO: " << message << " at " << filename << " line " << line << std::endl;
					break;
				case LOG_TYPES::LOG_WARNING:
					fcout << "WARNING: " << message << " at " << filename << " line " << line << std::endl;
					break;
				case LOG_TYPES::LOG_ERROR:
					fcout << "ERROR: " << message << " at " << filename << " line " << line << std::endl;
					break;
				default:
					break;
				}
			}
		}

		void Logger::LogInfo (const FString message)
		{
			this->Log (LOG_TYPES::LOG_INFO, message, _TEXT(__FILE__), __LINE__);
		}

		void Logger::LogWarning (const FString message)
		{
			this->Log (LOG_TYPES::LOG_WARNING, message, _TEXT (__FILE__), __LINE__);
		}

		void Logger::LogError (const FString message)
		{
			this->Log (LOG_TYPES::LOG_WARNING, message, _TEXT (__FILE__), __LINE__);
		}
	}
}