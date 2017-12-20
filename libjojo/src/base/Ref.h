#pragma once

namespace jojogame {
	class Ref {
	public:
		Ref();
		virtual ~Ref();

		void release();

	protected:
		unsigned int _referenceCount;
	};
}