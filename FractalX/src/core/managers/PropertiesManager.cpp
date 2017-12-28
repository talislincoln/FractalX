#include <FractalPCH.h>
#include <core\managers\PropertiesManager.h>

#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <regex>

namespace fractal
{
	namespace fcore
	{

		PropertiesManager::PropertiesManager () :
			IManager<PropertiesManager>()
		{
			// empty
		}

		PropertiesManager::~PropertiesManager ()
		{
			// empty
		}

		bool PropertiesManager::Init ()
		{
			std::string s = "../resources/engine.INI";

			std::ifstream infile;
			infile.open (s);

			parse (infile);

			return true;
		}

		void PropertiesManager::Update ()
		{
			// empty
		}

		bool PropertiesManager::Shutdown ()
		{
			return true;
		}

		void PropertiesManager::ReadFile (FString filePath)
		{
			Section currentSection;
			std::list<Section> sections;

			using namespace std;
			std::string str1, str2;
			std::vector<string> vec(10);

			std::ifstream infile;
			infile.open (filePath);

			/*while(infile >> str1)
			{
				TrimString (str1);
				// its a comment - go to next line
				if (str1[0] == '#')
					continue;
				vec.emplace_back (std::move (str1));
			}*/

			for (std::string line; std::getline (infile, line);)
			{
				LeftTrim (line);

				if (line.empty ())
					continue;

				// if a comment
				if (line[0] == ';' || line[0] == '#')
				{
					// allow both ; and # comments at the start of a line
					continue;
				}

				// section
				if (line[0] == '[')
				{
					/* A "[section]" line */
					size_t end = line.find_first_of (']');
					if (end != std::string::npos)
					{

						// this is a new section so if we have a current section populated, add it to list
						if (!currentSection.Name.empty ())
						{
							sections.push_back (currentSection);  // copy
							currentSection.Name.clear ();  // clear section for re-use
							currentSection.KeyValues.clear ();
						}
						currentSection.Name = line.substr (1, end - 1);
					}

					continue;
				}

				if (!line.empty ())
				{
					/* Not a comment, must be a name[=:]value pair */
					size_t end = line.find_first_of ("=");
					if (end != std::string::npos)
					{
						std::string name = line.substr (0, end);
						std::string value = line.substr (end + 1);
						RightTrim (name);
						LeftTrim (value);

						currentSection.KeyValues[name] = value;
					}
				}
			}

			if (!currentSection.Name.empty ())
			{
				sections.push_back (currentSection);  // copy
				currentSection.Name = "";
				currentSection.KeyValues.clear ();
			}
		}

		std::string PropertiesManager::GetValue (const std::string& section, const std::string& key) const
		{
			auto sectionValues = GetSectionValues (section);
			if (sectionValues)
			{
				auto it = sectionValues->find (key);
				if (it != sectionValues->end ())
				{
					return it->second;
				}
			}

			return "";
		}

		const std::unordered_map<std::string, std::string>* PropertiesManager::GetSectionValues (const std::string& name)  const
		{
			const auto it = m_sections.find (name);
			if (it != m_sections.end ())
			{
				return &it->second;
			} else 
			{
				return nullptr;
			}
		}

		void PropertiesManager::LeftTrim (std::string& s)
		{
			s.erase (s.begin (), std::find_if (s.begin (), s.end (), [](int c)
				{
					return !std::isspace (c);
				}));
		}

		void PropertiesManager::RightTrim (std::string& s)
		{
			size_t endpos = s.find_last_not_of (" \t\r\n\v\f");
			if (std::string::npos != endpos)
			{
				s = s.substr (0, endpos + 1);
			}
		}

		void PropertiesManager::parse (std::istream& in)
		{
			static const std::regex comment_regex{ R"x(\s*[;#])x" };
			static const std::regex section_regex{ R"x(\s*\[([^\]]+)\])x" };
			static const std::regex value_regex{ R"x(\s*(\S[^ \t=]*)\s*=\s*((\s?\S+)+)\s*$)x" };
			std::string current_section;
			std::smatch pieces;
			for (std::string line; std::getline (in, line);)
			{
				if (line.empty () || std::regex_match (line, pieces, comment_regex))
				{
					// skip comment lines and blank lines                    
				}
				else if (std::regex_match (line, pieces, section_regex))
				{
					if (pieces.size () == 2)
					{ // exactly one match
						current_section = pieces[1].str ();
					}
				}
				else if (std::regex_match (line, pieces, value_regex))
				{
					if (pieces.size () == 4)
					{ // exactly enough matches
						m_sections[current_section][pieces[1].str ()] = pieces[2].str ();
					}
				}
			}
		}
	}
}