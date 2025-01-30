#ifndef LCSM_PHYSICAL_STD_POWER_H
#define LCSM_PHYSICAL_STD_POWER_H

#include <lcsm/Model/Width.h>
#include <lcsm/Physical/std/Constant.h>

namespace lcsm
{
	namespace physical
	{
		class Power : public Constant
		{
		  public:
			Power(Width width);
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_POWER_H */
