---
keywords: Setup
summary:  How to setup the documentation
author:   Christoph Hart
modified: 08.10.2019
colour:   0xFF8877EE
index:    00
---
  
The MarkdownPanel can be used to embed a user manual directly into the plugin.

Just add a MarkdownPanel as FloatingTile on your interface and it will display the documentation you've written in the Documentation subfolder.

## Initialisation

There are a few things that the Markdown Panel has to do the first time it is going to be used:

- building of the database. This might take a few seconds for complex documentations.
- if [server sync](/sync#setup-the-server-url) is enabled, it will fetch the hash from the server and update it if it finds a new version.

This initialisation will take place the first time it is made visible. If you show the panel without any page logic (like this tutorial), this will happen when you first create the plugin's interface.  
A much more real-world use case approach is that the MarkdownPanel is not visible at the beginning and only shows up after some sort of user interaction (like pressing a help button). In this case it will defer the initialisation until the last possible moment, which is shortly before the user wants to read the documentation.

> So unless you have some special use case, it's highly recommended to make the Markdown Panel not visible at startup.

## Writing the documentation

The documentation will fetch its content from the markdown files in the Documentation subdirectory of your project.  
The best workflow is to use the scripting workspace with the interface designer on the right and a markdown editor in the left code editor panel. If HISE finds a MarkdownEditor there, you will be able to right click and directly open a new tab with the markdown file.

- Click on the **+** icon
- Right click on the content of the new tab 
- Choose MarkdownEditor (below ScriptEditor)

If you want to view the changes you've made, just save the markdown file (Cmd+S), then rebuild the interface.
