/***************************************************************
 * Name:      testApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2015-08-25
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "testApp.h"
#include "testMain.h"

IMPLEMENT_APP(testApp);

bool testApp::OnInit()
{
    
    testDialog* dlg = new testDialog(0L, _("wxWidgets Application Template"));
    dlg->SetIcon(wxICON(aaaa)); // To Set App Icon
    dlg->Show();
    return true;
}
