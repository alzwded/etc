function doStuff() {
    alert('doing')
    //var iframes = document.getElementsByTagName("object")
    //var contents = new Array()
    //for(i = 0; i < iframes.length; ++i) {
    //    var frame = iframes[i]
    //    var contentDiv = frame.contentWindow.document.getElementsByClassName("main-content")
    //    contents.push(contentDiv)
    //}

    var contents = document.getElementsByClassName("main-content")
    var target = document.getElementById('bookContent')
    
    for(i = 0; i < contents.length; ++i) {
        var el = contents[i]
        var newDiv = document.createElement("DIV")
        newDiv.innerHTML = el.innerHTML
        target.appendChild(newDiv)
    }
}
    
function increment() {
    counter--
    if(counter == 0) {
        doStuff()
    }
}
