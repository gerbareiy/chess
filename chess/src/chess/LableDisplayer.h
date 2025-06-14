#pragma once

#include "logic/Inputer.h"

#include <string>

namespace Chess
{
	class LableDisplayer
	{
	public:
		LableDisplayer(std::shared_ptr<Inputer> const& Inputer);

	public:
		void Show(std::string const& toPrint) const;
	};
}
