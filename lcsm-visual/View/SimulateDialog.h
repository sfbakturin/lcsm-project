#ifndef LCSM_VISUAL_VIEW_SIMULATEDIALOG_H
#define LCSM_VISUAL_VIEW_SIMULATEDIALOG_H

#include <lcsm/LCSM.h>
#include <lcsm/LCSMState.h>
#include <lcsm/Model/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Physical/DataBits.h>

class SimulateDialog
{
  public:
	static lcsm::DataBits PutValue(lcsm::width_t width, bool *ok = nullptr);
	static void ShowValue(const lcsm::LCSMState *state, lcsm::Identifier id);
};

#endif /* LCSM_VISUAL_VIEW_SIMULATEDIALOG_H */
