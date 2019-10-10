/* JS Toc Generator

  Takes the JSON object from toc.js in the same directory and generates
  a TOC using this scheme:

  <details >
  <summary style="padding-left: 10px; border-left: 3px solid #E1C81C">
    <a href="/introduction/index.html"> Introduction </a>
  </summary>
  <details >
    <summary style="padding-left: 10px; border-left: 3px solid #E1C81C">
      <a href="/introduction/installation-and-setup/index.html">Installation and Setup</a>
    </summary>
  </details>
</details>*/

function surroundWithTag(string, tagName, attributes)
{
	return "<" + tagName + " " + attributes + ">" + string + "</" + tagName + ">";
}

function createTocElement(object)
{
    if(object.Title.length == 0)
      return "";

    var path = window.location.pathname;
    var thisPath =object.URL.replace("/index.html", "")
    var isOpen = path.indexOf(thisPath) != -1;
    var isSelected = path.replace("/index.html", "") == thisPath;
    var cssStyleForSelected = surroundWithTag(object.Title, "a", "href='" + object.URL + "'");
    var selectedStyle = isSelected ? "class=\"selected_details\"" : "";

    var link = surroundWithTag(object.Title, "a", "href='" + object.URL + "'");
    var summary = surroundWithTag(link, "summary", selectedStyle + "style=\"border-left: 3px solid " + object.Colour + ";\"");

    for(child in object.Children)
      summary += createTocElement(object.Children[child]);


    summary += "\n";

    return surroundWithTag(summary, "details", isOpen ? "open" : "");
}


 // Sidebar opener for mobile

 window.onload = function () {

  var toc = document.querySelector(".toc");
  var menuButton = document.getElementById("nav-button");

  menuButton.addEventListener("click", function () {
    toc.classList.toggle("open");
  })

  document.body.addEventListener("click", function (e) {
    if (e.target !== menuButton && !toc.contains(e.target)) {
      toc.classList.remove("open");
    }
  })

  // Toggle toc on swipe
  var start = {}, end = {}

  document.body.addEventListener("touchstart", function (e) {
    start.x = e.changedTouches[0].clientX
    start.y = e.changedTouches[0].clientY
  })

  document.body.addEventListener("touchend", function (e) {
    end.y = e.changedTouches[0].clientY
    end.x = e.changedTouches[0].clientX

    var xDiff = end.x - start.x
    var yDiff = end.y - start.y

    if (Math.abs(xDiff) > Math.abs(yDiff)) {
      if (xDiff > 0 && start.x <= 80) toc.classList.add("open")
      else toc.classList.remove("open")
    }
  })

}
