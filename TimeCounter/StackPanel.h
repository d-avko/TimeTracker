#pragma once
#include "TrackedTimeModel.h"
#include "WindowTrackingService.h"
#include "framework.h"

struct DrawableIcon {
	HICON icon;
	int width;
	int height;
	int xPos;
	int yPos;
};

struct DrawablePanel {
	std::wstring* appName;
	std::wstring* timeSpent;
	DrawableIcon icon;
	bool hasIcon;
	int textxPos;
	int textyPos;
	int text2xPos;
	int text2yPos;
	int xPos;
	int yPos;
};

class StackPanel {
public:
	StackPanel(WindowTrackingService* windowTrackingService, HWND windowHandle, int panelHeight = 100) {
		this->windowTrackingService = windowTrackingService;
		this->windowHandle = windowHandle;
		this->panelHeight = panelHeight;
		this->iconsList = new std::list<DrawablePanel>();
	}

	std::list<DrawablePanel>* GetDrawablePanels();
private:
	std::wstring* GetTimeSpentFormatted(long msSpent);

	RECT thisWindowRect;
	float iconToScreenWidthRatio = (float)1 / (float)5;
	float textToScreenWidthRatio = (float)3 / (float)5;
	float timeToScreenWidthRatio = (float)1 / (float)5;
	int iconWidth = 50;
	int iconHeight = 50;
	std::list<DrawablePanel>* iconsList;
	WindowTrackingService* windowTrackingService;
	HWND windowHandle;
	int panelHeight;
};