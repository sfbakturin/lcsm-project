#ifndef LCSM_PHYSICAL_STD_GROUND_H
#define LCSM_PHYSICAL_STD_GROUND_H

#include <lcsm/Model/Width.h>
#include <lcsm/Physical/std/Constant.h>

namespace lcsm
{
	namespace physical
	{
		class Ground : public Constant
		{
		  public:
			Ground(Width width);
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_GROUND_H */
