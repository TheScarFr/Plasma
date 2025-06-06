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

from __future__ import annotations

from Plasma import *

import abc
from typing import NamedTuple, TYPE_CHECKING

if TYPE_CHECKING:
    from .xMarkerMgr import MarkerGameManager


class xMarker(NamedTuple):
    idx: int
    age: str
    pos: ptVector3
    desc: str


class MarkerGameBrain(abc.ABC):
    """ABC for marker games"""

    @abc.abstractmethod
    def CaptureMarker(self, idx: int):
        ...

    @abc.abstractmethod
    def Cleanup(self) -> int:
        ...

    @abc.abstractmethod
    def FlushMarkers(self) -> None:
        ...

    @abc.abstractproperty
    def game_id(self) -> str:
        ...

    @abc.abstractproperty
    def game_name(self) -> str:
        ...

    @abc.abstractproperty
    def game_type(self) -> int:
        ...

    @abc.abstractproperty
    def is_loaded(self) -> bool:
        return False

    @abc.abstractmethod
    def IsMarkerCaptured(self, idx: int) -> bool:
        """Returns if the marker has been captured"""
        ...

    @abc.abstractclassmethod
    def LoadFromVault(cls, mgr: MarkerGameManager) -> MarkerGameBrain:
        ...

    @abc.abstractproperty
    def marker_colors(self) -> Tuple[str, str]:
        """Returns a tuple of colors to use in the KI (captured, need_capture)"""
        pass

    @abc.abstractproperty
    def marker_total(self) -> int:
        """Returns the total number of markers"""
        ...

    @abc.abstractproperty
    def markers(self) -> Iterator[xMarker]:
        """Returns all markers as (id, age, position, description)"""
        ...

    @abc.abstractproperty
    def markers_captured(self) -> Iterator[xMarker]:
        """Returns all markers captured"""
        ...

    @abc.abstractproperty
    def markers_visible(self) -> bool:
        """Returns whether or not game markers are being shown."""
        ...

    @abc.abstractproperty
    def num_markers_captured(self) -> int:
        """Returns the total number of markers captured"""
        ...

    def Play(self) -> None:
        ...

    @abc.abstractproperty
    def playing(self) -> bool:
        ...

    @abc.abstractmethod
    def RefreshMarkers(self) -> None:
        ...
