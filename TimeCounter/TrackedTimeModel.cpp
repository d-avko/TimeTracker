#include "TrackedTimeModel.h"

bool TrackedTimeModel::operator==(const TrackedTimeModel& other) const {
	return !(WindowName->compare(*other.WindowName));
}
