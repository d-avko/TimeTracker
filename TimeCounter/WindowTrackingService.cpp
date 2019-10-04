#include "WindowTrackingService.h"
#include "framework.h"
#include "Resource.h"

void WindowTrackingService::OnWindowChanged(std::wstring* name, HWND id)
{
	auto valueToFind = new TrackedTimeModel(id, name);
	auto entry = std::find(TrackedTime->begin(), TrackedTime->end(), *valueToFind);

	if (entry == TrackedTime->end()) {
		TrackedTime->push_back(*valueToFind);
	}
	else {
		//app could be reopened.
		entry->Id = id;
	}

	///idk about that
	delete CurrentWindow;
	CurrentWindow = valueToFind;
}

void WindowTrackingService::OnDeserialization(std::list<TrackedTimeModel>* value)
{
	if (value == nullptr) {
		return;
	}

	TrackedTime = value;
}

void WindowTrackingService::OnTimerTick(int tickLengthMs)
{
	if (CurrentWindow == nullptr) {
		return;
	}
	auto entry = std::find(TrackedTime->begin(), TrackedTime->end(), *CurrentWindow);

	if (entry == TrackedTime->end()) {
		return;
	}

	entry->MillisecondsSpent += tickLengthMs;
}
