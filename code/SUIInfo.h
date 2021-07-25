//----------------------------------------------------------------------------------------
/**
 * \file       SUIInfo.h
 * \author     Pavel Kříž
 * \date       2019/11/30
 * \brief      Contains SUIInfo struct
*/
//----------------------------------------------------------------------------------------


//include guard
#ifndef SUIINFO_H
#define SUIINFO_H

/// SUIInfo holds UI and global info, mainly pointers to UI widgets and Algorithm parameters
typedef struct
{
  gint          radius;       ///< chosen radius for algorithm
  gint          pointsCount;  ///< chosen number of points for algorithm
  ELBPType      processType;  ///< chosen method to be processed
  gboolean      preview;      ///< holds piece of information about preview or NULL if whole image has to be processed
  GtkWidget * pointsCountSpinButton; ///< pointer to spin button widget that determines number of points
  GtkWidget * subComboBox;           ///< pointer to combo box which shows and choose algorithm method
} SUIInfo;

#endif //SUIINFO_H