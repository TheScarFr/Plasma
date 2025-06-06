/*==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011  Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

*==LICENSE==*/

#include "HeadSpin.h"

#include "plComponent.h"
#include "plComponentReg.h"
#include "plMiscComponents.h"
#include "MaxMain/plMaxNode.h"
#include "MaxMain/MaxAPI.h"

#include "resource.h"

#include "MaxMain/plPlasmaRefMsgs.h"

#include "pnSceneObject/plSceneObject.h"
#include "pnSceneObject/plCoordinateInterface.h"
#include "pnSceneObject/plDrawInterface.h"

#include "plMessage/plSimStateMsg.h"
#include "pnMessage/plEnableMsg.h"

#include "MaxMain/plPluginResManager.h"

void DummyCodeIncludeFuncIgnore() {}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Ignore Component
//
//

//Class that accesses the paramblock below.
class plIgnoreComponent : public plComponent
{
public:
    plIgnoreComponent();

    // SetupProperties - Internal setup and write-only set properties on the MaxNode. No reading
    // of properties on the MaxNode, as it's still indeterminant.
    bool SetupProperties(plMaxNode *pNode, plErrorMsg *pErrMsg) override;
    bool Convert(plMaxNode *node, plErrorMsg *pErrMsg) override;

    void CollectNonDrawables(INodeTab& nonDrawables) override;
};

//Max desc stuff necessary below.
CLASS_DESC(plIgnoreComponent, gIgnoreDesc, "Ignore",  "Ignore", COMP_TYPE_IGNORE, Class_ID(0x48326288, 0x528a3dea))

enum
{
    kIgnoreMeCheckBx
};

ParamBlockDesc2 gIgnoreBk
(
    plComponent::kBlkComp, _T("Ignore"), 0, &gIgnoreDesc, P_AUTO_CONSTRUCT + P_AUTO_UI, plComponent::kRefComp,

    IDD_COMP_IGNORE, IDS_COMP_IGNORES, 0, 0, nullptr,

    kIgnoreMeCheckBx,  _T("Ignore"), TYPE_BOOL,         0, 0,
        p_default,  TRUE,
        p_ui,   TYPE_SINGLECHEKBOX, IDC_COMP_IGNORE_CKBX,
        p_end,

    p_end
);

plIgnoreComponent::plIgnoreComponent()
{
    fClassDesc = &gIgnoreDesc;
    fClassDesc->MakeAutoParamBlocks(this);
}

void plIgnoreComponent::CollectNonDrawables(INodeTab& nonDrawables)
{
    if (fCompPB->GetInt(kIgnoreMeCheckBx))
    {
        AddTargetsToList(nonDrawables);
    }
}

// SetupProperties - Internal setup and write-only set properties on the MaxNode. No reading
// of properties on the MaxNode, as it's still indeterminant.
bool plIgnoreComponent::SetupProperties(plMaxNode *pNode,  plErrorMsg *pErrMsg)
{
    if (fCompPB->GetInt(kIgnoreMeCheckBx))
        pNode->SetCanConvert(false);

    return true;
}

bool plIgnoreComponent::Convert(plMaxNode *node, plErrorMsg *pErrMsg)
{
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  IgnoreLite Component
//
//

//Class that accesses the paramblock below.
class plIgnoreLiteComponent : public plComponent
{
public:
    enum {
        kSelectedOnly
    };
    enum LightState {
        kTurnOn,
        kTurnOff,
        kToggle
    };
public:
    plIgnoreLiteComponent();

    void SetState(LightState s);

    // SetupProperties - Internal setup and write-only set properties on the MaxNode. No reading
    // of properties on the MaxNode, as it's still indeterminant.
    bool SetupProperties(plMaxNode *pNode, plErrorMsg *pErrMsg) override { return true; }

    bool PreConvert(plMaxNode *pNode, plErrorMsg *pErrMsg) override { return true; }
    bool Convert(plMaxNode *node, plErrorMsg *pErrMsg) override { return true; }
};


class plIgnoreLiteProc : public ParamMap2UserDlgProc
{
public:
    INT_PTR DlgProc(TimeValue t, IParamMap2 *map, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override
    {
        switch (msg)
        {

        case WM_COMMAND:
            if( (HIWORD(wParam) == BN_CLICKED) && (LOWORD(wParam) == IDC_COMP_IGNORELITE_ON) )
            {
                plIgnoreLiteComponent* ilc = (plIgnoreLiteComponent*)map->GetParamBlock()->GetOwner();
                ilc->SetState(plIgnoreLiteComponent::kTurnOn);

                return TRUE;
            }
            if( (HIWORD(wParam) == BN_CLICKED) && (LOWORD(wParam) == IDC_COMP_IGNORELITE_OFF) )
            {
                plIgnoreLiteComponent* ilc = (plIgnoreLiteComponent*)map->GetParamBlock()->GetOwner();
                ilc->SetState(plIgnoreLiteComponent::kTurnOff);

                return TRUE;
            }
            if( (HIWORD(wParam) == BN_CLICKED) && (LOWORD(wParam) == IDC_COMP_IGNORELITE_TOGGLE) )
            {
                plIgnoreLiteComponent* ilc = (plIgnoreLiteComponent*)map->GetParamBlock()->GetOwner();
                ilc->SetState(plIgnoreLiteComponent::kToggle);

                return TRUE;
            }
            break;
        }

        return FALSE;
    }
    void DeleteThis() override { }
};
static plIgnoreLiteProc gIgnoreLiteProc;


//Max desc stuff necessary below.
CLASS_DESC(plIgnoreLiteComponent, gIgnoreLiteDesc, "Control Max Light", "ControlLite", COMP_TYPE_IGNORE, IGNORELITE_CID)

ParamBlockDesc2 gIgnoreLiteBk
(   
 plComponent::kBlkComp, _T("IgnoreLite"), 0, &gIgnoreLiteDesc, P_AUTO_CONSTRUCT + P_AUTO_UI, plComponent::kRefComp,

    IDD_COMP_IGNORELITE, IDS_COMP_IGNORELITES, 0, 0, &gIgnoreLiteProc,

    plIgnoreLiteComponent::kSelectedOnly,  _T("SelectedOnly"), TYPE_BOOL,       0, 0,
        p_default,  FALSE,
        p_ui,   TYPE_SINGLECHEKBOX, IDC_COMP_IGNORELITE_SELECTED,
        p_end,

    p_end
);

plIgnoreLiteComponent::plIgnoreLiteComponent()
{
    fClassDesc = &gIgnoreLiteDesc;
    fClassDesc->MakeAutoParamBlocks(this);
}

void plIgnoreLiteComponent::SetState(LightState s)
{
    BOOL selectedOnly = fCompPB->GetInt(kSelectedOnly);

    int numTarg = NumTargets();
    int i;
    for( i = 0; i < numTarg; i++ )
    {
        plMaxNodeBase* targ = GetTarget(i);
        if( targ )
        {
            if( selectedOnly && !targ->Selected() )
                continue;

            Object *obj = targ->EvalWorldState(TimeValue(0)).obj;
            if (obj && (obj->SuperClassID() == SClass_ID(LIGHT_CLASS_ID))) 
            {
                LightObject* liObj = (LightObject*)obj;
                switch( s )
                {
                case kTurnOn:
                    liObj->SetUseLight(true);
                    break;
                case kTurnOff:
                    liObj->SetUseLight(false);
                    break;
                case kToggle:
                    liObj->SetUseLight(!liObj->GetUseLight());
                    break;
                }
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Barney Component
//
//

//Class that accesses the paramblock below.
class plBarneyComponent : public plComponent
{
public:
    plBarneyComponent();

    // SetupProperties - Internal setup and write-only set properties on the MaxNode. No reading
    // of properties on the MaxNode, as it's still indeterminant.
    bool SetupProperties(plMaxNode *pNode, plErrorMsg *pErrMsg) override;
    bool Convert(plMaxNode *node, plErrorMsg *pErrMsg) override;
};

//Max desc stuff necessary below.
CLASS_DESC(plBarneyComponent, gBarneyDesc, "Barney",  "Barney", COMP_TYPE_IGNORE, Class_ID(0x376955dc, 0x2fec50ae))

ParamBlockDesc2 gBarneyBk
(
 plComponent::kBlkComp, _T("Barney"), 0, &gBarneyDesc, P_AUTO_CONSTRUCT + P_AUTO_UI, plComponent::kRefComp,

    IDD_COMP_BARNEY, IDS_COMP_BARNEYS, 0, 0, nullptr,

    p_end
);

plBarneyComponent::plBarneyComponent()
{
    fClassDesc = &gBarneyDesc;
    fClassDesc->MakeAutoParamBlocks(this);
}

// SetupProperties - Internal setup and write-only set properties on the MaxNode. No reading
// of properties on the MaxNode, as it's still indeterminant.
bool plBarneyComponent::SetupProperties(plMaxNode *pNode,  plErrorMsg *pErrMsg)
{
    pNode->SetCanConvert(false);
    pNode->SetIsBarney(true);
    return true;
}

bool plBarneyComponent::Convert(plMaxNode *node, plErrorMsg *pErrMsg)
{
    return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  NoShow Component
//
//

//Class that accesses the paramblock below.
class plNoShowComponent : public plComponent
{
public:
    enum
    {
        kShowable,
        kAffectDraw,
        kAffectPhys
    };
    

public:
    plNoShowComponent();

    void CollectNonDrawables(INodeTab& nonDrawables) override;

    // SetupProperties - Internal setup and write-only set properties on the MaxNode. No reading
    // of properties on the MaxNode, as it's still indeterminant.
    bool SetupProperties(plMaxNode *pNode, plErrorMsg *pErrMsg) override;
    bool Convert(plMaxNode *node, plErrorMsg *pErrMsg) override;
};

const Class_ID COMP_NOSHOW_CID(0x41cb2b85, 0x615932c6);

//Max desc stuff necessary below.
CLASS_DESC(plNoShowComponent, gNoShowDesc, "NoShow",  "NoShow", COMP_TYPE_IGNORE, COMP_NOSHOW_CID)

ParamBlockDesc2 gNoShowBk
(
    plComponent::kBlkComp, _T("NoShow"), 0, &gNoShowDesc, P_AUTO_CONSTRUCT + P_AUTO_UI, plComponent::kRefComp,

    IDD_COMP_NOSHOW, IDS_COMP_NOSHOW, 0, 0, nullptr,

    plNoShowComponent::kShowable,  _T("Showable"), TYPE_BOOL,       0, 0,
        p_default,  FALSE,
        p_ui,   TYPE_SINGLECHEKBOX, IDC_COMP_NOSHOW_SHOWABLE,
        p_end,

    plNoShowComponent::kAffectDraw,  _T("AffectDraw"), TYPE_BOOL,       0, 0,
        p_default,  TRUE,
        p_ui,   TYPE_SINGLECHEKBOX, IDC_COMP_NOSHOW_AFFECTDRAW,
        p_end,

    plNoShowComponent::kAffectPhys,  _T("AffectPhys"), TYPE_BOOL,       0, 0,
        p_default,  FALSE,
        p_ui,   TYPE_SINGLECHEKBOX, IDC_COMP_NOSHOW_AFFECTPHYS,
        p_end,

    p_end
);

plNoShowComponent::plNoShowComponent()
{
    fClassDesc = &gNoShowDesc;
    fClassDesc->MakeAutoParamBlocks(this);
}

// SetupProperties - Internal setup and write-only set properties on the MaxNode. No reading
// of properties on the MaxNode, as it's still indeterminant.
bool plNoShowComponent::SetupProperties(plMaxNode *pNode,  plErrorMsg *pErrMsg)
{
    if( !fCompPB->GetInt(kShowable) )
    {
        if( fCompPB->GetInt(kAffectDraw) )
            pNode->SetDrawable(false);
        if( fCompPB->GetInt(kAffectPhys) )
            pNode->SetPhysical(false);
    }

    return true;
}

bool plNoShowComponent::Convert(plMaxNode *node, plErrorMsg *pErrMsg)
{
    plSceneObject* obj = node->GetSceneObject();
    if( !obj )
        return true;

    if( fCompPB->GetInt(kShowable) )
    {
        if( fCompPB->GetInt(kAffectDraw) )
        {
            plEnableMsg* eMsg = new plEnableMsg(plEnableMsg::kDisable, plEnableMsg::kDrawable);
            eMsg->AddReceiver(obj->GetKey());
            eMsg->Send();
        }
        if( fCompPB->GetInt(kAffectPhys) )
        {
            hsAssert(0, "Who uses this?");
//          plEventGroupEnableMsg* pMsg = new plEventGroupEnableMsg;
//          pMsg->SetFlags(plEventGroupEnableMsg::kCollideOff | plEventGroupEnableMsg::kReportOff);
//          pMsg->AddReceiver(obj->GetKey());
//          pMsg->Send();
        }
#if 0
        plDrawInterface* di = node->GetDrawInterface();
        if( di && 
        {
            di->SetProperty(plDrawInterface::kDisable, true);
        }
#endif
    }
    return true;
}
void plNoShowComponent::CollectNonDrawables(INodeTab& nonDrawables) 
{ 
    if( fCompPB->GetInt(kAffectDraw) )
        AddTargetsToList(nonDrawables); 
}

