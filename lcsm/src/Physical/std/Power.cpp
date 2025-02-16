#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/std/Constant.h>
#include <lcsm/Physical/std/Power.h>
#include <lcsm/Verilog/Bit.h>

lcsm::physical::Power::Power(lcsm::object_type_t objectType, lcsm::Width width) :
	lcsm::physical::Constant(objectType, lcsm::DataBits(width, lcsm::verilog::Bit::True))
{
}
