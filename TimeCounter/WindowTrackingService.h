#pragma once
#include "framework.h"
#include "Resource.h"
#include "TrackedTimeModel.h"

class WindowTrackingService
{
public:
	WindowTrackingService() {
		TrackedTime = new std::list<TrackedTimeModel>();
		tempModel = new TrackedTimeModel(0);
	}

	void OnWindowChanged(std::wstring* name, HWND id);
	void OnDeserialization(std::list<TrackedTimeModel>* value);
	void OnTimerTick(int tickLengthMs);
	std::list<TrackedTimeModel>* TrackedTime;
private:
	TrackedTimeModel* CurrentWindow = nullptr;
	TrackedTimeModel* tempModel;
};
