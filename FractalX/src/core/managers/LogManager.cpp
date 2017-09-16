#include <FractalPCH.h>
#include <core\managers\LogManager.h>

namespace fractal
{
	namespace fcore
	{
		bool LogManager::Init ()
		{

			return true;
		}

		bool LogManager::Shutdown ()
		{

			return true;
		}

		void LogManager::SetLogLevel (LOG_TYPES newLevel)
		{
			m_logLevel = newLevel;
		}

		void LogManager::Log (const LOG_TYPES MsgType, const FString& message, const FString& filename, const int& line)
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

		void LogManager::LogInfo (const FString message, const FString& filename, const int& line)
		{
			this->Log (LOG_TYPES::LOG_INFO, message, filename, line);
		}

		void LogManager::LogWarning (const FString message, const FString& filename, const int& line)
		{
			this->Log (LOG_TYPES::LOG_WARNING, message, filename, line);
		}

		void LogManager::LogError (const FString message, const FString& filename, const int& line)
		{
			this->Log (LOG_TYPES::LOG_WARNING, message, filename, line);
		}
	}
}