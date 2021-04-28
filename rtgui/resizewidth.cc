/** -*- C++ -*-
 *  
 *  This file is part of RawTherapee.
 *
 *  Copyright (c) 2018 Alberto Griggio <alberto.griggio@gmail.com>
 *
 *  RawTherapee is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RawTherapee is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RawTherapee.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <cmath>
#include <iomanip>

#include "resizewidth.h"

#include "eventmapper.h"

#include "../rtengine/procparams.h"

using namespace rtengine;
using namespace rtengine::procparams;

ResizeWidth::ResizeWidth(): FoldableToolPanel(this, "resizewidth", M("TP_RESIZE_WIDTH_LABEL"), false, true)
{
    auto m = ProcEventMapper::getInstance();
    EvResizeWidthEnabled = m->newEvent(LUMINANCECURVE, "HISTORY_MSG_RESIZE_WIDTH_ENABLED");
    EvResizeWidthStrength = m->newEvent(LUMINANCECURVE, "HISTORY_MSG_RESIZE_WIDTH_STRENGTH");
    
    strength = Gtk::manage(new Adjuster(M("TP_RESIZE_WIDTH_STRENGTH"), 0., 2., .01, 1.));
    strength->setAdjusterListener(this);
    strength->show();

    pack_start(*strength);
}


void ResizeWidth::read(const ProcParams *pp, const ParamsEdited *pedited)
{
    disableListener();

    if (pedited) {
        strength->setEditedState(pedited->resizewidth.strength ? Edited : UnEdited);
        set_inconsistent(multiImage && !pedited->resizewidth.enabled);
    }

    setEnabled(pp->resizewidth.enabled);
    strength->setValue(pp->resizewidth.strength);

    enableListener();
}


void ResizeWidth::write(ProcParams *pp, ParamsEdited *pedited)
{
    pp->resizewidth.strength = strength->getValue();
    pp->resizewidth.enabled = getEnabled();

    if (pedited) {
        pedited->resizewidth.strength = strength->getEditedState();
        pedited->resizewidth.enabled = !get_inconsistent();
    }
}

void ResizeWidth::setDefaults(const ProcParams *defParams, const ParamsEdited *pedited)
{
    strength->setDefault(defParams->resizewidth.strength);

    if (pedited) {
        strength->setDefaultEditedState(pedited->resizewidth.strength ? Edited : UnEdited);
    } else {
        strength->setDefaultEditedState(Irrelevant);
    }
}


void ResizeWidth::adjusterChanged(Adjuster* a, double newval)
{
    if (listener && getEnabled()) {
        listener->panelChanged(EvResizeWidthStrength, a->getTextValue());
    }
}

void ResizeWidth::enabledChanged ()
{
    if (listener) {
        if (get_inconsistent()) {
            listener->panelChanged(EvResizeWidthEnabled, M("GENERAL_UNCHANGED"));
        } else if (getEnabled()) {
            listener->panelChanged(EvResizeWidthEnabled, M("GENERAL_ENABLED"));
        } else {
            listener->panelChanged(EvResizeWidthEnabled, M("GENERAL_DISABLED"));
        }
    }
}


void ResizeWidth::setBatchMode(bool batchMode)
{
    ToolPanel::setBatchMode(batchMode);

    strength->showEditedCB();
}


void ResizeWidth::setAdjusterBehavior(bool strengthAdd)
{
    strength->setAddMode(strengthAdd);
}

