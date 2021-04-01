#pragma once

#include "OptionsFile.h"

class OptionsManager
{
public:
	void Reset()
	{
		m_configFile.Reset();
		m_twitchFile.Reset();
		m_bitsFile.Reset();
		m_bitEventsFile.Reset();
	}

	template <typename T>
	inline T GetConfigValue(const std::string& key, T defaultValue)
	{
		return GetOptionValue(m_configFile, key, defaultValue);
	}

	template <typename T>
	inline T GetTwitchValue(const std::string& key, T defaultValue)
	{
		return GetOptionValue(m_twitchFile, key, defaultValue);
	}

	template <typename T>
	inline T GetBitsValue(const std::string& key, T defaultValue)
	{
		return GetOptionValue(m_bitsFile, key, defaultValue);
	}

	template <typename T>
	inline T GetBitEventsValue(const std::string& key, T defaultValue)
	{
		return GetOptionValue(m_bitEventsFile, key, defaultValue);
	}

private:
	OptionsFile m_configFile {"chaosmod/config.ini"};
	OptionsFile m_twitchFile {"chaosmod/twitch.ini"};
	OptionsFile m_bitsFile{"chaosmod/bits.ini"};
	OptionsFile m_bitEventsFile{ "chaosmod/bits_events.ini" };

	template <typename T>
	inline T GetOptionValue(const OptionsFile& optionsFile, const std::string& key, T defaultValue = T())
	{
		if constexpr (std::is_same<std::remove_const<T>::type, std::string>() || std::is_same<std::remove_const<T>::type, char*>())
		{
			return optionsFile.ReadValueString(key, defaultValue);
		}
		else
		{
			return optionsFile.ReadValue(key, defaultValue);
		}
	}
};

inline OptionsManager g_optionsManager;