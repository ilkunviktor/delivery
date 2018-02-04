#include "StdAfx.h"
#include "Validation.h"

using namespace std;

bool Validate(const std::string& validationFileName)
{
	ifstream validationFile;
	validationFile.open(validationFileName);
	assert(validationFile.is_open());

	validationFile.close();

	return true;
}

