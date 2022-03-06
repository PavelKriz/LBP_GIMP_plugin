# Pavel's LBP Gimp Plugin
## Input
LBP Flags Create a GIMP plug-in that counts and displays (parametric) LBP 30b flags

Create a plugin that counts and displays basic, not widespread LBP flags.

An additional 10b can be obtained to create parametric flags (radius).

An additional 10b can be obtained using a centralized LBP.

An additional 10b can be obtained using LBP dominants.

The task can be further extended by other variants of LBP symptoms.

## Note for evaluators
 After consulting with the instructor, I implemented the assignment for a total of 50 points. I implemented the basic task, extended other LBP methods. The condition of the consulting teacher for the 50-point task was the implementation of a more complex LBP, thus the CLBP is implemented here. Also added to the input to slightly improve the UI plugin, now it's a bit interactive. Other advantages of why I think my implementation of such a plugin is good are listed in the Plugin Properties section. I would also try to mention mainly the documentation that I tried to do extensively, because I missed something like that when creating the plugin. In particular, none of the students' sample works are written in c ++. I have tried to do this, if this work is decent, so that it can help future students.

## Introduction
According to the assignment, the plugin was added to the Gimp photo editor. The goal of the plugin is to calculate and visualize the selected types of LBP flags. Implemented LBP methods of symptom screening are LBP, mLBP, ULBP, CLBP. A description of these algorithms is given in the appendix Description of used algorithms. Further descriptions of the algorithms used can be found in the code documentation.

**Note**:

Much of the plugin can be used as a skeleton when programming another plugin for Gimp. All you have to do is implement the CAlghoritm interface (according to the code documentation), or modify the UI part of the plugin and enable the execution of the newly programmed algorithm in the CProcessHandler class (according to the code documentation).

## Important links

<a href="https://github.com/PavelKriz/LBP_GIMP_plugin/tree/master/code"> Folder containing plugin code and compile.sh installation script. </a>

<a href="https://github.com/PavelKriz/LBP_GIMP_plugin/tree/master/docs/html"> Doxygen html documentation folder. </a>

<a href="https://github.com/PavelKriz/LBP_GIMP_plugin/blob/master/docs/html/index.html"> Introductory entry to the documentation. </a>

<a href="https://github.com/PavelKriz/LBP_GIMP_plugin/blob/master/Algorithms/LBP.pdf"> PDF algorithm documentation. </a>

## Plugin properties

* The plugin code is extensively documented for the scope of work.
* The plugin can be converted to other functionality very quickly. All you have to do is implement the CAlghoritm interface (according to the documentation), or modify the UI part of the plugin and enable the execution of the newly programmed algorithm (according to the documentation) in the CProcessHandler class. By using this procedure, the programmer saves a lot of time with the same functionality.
* The plugin is implemented in C ++ and is therefore fast.
* Image processing is performed by tiles, so it is very memory-intensive.
* Part processing is not a primitive task in LBP as its surroundings are needed to calculate the pixel value. The area around the tile is either obtained from the image or by mirroring the tile over the edge of the image.
* The coordinates of the surrounding points for calculating the local LBP flag are interpolated from two pixels. The values ​​obtained are thus more accurate.
* The plugin contains an interactive UI that changes based on the choice of LBP retrieval method.

## Additional documentation

I created two more dossiers.

PDF documentation describing the used LBP methods can be found in the folder ./Algorithms

The code documentation is located in the ./Docs/html folder and is launched on the start page by opening the index.html file. This is the generated html code documentation. The documentation was generated from code, mainly header files. This documentation can also be found here in uncompiled latex in the ./Docs/latex folder.

## Installation

The manual is written for the Linux operating system. Because the plugin consists of multiple folders and uses a library, it cannot be compiled using the basic plugin installer for Gimp gimptool-2.0. However, installation is simple. All files in the ./code folder have the same directory. Run the command [gimptool-2.0 --install PavelsLBPGimpPlugin.cpp] from this directory. This command fails. It is necessary to ignore error messages and it is necessary to copy the part that gimptool prints at the beginning of the listing from gcc to the last library added (it should be -lfreetype). Then run the command [./compile.sh "[copied part]"] from the same directory. The plugin should now be successfully compiled into the correct Gimp plugin folder.

The plugin is then accessible via filters / misc / PavelsLBPGimpPlugin. An image must be open for the plugin to be enabled.

Note: Square brackets do not belong to commands, they only indicate the area in which the command or some variable is.


## Implemented LBP methods

The processed results of the plugin will be shown in the following image. The rag area in the picture is interesting for its textureis clearly visible on LBP visualizations. It is also interesting to note the absence of an effect of color shades on the LBP. You can see it well, for example, on a rag or on a pallet. For example, edges can be interesting, namely the edge of a palace or a brush presenting blurred edges. The image thus allows to observe the differences in LBP in the focused part of the image and in the unfocused part of the image.

<figure>
<img src=example_images/testImage.JPG width="400">
<figcaption> Image used to demonstrate how the plugin works. </figcaption>
</figure>

The author of the image is the author of the plugin.

In all implementation cases, the range of the radio is a maximum of 190 and a minimum of 1. The number of neighboring points varies according to the implemented method, but the minimum is 4 and the maximum of all methods is 24.

### LBP

LBP is a method of obtaining the symptoms on which other LBP methods are based. The plugin displays the LBP values ​​according to Figure 1. Each color has its own meaning.

The following diagram explains the meaning of each RGB channel when displaying LBP. The colors explain what part of the environment affected the given LBP value. Each color channel thus displays the value as if the LBP sum was calculated only for points from the given neighborhood. Neighboring points mean neighboring points. It all depends on the number of neighboring points. Additional color channels are displayed or not displayed depending on the number of adjacent points. For 24 contiguous points, there are 8 contiguous points for each color channel. However, if the number of neighboring points was only 19 then the red channel for the first eight points in the direction of the arrows, the green channel for the next eight points and finally for the remaining 3 points the blue channel. Thus analogous to other numbers of neighboring points.
<figure>
<figcaption> Diagram 1 - visualization of values, p = number of adjacent pixel points </figcaption>
<img src=GimpPluginLBPSousedniBodydrawio.JPG width="1000">
</figure>

<figure>
<img src=example_images/LBP10r8p.JPG width="400" alt="LBP s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption> LBP with parameters radius = 10 and number of neighboring points = 8 </figcaption>

<img src=example_images/LBP1r16p.JPG width="400" alt="LBP s parametry radius = 1 a počet sousedních bodů = 16">
<figcaption> LBP with parameters radius = 1 and number of neighboring points = 16, It is interesting to note that the red color is for LBP values ​​of the sum only with lower neighboring points and green vice versa </figcaption>

<img src=example_images/LBP10r24p.JPG width="400" alt="LBP s parametry radius = 10 a počet sousedních bodů = 24">
<figcaption> LBP with parameters radius = 10 and number of neighboring points = 24 </figcaption>
</figure>

### mLBP

The mLBP method is derived from LBP and its results are similar to LBB. MLBP visualization has the same rules as LBP see diagram 1.

<figure>
<img src=example_images/mLBP10r8p.JPG width="400" alt="mLBP s parametry radius = 10 a počet sousedních bodů = 8">
mLf with parameters radius = 10 and number of neighboring points = 8 </figcaption>
</figure>

### ULBP

The ULBP method is calculated by the same basic LBP but omits non-uniform values. In the plugin, ULBPs are visualized in the same way as LBPs with a maximum of 16 points, with the blue color symbolizing the omitted pixel with a non-uniform LBP value. Areas with non-uniform values ​​can be seen in these images, which were created with the same parameters only once with LBP and a second time with ULBP.

The LBP method is displayed similarly to LBP with the only difference that the maximum number of neighboring points is only 16 and non-uniform values ​​are displayed as RGB (0,0,255).

<figure>
<img src=example_images/LBP190r16p.JPG width="400" alt="LBP s parametry radius = 190 a počet sousedních bodů = 16">
<figcaption> LBP with parameters radius = 190 and number of neighboring points = 16 </figcaption>

<img src=example_images/ULBP190r16p.JPG width="400" alt="ULBP s parametry radius = 190 a počet sousedních bodů = 16">
<figcaption> ULBP with parameters radius = 190 and number of neighboring points = 16 </figcaption>
</figure>

### CLBP

The CLBP calculation consists of three parts CLBP_S, CLBP_M, CLBP_C. The plugin visualizes all three at once CLBP_S (red channel RGB), CLBP_M (green channel RGB) and CLBP_C (blue channel LBP). The individual channels then visualize the CLBP subpart values ​​in the range 0 to 255. The maximum number of adjacent points is 8.

<figure>
<img src=example_images/CLBP10r8p.JPG width="400" alt="CLBP s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption> CLBP with parameters radius = 10 and number of neighboring points = 8 </figcaption>
</figure>

#### CLBP_S

The plugin can also display individual CLBP components. Values ​​are mapped from smallest (darkest) to largest (lightest). The highest value is not white but it is colored white. Here it is colored red, because in CLBP, CLBP_S is represented by a red RGB channel.

<figure>
<img src=example_images/CLBP_S10r8p.JPG width="400" alt="CLBP_S s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption> CLBP_S with parameters radius = 10 and number of neighboring points = 8 </figcaption>
</figure>

#### CLBP_M

The plugin can also display individual CLBP components. Values ​​are mapped from smallest (darkest) to largest (lightest). The highest value is not white but it is a colorwhite. Here it is colored green, because in CLBP, CLBP_M is represented by the green RGB channel.

<figure>
<img src=example_images/CLBP_M10r8p.JPG width="400" alt="CLBP_M s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption> CLBP_M with parameters radius = 10 and number of neighboring points = 8 </figcaption>
</figure>

#### CLBP_C

The plugin can also display individual CLBP components. Values ​​are mapped from smallest (darkest) to largest (lightest). The highest value is not white but it is colored white. Here it is colored blue, because in CLBP, CLBP_C is represented by a blue RGB channel.

<figure>
<img src=example_images/CLBP_C10r8p.JPG width="400" alt="CLBP_C s parametry radius = 10 a počet sousedních bodů = 8">
<figcaption> CLBP_C with parameters radius = 10 and number of neighboring points = 8 </figcaption>
</figure>

## Short summary of functions

All functions can be seen to be similar in some way. No wonder everyone has an LBP in their name. The CLBP contains the most information and the LBP value is displayed in a different way. However, it is the most computationally intensive of all the functions.
