/***************************************************************
 * Name:      testMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2015-08-25
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef TESTMAIN_H
#define TESTMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "testApp.h"


#include <wx/button.h>
#include <wx/statline.h>
class testDialog: public wxDialog
{
    public:
        testDialog(wxDialog *dlg, const wxString& title);
        ~testDialog();

    protected:
        enum
        {
            idBtnQuit = 1000,
            idBtnAbout
        };
        wxStaticText* m_staticText1;
        wxButton* BtnAbout;
        wxStaticLine* m_staticline1;
        wxButton* BtnQuit;

    private:
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};

#endif // TESTMAIN_H
