// Set this to false to enable the standard browser interactions
// (context menu, scroll wheel, etc)
var ENABLED = true;

if(ENABLED)
{
	document.addEventListener('contextmenu', event => event.preventDefault());
	document.addEventListener('wheel', function(e) {
	e.preventDefault();
	e.stopPropagation();

	if (e.ctrlKey) {
	    var s = Math.exp(-e.deltaY / 100);
	    scale *= s;
	} else {
	    var direction = 1; //natural.checked ? -1 : 1;
	    tx += e.deltaX * direction;
	    ty += e.deltaY * direction;
	}
	var transform = 'translate(' + tx + 'px, ' + ty + 'px) scale(' + scale + ')';
	box.style.webkitTransform = transform;
	box.style.transform = transform;
	}, {
	passive: false // Add this
	});
}