#include <lcsm/LCSM.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/std/Constant.h>
#include <lcsm/Physical/std/Ground.h>
#include <lcsm/Verilog/Bit.h>

lcsm::physical::Ground::Ground(lcsm::ObjectType objectType, lcsm::Width width) :
	lcsm::physical::Constant(objectType, lcsm::DataBits(width, lcsm::verilog::Bit::False))
{
}
