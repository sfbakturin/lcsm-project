#ifndef LCSM_PHYSICAL_STD_GROUND_H
#define LCSM_PHYSICAL_STD_GROUND_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/std/Constant.h>

namespace lcsm
{
	namespace physical
	{
		class Ground : public Constant
		{
		  public:
			Ground(object_type_t objectType, Width width);
		};
	}	 // namespace physical
}	 // namespace lcsm

#endif /* LCSM_PHYSICAL_STD_GROUND_H */
