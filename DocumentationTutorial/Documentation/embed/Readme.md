---
keywords: Embed files
summary:  How to use fonts an images
author:   Christoph Hart
modified: 08.10.2019
colour:   0xFF555588
index:    03
---
  
This chapter covers the use of external data: displaying images and loading a custom font.

## Font

In order to use a special font, you'll have to go through the default procedure of embedding fonts in HISE:

1. Copy them into the **Images/fonts** subdirectory
2. Call `Engine.loadFontAs()` to load the font
3. Choose the **Font** (and **BoldFont**) in the MarkdownPanel's property list.

> Just take a look at the `onInit` callback of this project to see it in action.

## Images

In order to embed images into your doc, you need to copy them into the images folder and make a link to it. 
The best way to do so is the inbuilt image creator tool in the MarkdownEditor. Just click on the image icon, select any file in your system and it will copy it into the images folder and paste an appropriate link in the current cursor position:

![MyFile](/images/custom/myfile.png) 

You can also opt to rename it in case you've created it using a screen grabber which gave it the meaningfull name **Unbenannt(8).png**.

> Be aware that the images will **NOT** use the Images folder of your plugin. These things are kept separate because they might be updated along with the content.

### SVG images

SVG images can also be embedded, but I highly recommend to use the width limiting syntax here, or small icons / graphs will be expanded to full width and look funny:

This will happen if you load a SVG image like this: `![](images/custom/kick_outline.svg)`

![](images/custom/kick_outline.svg)

Better: `![](images/custom/kick_outline.svg:64px)`

![](images/custom/kick_outline.svg:64px)

## ![](images/custom/kick_outline.svg:64px) Pro-Tip: Using icons in a headline

By default images can't be inlined so they appear inside normal text flow. The only exception is to use icons in a headline like shown above:


> `## ![](images/custom/kick_outline.svg:64px) Pro Tip: ...`


