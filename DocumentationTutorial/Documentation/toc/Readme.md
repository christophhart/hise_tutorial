---
keywords: Using the TOC
summary:  The TOC structure
author:   Christoph Hart
modified: 08.10.2019
index:    01
colour:   0xFF448877
---
  
The TOC on the left is a handy way of navigating the documentation. By default it mirrors the file structure in your documentation folder structure, but you can fine-tune it with the tricks described here.

## Hierarchy

The hierarchy is closely mirroring the folder structure: every folder will be a expandable item that will show its files. Unfortunately this would result in a high depth of hierarchy (one level for each folder, then one level for each file.  
This is why there is one trick to keep the hierarchy depth low: a file called `readme.md` that is found in the folder is being used as default content of the folder.

## Headlines

The TOC will also show headlines up to level 2 (so the ones with `#` or `##`) in the TOC. Be aware that you don't need to specify a top headline as it will be done automatically from the keywords YAML property.

## Customizing the TOC

There are a few YAML header properties that allow you to define the appearance of the TOC:

| Property | Effect |
| --- | -------- |
| `keywords` | The exact string that will show up in the TOC. It will also be the first headline in the content being displayed. |
| `colour` | The colour in the hex format `0xAARRGGBB` that will be used for the TOC item (and all its child items unless they "override" it with a custom colour themselves. |
| `index` | By default it will sort the TOC items alphabetically, which might not correlate with the order you want them to appear. In this case you can **force** an order by specifying an index. As soon as an index is specified, all its children without index will be sorted after the sibling with an index (and those with an index will be obviously sorted by index. |

The other properties will affect the search engine, so if you want to know about them, check the full documentation in HISE about this topic.



