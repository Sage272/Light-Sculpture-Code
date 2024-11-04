# Light-Sculpture-Code
My Dos Pueblos Engineering Academy (DPEA) Light Sculpture Code

The base code was provided to me by the DPEA, but I significantly modified the code in "LightSculptureLib.cpp", I was provided with only the initial functions of loop and setup in "MyFinalLSCode.ino", and I added some function names into "LightSculptureLib.h".

In "LightSculptureLib.cpp" I added the mode switching functionality, I altered the code so that the delay times no longer ran on the position of the potentiometer knob, and I created all of the partial fade up/down functions. The mode switching functionality allowed me to stop functions mid way through running, providing imediate switching from calm, normal, or night mode in "MyFinalLSCode.ino". Previously, any function would have had to run is full course, but now that it is tracked in the fade functions, that is not an issue.
