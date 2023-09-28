#include "../../include/Sliding-matching/Sliding-matching.h"

namespace PROJECT_NAME 
{
namespace MOD_NAME 
{
	int matchSequence(std::string* MainSequence, std::string* SlidingSequence)
	{
		const unsigned StrLen = MainSequence->length();
		const unsigned MinMatchLen = MINIMUM_MATCHING_PERCENTAGE * StrLen;

		std::string* MainSequenceStr = new std::string(*MainSequence);
		std::string* SlidingSequenceStr = new std::string(*SlidingSequence);

		

		delete MainSequenceStr;
		delete SlidingSequenceStr;

		return 0;

	}
}
}
