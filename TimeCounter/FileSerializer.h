#pragma once
#include "framework.h"
#include "TrackedTimeModel.h"

class FileSerializer
{
public:
	FileSerializer() {

	}

	void Serialize(std::list<TrackedTimeModel> value);
	std::list<TrackedTimeModel>* Deserialize();
private:
	char filePath[18] = "spentTimeData.txt";
};

