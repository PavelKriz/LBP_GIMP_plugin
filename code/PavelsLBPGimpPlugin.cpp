//----------------------------------------------------------------------------------------
/**
 * \file       PavelsLBPGimpPlugin.cpp
 * \author     Pavel Kříž, Ing. Jiří Chludil
 * \date       2019/11/30
 * \brief      Main file of plugin. Contains necessary functions for plugin and few others which are colaborating with necessary ones.
*/
//----------------------------------------------------------------------------------------

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <stdio.h>
#include <string>
#include "CProcessHandler.hpp"
#include "params.h"
#include "UI.hpp"

//===================================================================================================================================================================

// declare functions
static void query       (void);
static void run         (const gchar      *name,
                         gint              nparams,
                         const GimpParam  *param,
                         gint             *nreturn_vals,
                         GimpParam       **return_vals);

static void process_image        (GimpDrawable     *drawable, GimpPreview  *preview);
static gboolean lbpDialog (GimpDrawable *drawable);

// pass query and run functions to gimp
GimpPlugInInfo PLUG_IN_INFO =
{
  NULL,  //init
  NULL,  //quit
  query,
  run
};

// gimp necessary macro
MAIN()

//===================================================================================================================================================================

/// query initiliezes plugin in gimp by calling gimp api functions
static void query (void)
{
  static GimpParamDef args[] =
  {
    {
      GIMP_PDB_INT32,
      (gchar*) "run-mode",
      (gchar*) "Run mode"
    },
    {
      GIMP_PDB_IMAGE,
      (gchar*) "image",
      (gchar*) "Input image"
    },
    {
      GIMP_PDB_DRAWABLE,
      (gchar*) "drawable",
      (gchar*) "Input drawable"
    }
  };

  gimp_install_procedure (
    "PavelsLBPGimpPlugin",
    "Shows local binary patterns of different LBP methods (LBP, mLBP, ULBP, CLBP)",
    "Help: Calculates and shows local binary patterns of image",
    "Pavel Kříž",
    "Copyright Pavel Kříž",
    "2019.11",
    "PavelsLBPGimpPlugin",
    "RGB, RGBA",
    GIMP_PLUGIN,
    G_N_ELEMENTS (args), 0,
    args, NULL);

  gimp_plugin_menu_register ("PavelsLBPGimpPlugin","<Image>/Filters/Misc");
}


//===================================================================================================================================================================

/// run is standart function of every gimp plugin - take a look on (gimp docs)
static void run (const gchar      *name,
       gint              nparams,
       const GimpParam  *param,
       gint             *nreturn_vals,
       GimpParam       **return_vals)
      {

  static GimpParam  values[1];
  GimpPDBStatusType status = GIMP_PDB_SUCCESS;
  GimpRunMode       run_mode;
  GimpDrawable     *drawable;

  /* Setting mandatory output values */
  *nreturn_vals = 1;
  *return_vals  = values;
  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
  //acquisition of run mode
  run_mode = (GimpRunMode) param[0].data.d_int32;
  //acquisition of image
  drawable = gimp_drawable_get (param[2].data.d_drawable);

  switch (run_mode) {
  //running from UI
	case GIMP_RUN_INTERACTIVE:
	  /* Get options last values if needed */
	  gimp_get_data ("PavelsLBPplugin", &globalUIInfo);
	  /* Display the mainDialog */
	  if (! lbpDialog (drawable))
		return;
	      break;

  //running from command line
	case GIMP_RUN_NONINTERACTIVE: {
	  if (nparams != 6) {status = GIMP_PDB_CALLING_ERROR;}
    const gchar * csMethod = sLBP.c_str();
    gint radiusTMP = DEFAULT_RADIUS;
    gint pointsCountTMP = DEFAULT_POINTS;
	  if (status == GIMP_PDB_SUCCESS){
      csMethod = param[3].data.d_string;  
      radiusTMP = param[4].data.d_int32;
      pointsCountTMP = param[5].data.d_int32;
    }
    if (!nonInteractiveParser(csMethod,radiusTMP, pointsCountTMP)){
      status = GIMP_PDB_CALLING_ERROR;
      return;
    }

	  break;
  }
  // running with last set values
	case GIMP_RUN_WITH_LAST_VALS:
	  /*  Get options last values if needed  */
	  gimp_get_data ("PavelsLBPplugin", &globalUIInfo);
	  break;

	default:
	  break;
	}

        // inversion
        process_image (drawable,NULL);

        // flush of the image
        gimp_displays_flush ();
        gimp_drawable_detach (drawable);
      }

//===================================================================================================================================================================

/// process_image() just initialises CProcessHandler and run CProcessHandler
/**
 * \param[in] drawable  pointer to gimp drawable (image canvas)
 * \param[in] drawable  pointer to gimp preview in the plugins GUI, may be NULL if the process is called on whole image and not just preview
 */
static void process_image (GimpDrawable *drawable, GimpPreview  *preview)
{
  CProcessHandler processHandler(drawable, preview, globalUIInfo);
  processHandler.process(globalUIInfo.processType);
}

//===================================================================================================================================================================

/// lbpDialog() initialises GUI window of the plugin if the plugin was called from GUI
/**
 * \param[in] drawable pointer to gimp drawable (image canvas)
 */
static gboolean lbpDialog (GimpDrawable *drawable)
{

//must be called, first is program name, second something non-used, is there only for historical reason
  gimp_ui_init ("Pavels LBP plugin", FALSE);

  //init of the window
  GtkWidget * mainDialog = gimp_dialog_new ("Pavels LBP plugin", "Pavels LBP plugin",
                            NULL, (GtkDialogFlags) 0,
                            gimp_standard_help_func, "Pavels LBP plugin",
                            GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                            GTK_STOCK_OK,     GTK_RESPONSE_OK,
                            NULL);  

  //main vertical box
  GtkWidget * main_vbox = gtk_vbox_new (FALSE, 6);
  gtk_container_add (GTK_CONTAINER (GTK_DIALOG (mainDialog)->vbox), main_vbox);
  gtk_widget_show (main_vbox);

  // preview widget
  GtkWidget * preview = gimp_drawable_preview_new (drawable,&globalUIInfo.preview);
  gtk_box_pack_start (GTK_BOX (main_vbox), preview, TRUE, TRUE, 0);
  gtk_widget_show (preview);

  /*frame for choosing methods and submethods of LBP*/
  GtkWidget * methodChoiceFrame = gtk_frame_new (NULL);
  gtk_widget_show (methodChoiceFrame);
  gtk_box_pack_start (GTK_BOX (main_vbox), methodChoiceFrame, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (methodChoiceFrame), 6);

  // aligment in methodChoiceFrame
  GtkWidget * methodChoiceAlignment = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (methodChoiceAlignment);
  gtk_container_add (GTK_CONTAINER (methodChoiceFrame), methodChoiceAlignment);
  gtk_alignment_set_padding (GTK_ALIGNMENT (methodChoiceAlignment), 6, 6, 6, 6);

  // box for choosing methods and submethods of LBP
  GtkWidget * main_choicebox = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (main_choicebox);
  gtk_container_add (GTK_CONTAINER (methodChoiceAlignment), main_choicebox);

  // label of method choice box
  GtkWidget * methodChoiceLabel = gtk_label_new (sPickLBPMethodLabel.c_str());
  gtk_widget_show (methodChoiceLabel);
  gtk_frame_set_label_widget (GTK_FRAME (methodChoiceFrame), methodChoiceLabel);
  gtk_label_set_use_markup (GTK_LABEL (methodChoiceLabel), TRUE);

  //METHOD COMBO BOX
  GtkWidget * comboBox  = gtk_combo_box_text_new();
  UIComboBox(comboBox, main_choicebox);

 //SUB-METHOD COMBO BOX
  GtkWidget * subComboBox  = gtk_combo_box_text_new();
  UISubComboBox(subComboBox, main_choicebox);

  /*frame for parameters of LBP input */
  GtkWidget * parametersFrame = gtk_frame_new (NULL);
  gtk_widget_show (parametersFrame);
  gtk_box_pack_start (GTK_BOX (main_vbox), parametersFrame, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (parametersFrame), 6);

  // aligment in parametersFrame
  GtkWidget * parametersAlignment = gtk_alignment_new (0.5, 0.5, 1, 1);
  gtk_widget_show (parametersAlignment);
  gtk_container_add (GTK_CONTAINER (parametersFrame), parametersAlignment);
  gtk_alignment_set_padding (GTK_ALIGNMENT (parametersAlignment), 6, 6, 6, 6);

  // horizontal box inside of the parametersAlignment inside of the parametersFrame
  GtkWidget * main_hbox = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (main_hbox);
  gtk_container_add (GTK_CONTAINER (parametersAlignment), main_hbox);

  // label of radiusSpinButton
  GtkWidget * radius_label = gtk_label_new_with_mnemonic (sRadiusLabel.c_str());
  gtk_widget_show (radius_label);
  gtk_box_pack_start (GTK_BOX (main_hbox), radius_label, FALSE, FALSE, 6);
  gtk_label_set_justify (GTK_LABEL (radius_label), GTK_JUSTIFY_RIGHT);

  // spinnbutton for radius input
  GtkObject * radiusSpinButton_adj;
  GtkWidget * radiusSpinButton = gimp_spin_button_new (&radiusSpinButton_adj, globalUIInfo.radius, MIN_RADIUS, MAX_RADIUS, 1, 1, 1, 5, 0);
  gtk_box_pack_start (GTK_BOX (main_hbox), radiusSpinButton, FALSE, FALSE, 0);
  gtk_widget_show (radiusSpinButton);

  //label for pointsCountSpinButton
  GtkWidget * points_label = gtk_label_new_with_mnemonic (sPointsLabel.c_str());
  gtk_widget_show (points_label);
  gtk_box_pack_start (GTK_BOX (main_hbox), points_label, FALSE, FALSE, 6);
  gtk_label_set_justify (GTK_LABEL (points_label), GTK_JUSTIFY_RIGHT);

  //spinbutton for number of neighbor points input
  GtkObject * pointsSpinButton_Adj;
  globalUIInfo.pointsCountSpinButton = gimp_spin_button_new (&pointsSpinButton_Adj, globalUIInfo.pointsCount, MIN_POINTS, MAX_POINTS_LBP, 1, 1, 1, 5, 0);
  gtk_box_pack_start (GTK_BOX (main_hbox), globalUIInfo.pointsCountSpinButton, FALSE, FALSE, 0);
  gtk_widget_show (globalUIInfo.pointsCountSpinButton);

  // label of parameters input box
  GtkWidget * parameters_frame_label = gtk_label_new (sMofifyParametsLabel.c_str());
  gtk_widget_show (parameters_frame_label);
  gtk_frame_set_label_widget (GTK_FRAME (parametersFrame), parameters_frame_label);
  gtk_label_set_use_markup (GTK_LABEL (parameters_frame_label), TRUE);

  //connecting events with functions
  g_signal_connect_swapped (preview, "invalidated",
                            G_CALLBACK (process_image),
                            drawable);
  g_signal_connect_swapped (radiusSpinButton_adj, "value_changed",
                            G_CALLBACK (gimp_preview_invalidate),
                            preview);

  g_signal_connect_swapped (pointsSpinButton_Adj, "value_changed",
                            G_CALLBACK (gimp_preview_invalidate),
                            preview);
  
  g_signal_connect_swapped (comboBox,
                    "changed",
                    G_CALLBACK (gimp_preview_invalidate),
                    preview);
  g_signal_connect_swapped (subComboBox,
                    "changed",
                    G_CALLBACK (gimp_preview_invalidate),
                    preview);                    

  process_image (drawable, GIMP_PREVIEW (preview));
  g_signal_connect (radiusSpinButton_adj, "value_changed",
                    G_CALLBACK (gimp_int_adjustment_update),
                    &globalUIInfo.radius);

  g_signal_connect (pointsSpinButton_Adj, "value_changed",
                    G_CALLBACK (gimp_int_adjustment_update),
                    &globalUIInfo.pointsCount);
  
  g_signal_connect(comboBox,
                    "changed",
                    GTK_SIGNAL_FUNC (methodParser),
                    (gpointer) comboBox);

  g_signal_connect(subComboBox,
                  "changed",
                  GTK_SIGNAL_FUNC (subMethodParser),
                  (gpointer) subComboBox);


  //showing GUI window of the plugin
  gtk_widget_show (mainDialog);

  gboolean   run = (gimp_dialog_run (GIMP_DIALOG (mainDialog)) == GTK_RESPONSE_OK);

  gtk_widget_destroy (mainDialog);

  return run;
}

//===================================================================================================================================================================
