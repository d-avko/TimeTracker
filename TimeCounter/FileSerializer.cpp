#include "FileSerializer.h"

void FileSerializer::Serialize(std::list<TrackedTimeModel> value)
{
	auto file = fopen(filePath, "w");

	if (file == NULL) {
		return;
	}

	for (auto entry : value) {
		fwprintf(file, L"%s %d\n", entry.WindowName->c_str(), entry.MillisecondsSpent);
	}

	fclose(file);
}

std::list<TrackedTimeModel>* FileSerializer::Deserialize()
{
	auto file = fopen(filePath, "r");

	if (file == NULL) {
		return nullptr;
	}

	auto res = new std::list<TrackedTimeModel>();

	while (true) {
		auto entry = new TrackedTimeModel(NULL);
		auto buffer = (wchar_t*)malloc(256);

		if (fwscanf(file, L"%s %d\n", buffer, &entry->MillisecondsSpent) == -1) {
			break;
		}

		entry->WindowName = new std::wstring(buffer);
		res->push_back(*entry);
	}
	fclose(file);
	return res;
}
