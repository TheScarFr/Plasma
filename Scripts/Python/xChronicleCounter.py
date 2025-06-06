# /*==LICENSE==*
#
# CyanWorlds.com Engine - MMOG client, server and tools
# Copyright (C) 2011  Cyan Worlds, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Additional permissions under GNU GPL version 3 section 7
#
# If you modify this Program, or any covered work, by linking or
# combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
# NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
# JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
# (or a modified version of those libraries),
# containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
# PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
# JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
# licensors of this Program grant you additional
# permission to convey the resulting work. Corresponding Source for a
# non-source form of such a combination shall include the source code for
# the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
# work.
#
# You can contact Cyan Worlds, Inc. by email legal@cyan.com
#  or by snail mail at:
#       Cyan Worlds, Inc.
#       14617 N Newport Hwy
#       Mead, WA   99021
#
# *==LICENSE==*/
"""Module: xChronicleCounter
Age: global
Date: October 2002
Author: Bill Slease
simple incremental counter stored in player's chronicle
"""

from Plasma import *
from PlasmaTypes import *

# define the attributes that will be entered in max
act = ptAttribActivator(1, "activator")
var = ptAttribString(2,"chronicle var")

# global variables
kChronicleVarType = 0  # not currently used
kInitialValue = 1

class xChronicleCounter(ptResponder):

    def __init__(self):
        ptResponder.__init__(self)
        self.id = 5021
        self.version = 1

    def OnFirstUpdate(self):
        pass
 
    def Load(self):
        pass
        

    def OnNotify(self,state,id,events):
        if not state:
            return
            
        if not PtWasLocallyNotified(self.key):
            return
        
        if id == act.id:
            if var.value is None:
                PtDebugPrint("xChronicleCounter.py:\t-- ERROR: missing chronicle variable name, can't record --")
                return
            
            vault = ptVault()
            entry = vault.findChronicleEntry(var.value)
            if entry is None:
                # not found... add current level chronicle
                vault.addChronicleEntry(var.value,kChronicleVarType,"%d" %(kInitialValue))
                PtDebugPrint("xChronicleCounter:\tentered new chronicle counter %s, count is %d" % (var.value,kInitialValue))
            else:
                count = int(entry.getValue())
                count = count + 1
                entry.setValue("%d" % (count))
                entry.save()
                PtDebugPrint("xChronicleCounter:\tyour current count for %s is %s" % (var.value,entry.getValue()))
