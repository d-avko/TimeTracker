#include "StackPanel.h"

std::list<DrawablePanel>* StackPanel::GetDrawablePanels()
{
	iconsList->clear();

	GetWindowRect(windowHandle, &thisWindowRect);
	int windowWidth = thisWindowRect.right - thisWindowRect.left;
	int index;
	index = 0;

	for (auto entry : *windowTrackingService->TrackedTime) {
		DrawablePanel panelToDraw;
		DrawableIcon iconToDraw;

		auto icon = (HICON)GetClassLongPtr(entry.Id, GCL_HICON);

		if (icon) {
			iconToDraw.icon = icon;
			iconToDraw.height = iconHeight;
			iconToDraw.width = iconWidth;
			panelToDraw.hasIcon = true;
		}
		else {
			panelToDraw.hasIcon = false;
		}

		//calculate xPos and yPos of icon even if it's not there.

		iconToDraw.yPos = ((panelHeight - iconHeight) / 2) + (index * panelHeight);
		iconToDraw.xPos = ((windowWidth * iconToScreenWidthRatio) - iconWidth) / 2;

		panelToDraw.icon = iconToDraw;
		panelToDraw.yPos = index * panelHeight;
		panelToDraw.xPos = 0;
		panelToDraw.timeSpent = this->GetTimeSpentFormatted(entry.MillisecondsSpent);
		panelToDraw.appName = entry.WindowName;
		panelToDraw.textxPos = iconToDraw.xPos + iconWidth * 4;
		panelToDraw.textyPos = iconToDraw.yPos;
		panelToDraw.text2yPos = iconToDraw.yPos;
		panelToDraw.text2xPos = windowWidth - (windowWidth * timeToScreenWidthRatio);

		++index;

		iconsList->push_back(panelToDraw);
	}

	return iconsList;
}

std::wstring* StackPanel::GetTimeSpentFormatted(long msSpent)
{
	wchar_t* buffer = (wchar_t*)malloc(sizeof(wchar_t));

	if (msSpent < 1000) {
		swprintf(buffer, L"%d ms", msSpent);
		auto result = new std::wstring(buffer);
		return result;
	}

	if (msSpent >= 1000 && msSpent < 1000 * 60) {
		swprintf(buffer, L"%d s", msSpent / 1000);
		auto result = new std::wstring(buffer);
		return result;
	}

	if (msSpent >= 1000 * 60 && msSpent < 1000 * 60 * 60) {
		swprintf(buffer, L"%d m", msSpent / 1000 * 60);
		auto result = new std::wstring(buffer);
		return result;
	}

	if (msSpent >= 1000 * 60 * 60 && msSpent < 1000 * 60 * 60 * 24) {
		swprintf(buffer, L"%d h", msSpent / 1000 * 60 * 60);
		auto result = new std::wstring(buffer);
		return result;
	}
}
