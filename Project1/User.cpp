#include "User.h"

using namespace System;

int User::CheckPassword(String ^input) {
	return input->CompareTo(this->password)==0;
}
