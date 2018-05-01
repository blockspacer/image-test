default_settings = {
  ["version"] =  0.1

-- RGB component values go from 0-255
-- HSV components: Hue goes from 0-360 like a color wheel, Saturation and Value go from 0.0 to 1.0
-- Grays range from 0.0 (black) to 1.0 (white)
-- Alpha values always go from 0 for transparent to 255 for opaque

-- The functions available for setting colors are: rgb, rgba, hsv, hsva, gray and grayAlpha

,["Menu text size in points"           ] = 12
,["Menu background top color"          ] = gray(0.2)
,["Menu background bottom color"       ] = gray(0.12)
,["Menu text color"                    ] = gray(0.7)
,["Menu text shaded color"             ] = gray(0.4)
,["Menu selected item color"           ] = gray(0.05)
,["Menu selected item background color"] = gray(0.05)

,["Linux font"  ] = "Ubuntu"--"Free Sans" 
,["Apple font"  ] = "Helvetica"
,["Windows font"] = "Arial"

,["Bubble halo corner margin in millimetres"] = 6
,["Bubble side maximum bulginess (as multiple of corner margin size)"] = 1.6
,["Bubble halo mouseover color"] = rgb(240,0,0)
,["Focused bubble center color"] = rgb(255,255,255)

,["Ungrouped bubble halo color"] = greyAlpha(0.5, 120)

,["File browser background color"] = grey(0)
,["File browser fisheye outline color"] = grey(0.5)
,["File browser text size (pts)"] = 12
,["File browser text color"] = grey(0.7)
,["File browser text background color"] = greyAlpha(0.1,125)
,["File browser directory node color"] = rgb(130,160,255)
,["File browser current directory arc color"] = rgb(160,190,255)
,["File browser unexplored directory color"] = rgb(220,120,80)
,["File browser file node color"] = grey(0.5)
,["File browser location text size"] = 12
,["File browser root node color"] = rgb(110,220,110)
,["File browser current directory color"] = grey(0.99)

,["Directory listing text color"] = grey(0.6)
,["Directory listing text size"] = 12
,["Directory listing frame color"] = grey(0.6)
,["Directory listing bubble background color"] = grey(0.0)
,["Directory listing background color 1"] = grey(0.03)
,["Directory listing background color 2"] = grey(0.15)

,["Modifiable option highlighted color"] = rgb(210,30,30)
,["Modifiable option color"] = rgb(190, 130,130)

}
