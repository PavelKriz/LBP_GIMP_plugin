#include "CProcessHandler.hpp"

CProcessHandler::CProcessHandler(GimpDrawable *drawable, GimpPreview  *preview, SUIInfo global){      
    this->global = global;
    this->drawable = drawable;
    this->preview = preview;
    

	if (preview){
        // we will get coordinates of left upper corner and right lower corner of part of image(preview part)
	    gimp_preview_get_position (preview, &x1, &y1);
	    gimp_preview_get_size (preview, &width, &height);
	    x2 = x1 + width;
	    y2 = y1 + height;

	} else {
        // we will get coordinates of left upper corner and right lower corner of imagae   
	    gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);
	    width = x2 - x1;
	    height = y2 - y1;
    }

    //size info of calculated area
    g_print("\nProcessing image of these parameters\n");
    g_print("width: %d, height: %d\n", (int)width, (int)height);
    g_print("x1: %d, y1: %d\n", (int)x1, (int)y1);
    g_print("x2: %d, y2: %d\n", (int)x2, (int)y2);    

    // channels represents how many channels are used per pixel
    channels = gimp_drawable_bpp (drawable->drawable_id);

    //output input init
    gimp_pixel_rgn_init (&(inOut.rgn_in),  drawable, x1, y1 , width, height , FALSE, FALSE);
    gimp_pixel_rgn_init (&(inOut.rgn_out), drawable, x1, y1, width, height,  TRUE,  TRUE);    
}


void CProcessHandler::process(ELBPType methodType){

    // print error in these cases
    if( width - 1 < global.radius || height - 1 < global.radius){
        //ON BEGGINING IS method CALLED ONLY WITH 1x1 size which is ridiculous and I dont know why
        if(width > 5 || height > 5){
            g_message("ERROR, radius is bigger then width or height\n");
        } else {
            g_print("Loading image passed to plugin. May happen on start of plugin. Or too small image.\n");
        }
        return;
    }

    //init progress
    gimp_progress_init ("Calculating chosen LBP method...");
    g_print ("Calculating chosen LBP method...");


    //init process info with right algorithm
    SProcessInfo processInfo = { channels, NULL};
    if(methodType == ELBP){
        processInfo.algorithm = new CLBP(inOut, global.radius, global.pointsCount);
    } else if (methodType == EmLBP) {
        processInfo.algorithm = new CMLBP(inOut, global.radius, global.pointsCount);
    } else if (methodType == EULBP){
        processInfo.algorithm = new CULBP(inOut, global.radius, global.pointsCount);

    } else if (methodType == ECLBP_SMC) {
        processInfo.algorithm = new CCLBP(inOut, global.radius, global.pointsCount, CCLBP::ECLBPType::ECLBP_SMC);
    } else if (methodType == ECLBP_S) {
        processInfo.algorithm = new CCLBP(inOut, global.radius, global.pointsCount, CCLBP::ECLBPType::ECLBP_S);
    } else if (methodType == ECLBP_M) {
        processInfo.algorithm = new CCLBP(inOut, global.radius, global.pointsCount, CCLBP::ECLBPType::ECLBP_M);
    } else  {
        processInfo.algorithm = new CCLBP(inOut, global.radius, global.pointsCount, CCLBP::ECLBPType::ECLBP_C);
    }

    //run processing of image as many times as needed
    CTileManager tileManager(width, height, x1, y1, processInfo.algorithm->sizeOfTileBorders());
    for(int it = 0; it < processInfo.algorithm->iterationsNeededToFinish();++it){
        tileManager.run( processInfo, it, processInfo.algorithm->iterationsNeededToFinish());
        processInfo.algorithm->loop();
    }

    delete processInfo.algorithm;
    g_print ("...Finished\n");

    //displaying preview or loading changed image
	if (preview) {
		gimp_drawable_preview_draw_region (GIMP_DRAWABLE_PREVIEW (preview),&(inOut.rgn_out));
        gimp_progress_update ( (gdouble) 1 );
	}
	else
	{
		// actualisation of output data
		gimp_drawable_flush (drawable);
		gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
		gimp_drawable_update (drawable->drawable_id, x1, y1, x2 - x1, y2 - y1);
	}
}