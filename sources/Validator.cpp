#include "StdAfx.h"
#include "Validator.h"

using namespace std;

bool Validate(const string& validationFileName)
{
	ifstream validationFile;
	validationFile.open(validationFileName);
	assert(validationFile.is_open());

	validationFile.close();

	return true;
}

