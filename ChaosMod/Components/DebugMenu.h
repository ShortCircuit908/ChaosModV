#pragma once

#include "Component.h"
#include "Effects/EffectIdentifier.h"

#include <vector>

#define _NODISCARD [[nodiscard]]

using DWORD = unsigned long;

class DebugMenu : public Component
{
private:
	struct DebugEffect
	{
		DebugEffect(const EffectIdentifier& effectIdentifier, const std::string& szEffectName) : m_EffectIdentifier(effectIdentifier), m_szEffectName(szEffectName)
		{

		}

		EffectIdentifier m_EffectIdentifier;
		std::string m_szEffectName;
	};

	std::vector<DebugEffect> m_rgEffects;

	bool m_bIsEnabled;

	int m_iSelectedIdx = 0;
	bool m_bVisible = false;

	DWORD m_ulRepeatTime = 0;
	bool m_bDispatchEffect = false;

public:
	DebugMenu();

	virtual void Run() override;

	bool _NODISCARD IsEnabled() const;

	void HandleInput(DWORD ulKey, bool bOnRepeat);

	void SetVisible(bool bState);
	bool _NODISCARD IsVisible() const;
};