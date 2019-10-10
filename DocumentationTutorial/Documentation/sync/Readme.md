---
keywords: Update the documentation
summary:  How to use a online resource to update the docs automatically
author:   Christoph Hart
modified: 08.10.2019
---
  
Chances are huge that you might want to update the documentation of your plugin after the release: you'll notice that most of the questions that users will have will repeat themselve and where's a better place to adress this as directly in the plugin? That would be a problem if the documentation is baked into the plugin. 

The solution is pretty easy: you can define a Server URL which hosts the compiled documentation files and the plugin will check on startup whether the documentation has changed.

## Setup the server URL

1. Make sure you "export" your documentation into the three doc files.
2. Upload them to a publicly available server. 
3. Enter the URL to the location where you host your cached documentation files as `ServerUpdateURL` property in the panel.

Right click on the MarkdownPreviewPanel in your interface inside HISE and choose **Export**, then press OK. After this, the three required files can be found in your AppData's **Documentation** subfolder:

- `Content.dat` - contains a compressed version of all texts
- `Images.dat` - contains a compressed version of all images
- `hash.json` - just contains the checksum of the files and determines whether it needs to update one of them.

The server URL that you have to specify needs to point to the **parent directory of a folder called** `cache`:  
If you host your files on `https://example.com/docs`, you'll have to create a subfolder called `cache` in your `docs` folder, put the file in there and supply `https://example.com/docs` as `ServerUpdateURL`.

> if you use Filezilla, you can add a bookmark that points to your local appdata's Documentation folder and the FTP target location so you just need to drag them over.

So whenever you change the docs, repeat these steps (export in HISE, open up FileZilla (or your favorite FTP client) with the bookmark and drag the files on the server).

## Static HTML version

An embedded documentation is nice, but you probably also want some sort of online manual so that users without internet connection (or possible customers who want to check out your software) can also read your documentation.  
For this reason, HISE offers a **HTML Page Generator** that takes your markdown files and renders a bunch of static HTML sites that you can upload on the server.

1. Add the template files for the header and the footer.
2. Export the doc as HTML folder structure
3. Upload the files to your server.

### Template files

In order to create the HTML files, the HTML generator will look for two files called `header.html` and `footer.html` in the `template` subdirectory of your Documentation folder.  
The HTML generator will then simply create one file per page using this rule:

> header + content + footer

So the minimal header and footer that makes sense is:

#### Header:

```
<html>
<head/>
<body>
<div id="content">
```

#### Footer

```
</div>
</body>
</html>
```

If you have a minimal understanding of HTML, you'll see that the content is being put into one div container that can be customized with CSS using the `content` ID.