#pragma once
#ifndef _PROPERTIESMANAGER_H
#define _PROPERTIESMANAGER_H

#include "IManager.h"
#include <unordered_map>

namespace fractal
{
	namespace fcore
	{
		struct Section
		{
			std::string Name;
			std::unordered_map<std::string, std::string> KeyValues;
		};

		class PropertiesManager : public IManager<PropertiesManager>
		{
		private:
			std::unordered_map<std::string, float> m_properties;
			std::vector<Section> m_sections;

			std::unordered_map< std::string, std::unordered_map<std::string, std::string> > sections;
		public:
			PropertiesManager ();
			~PropertiesManager ();

			bool Init () override;
			bool Shutdown () override;
			void Update () override;

			static void ReadFile (FString filePath);
			std::string GetValue (const std::string& section, const std::string& key) const;

		private:
			const Section* GetSection (const std::string& sectionName) const;

			static void LeftTrim (std::string& s);
			static void RightTrim (std::string& s);

			void parse (std::istream& in);
		};
	}
}

#endif // !_PROPERTIESMANAGER_H
