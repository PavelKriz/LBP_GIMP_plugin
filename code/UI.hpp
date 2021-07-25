//----------------------------------------------------------------------------------------
/**
 * \file       UI.hpp
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Helping functions for plugin UI.
*/
//----------------------------------------------------------------------------------------

#include "params.h"
#include <libgimp/gimp.h>
#include "SUIInfo.h"
#include <string>

//=============================================================================================================================================================

/// globalUIInfo is global variable. It holds UI and global info, mainly pointers to UI widgets and Algorithm parameters
static SUIInfo globalUIInfo =
{
  DEFAULT_RADIUS,   // radius
  DEFAULT_POINTS,   // points count
  ELBP,  //process type
  1,     //preview
  NULL, //pointsCountSpinButton
  NULL  //subComboBox
};

//=============================================================================================================================================================

/// Function that parses input from command line
/**
 * It parses method that was typed as first plugin argument and it as checking ranges of other parameters too
 * 
 * \param[in] sMethod string containing method argument from command line
 */
bool nonInteractiveMethodParser(std::string sMethod){
  if(sMethod == std::string("help") || sMethod == std::string("HELP")|| sMethod == std::string("-help")|| sMethod == std::string("--help")){
    g_print("USAGE OF PLUGIN IS- [pluginName] [LBP METHOD\nPOSSIBLE NAMES ARE:\nLBP, mLBP, ULBP, CLBP_SMC, CLBP_S, CLBP_M, CLBP_C\n] [LBP radius] [number of neighbour points] \n");
  } else if(sMethod == sLBP){
    g_print("LBP\n");
    globalUIInfo.processType = ELBP;

    if(globalUIInfo.radius < MIN_RADIUS || globalUIInfo.radius > MAX_RADIUS){
      g_print("ERROR IN RADIUS, possible range is %d<= radius <= %d\nRange also depends on Image Size\n", MIN_RADIUS, MAX_RADIUS);
      return false;
    } else if (globalUIInfo.pointsCount < MIN_POINTS || globalUIInfo.pointsCount > MAX_POINTS_LBP){
      g_print("ERROR IN COUNT OF NEIGHBOUR POINTS, possible range is %d<= radius <= %d\n", MIN_POINTS, MAX_POINTS_LBP);
      return false;
    }

  } else if (sMethod == smLBP){
    g_print("mLBP\n");
    globalUIInfo.processType = EmLBP;

    if(globalUIInfo.radius < MIN_RADIUS || globalUIInfo.radius > MAX_RADIUS){
      g_print("ERROR IN RADIUS, possible range is %d<= radius <= %d\nRange also depends on Image Size\n", MIN_RADIUS, MAX_RADIUS);
      return false;
    } else if (globalUIInfo.pointsCount < MIN_POINTS || globalUIInfo.pointsCount > MAX_POINTS_MLBP){
      g_print("ERROR IN COUNT OF NEIGHBOUR POINTS, possible range is %d<= radius <= %d\n", MIN_POINTS, MAX_POINTS_MLBP);
      return false;
    }

  } else if (sMethod == sULBP){
    g_print("ULBP\n");
    globalUIInfo.processType = EULBP;
    
    if(globalUIInfo.radius < MIN_RADIUS || globalUIInfo.radius > MAX_RADIUS){
      g_print("ERROR IN RADIUS, possible range is %d<= radius <= %d\nRange also depends on Image Size\n", MIN_RADIUS, MAX_RADIUS);
      return false;
    } else if (globalUIInfo.pointsCount < MIN_POINTS || globalUIInfo.pointsCount > MAX_POINTS_ULBP){
      g_print("ERROR IN COUNT OF NEIGHBOUR POINTS, possible range is %d<= radius <= %d\n", MIN_POINTS, MAX_POINTS_ULBP);
      return false;
    }

  } else if(sMethod == sCLBP_SMC){
    g_print("CLBP_SMC\n");
    globalUIInfo.processType = ECLBP_SMC;

    if(globalUIInfo.radius < MIN_RADIUS || globalUIInfo.radius > MAX_RADIUS){
      g_print("ERROR IN RADIUS, possible range is %d<= radius <= %d\nRange also depends on Image Size\n", MIN_RADIUS, MAX_RADIUS);
      return false;
    } else if (globalUIInfo.pointsCount < MIN_POINTS || globalUIInfo.pointsCount > MAX_POINTS_CLBP){
      g_print("ERROR IN COUNT OF NEIGHBOUR POINTS, possible range is %d<= radius <= %d\n", MIN_POINTS, MAX_POINTS_CLBP);
      return false;
    }

  } else if (sMethod == sCLBP_S){
    g_print("CLBP_S\n");
    globalUIInfo.processType = ECLBP_S;
    if(globalUIInfo.radius < MIN_RADIUS || globalUIInfo.radius > MAX_RADIUS){
      g_print("ERROR IN RADIUS, possible range is %d<= radius <= %d\nRange also depends on Image Size\n", MIN_RADIUS, MAX_RADIUS);
      return false;
    } else if (globalUIInfo.pointsCount < MIN_POINTS || globalUIInfo.pointsCount > MAX_POINTS_CLBP){
      g_print("ERROR IN COUNT OF NEIGHBOUR POINTS, possible range is %d<= radius <= %d\n", MIN_POINTS, MAX_POINTS_CLBP);
      return false;
    }

  } else if (sMethod == sCLBP_M){
    g_print("CLBP_M\n");
    globalUIInfo.processType = ECLBP_M;
    if(globalUIInfo.radius < MIN_RADIUS || globalUIInfo.radius > MAX_RADIUS){
      g_print("ERROR IN RADIUS, possible range is %d<= radius <= %d\nRange also depends on Image Size\n", MIN_RADIUS, MAX_RADIUS);
      return false;
    } else if (globalUIInfo.pointsCount < MIN_POINTS || globalUIInfo.pointsCount > MAX_POINTS_CLBP){
      g_print("ERROR IN COUNT OF NEIGHBOUR POINTS, possible range is %d<= radius <= %d\n", MIN_POINTS, MAX_POINTS_CLBP);
      return false;
    }

  } else if (sMethod == sCLBP_C) {
    g_print("CLBP_C\n");
    globalUIInfo.processType = ECLBP_C;
    if(globalUIInfo.radius < MIN_RADIUS || globalUIInfo.radius > MAX_RADIUS){
      g_print("ERROR IN RADIUS, possible range is %d<= radius <= %d\nRange also depends on Image Size\n", MIN_RADIUS, MAX_RADIUS);
      return false;
    } else if (globalUIInfo.pointsCount < MIN_POINTS || globalUIInfo.pointsCount > MAX_POINTS_CLBP){
      g_print("ERROR IN COUNT OF NEIGHBOUR POINTS, possible range is %d<= radius <= %d\n", MIN_POINTS, MAX_POINTS_CLBP);
      return false;
    }

  } else {
    //ERROR
    g_print("ERROR IN METHOD NAME, POSSIBLE NAMES ARE:\nLBP, mLBP, ULBP, CLBP_SMC, CLBP_S, CLBP_M, CLBP_C\n");
    return false;
  }
  return true;
}

//=============================================================================================================================================================

/// nonInteractiveParser sets int parameters and pass c++ method string to method parser and ranges checker - nonInteractiveMethodParser()
/**
 * \param[in] csMethod      c string method choice paramater typed in command line
 * \param[in] toRadius      radius parameter typed in command line
 * \param[in] toPointsCount number of points paramater typed in command line
 */
bool nonInteractiveParser(const gchar * csMethod, gint toRadius, gint toPointsCount){
  std::string sMethod = (const char* ) csMethod;
  globalUIInfo.radius = toRadius;
  globalUIInfo.pointsCount = toPointsCount;
  return nonInteractiveMethodParser(sMethod);
}

//=============================================================================================================================================================

/// subMethodParser parses sub method from submethod combo box
/**
 * \param[in] widget    comboBox widget
 * \param[in] comboBox  pointer to sub-method choice combo box
 */
void subMethodParser(GtkWidget * widget, gpointer comboBox){
  GtkWidget * PtrComboBox = (GtkWidget *) comboBox;
  std::string sSubMethod = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(PtrComboBox));
  
  if(sSubMethod == sCLBP_SMC){
    globalUIInfo.processType = ECLBP_SMC;
  } else if (sSubMethod == sCLBP_S){
    globalUIInfo.processType = ECLBP_S;
  } else if (sSubMethod == sCLBP_M){
    globalUIInfo.processType = ECLBP_M;
  } else {
    globalUIInfo.processType = ECLBP_C;
  }
}

//=============================================================================================================================================================

/// methodParser parses method from bmethod combo box
/**
 * It is also setting ranges of number of neighbor points spinn button and hiding or showing submethod combo box
 * 
 * \param[in] widget    comboBox widget
 * \param[in] comboBox  pointer to method choice combo box
 */
void methodParser(GtkWidget * widget, gpointer comboBox){

  GtkWidget * PtrComboBox = (GtkWidget *) comboBox;
  std::string sMethod = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(PtrComboBox));

  if(sMethod == sLBP){
    gtk_widget_hide(globalUIInfo.subComboBox);
    gtk_spin_button_set_range ((GtkSpinButton*)globalUIInfo.pointsCountSpinButton, MIN_POINTS, MAX_POINTS_LBP);
    globalUIInfo.processType = ELBP;

  } else if (sMethod == smLBP){
    gtk_widget_hide(globalUIInfo.subComboBox);
    gtk_spin_button_set_range ((GtkSpinButton*)globalUIInfo.pointsCountSpinButton, MIN_POINTS, MAX_POINTS_MLBP);
    globalUIInfo.processType = EmLBP;

  } else if (sMethod == sULBP){
    gtk_widget_hide(globalUIInfo.subComboBox);
    gtk_spin_button_set_range ((GtkSpinButton*)globalUIInfo.pointsCountSpinButton, MIN_POINTS, MAX_POINTS_ULBP);
    globalUIInfo.processType = EULBP;

  } else if (sMethod == sCLBP){
    gtk_widget_show (globalUIInfo.subComboBox);
    std::string sSubMethod = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(globalUIInfo.subComboBox));
    gtk_spin_button_set_range ((GtkSpinButton*)globalUIInfo.pointsCountSpinButton, MIN_POINTS, MAX_POINTS_CLBP);
    globalUIInfo.processType = ECLBP_SMC;  
  }
}

//=============================================================================================================================================================

/// UIComboBox is sequence of function calls to set method combo box
/**
 * \param[in] comboBox    pointer to already initialized combo box
 * \param[in] main_choicebox    pointer to box where will be widget placed
 */ 
void UIComboBox(GtkWidget * comboBox, GtkWidget * main_choicebox){
  gtk_box_pack_start (GTK_BOX (main_choicebox), comboBox, FALSE, FALSE, 6);
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBox), sLBP.c_str());
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBox), smLBP.c_str());
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBox), sULBP.c_str());
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboBox), sCLBP.c_str());
  gtk_combo_box_set_active (GTK_COMBO_BOX(comboBox), 0);
  gtk_widget_show (comboBox);
}

//=============================================================================================================================================================

/// UISubComboBox is sequence of function calls to set sub method combo box
/**
 * \param[in] subComboBox    pointer to already initialized combo box
 * \param[in] main_choicebox    pointer to box where will be widget placed
 */ 
void UISubComboBox(GtkWidget * subComboBox, GtkWidget * main_choicebox){
  gtk_box_pack_start (GTK_BOX (main_choicebox), subComboBox, FALSE, FALSE, 6);
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(subComboBox), sCLBP_SMC.c_str());
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(subComboBox), sCLBP_S.c_str());
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(subComboBox), sCLBP_M.c_str());
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(subComboBox), sCLBP_C.c_str());
  gtk_combo_box_set_active (GTK_COMBO_BOX(subComboBox), 0);
  globalUIInfo.subComboBox = subComboBox;
}