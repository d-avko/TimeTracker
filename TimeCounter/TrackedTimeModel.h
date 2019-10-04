#pragma once
#include "framework.h"

class TrackedTimeModel
{
public:
	TrackedTimeModel(HWND Id) {
		this->Id = Id;
		this->MillisecondsSpent = 0;
		this->WindowName = new std::wstring();
	}
	TrackedTimeModel(HWND Id, std::wstring* WindowName) {
		this->Id = Id;
		this->WindowName = WindowName;
		this->MillisecondsSpent = 0;
	}

	HWND Id;
	std::wstring* WindowName;
	long MillisecondsSpent;

	bool operator==(const TrackedTimeModel& other) const;
};