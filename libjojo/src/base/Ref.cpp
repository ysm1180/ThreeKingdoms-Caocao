#include "Ref.h"

namespace jojogame {
	Ref::Ref()
		: _referenceCount(1)
	{
	}
	Ref::~Ref()
	{
	}
	void Ref::release()
	{
		--_referenceCount;

		if (_referenceCount == 0) {
			delete this;
		}
	}
}