#include "StdAfx.h"
#include "Validator.h"

using namespace std;

bool Validate(Result)
{
	ifstream validationFile;
	validationFile.open(validationFileName);
	assert(validationFile.is_open());

	validationFile.close();

	return true;
}

