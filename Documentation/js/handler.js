function readBrowserFile(file, textFunc) {
    fetch(file)
    .then((res) => res.text())
    .then((text) => {
        textFunc(text)
    })
    .catch((e) => console.error(e));
}

var loadedCount = 0;
var totalLoaded = 0;

function checkFinish() {
    loadedCount += 1;

    document.getElementById("load_status").innerHTML = "Were preparing the docs ("+ loadedCount.toString() +"/"+ totalLoaded.toString() +")."

    if (loadedCount >= totalLoaded) {
        document.getElementById("load_overlay").classList.add("load_overlay_opacity");
        setTimeout(function() {
            document.getElementById("load_overlay").remove();
        }, 1000)
    }
}

function parse(t) {
    var parsed = {
        Functions: [],
        Directories: [],
        Texts: []
    }

    var sp = t.split("\n")

    for (let i = 0; i < sp.length; i++) {
        const element = sp[i];
        if (element.substring(0, 4) == "$dir") {
            var name = element.substring(5, element.indexOf("<"));
            var rest = element.substring(element.indexOf("<")+1);

            parsed.Directories.push([0, name, rest])
        }
        else if (element.substring(0, 5) == "$file") {
            var name = element.substring(6, element.indexOf("<"));
            var rest = element.substring(element.indexOf("<")+1);

            parsed.Directories.push([1, name, rest])
        }
        else if (element.substring(0, 5) == "func$") {
            var name = element.substring(6, element.indexOf("<"));
            var rest = element.substring(element.indexOf("<")+1);

            parsed.Functions.push([0, name, rest])
        }
        else if (element.substring(0, 5) == "func#") {
            var name = element.substring(5);

            parsed.Functions.push([1, name])
        }
        else if (element.substring(0, 3) == "###") {
            var name = element.substring(3);

            parsed.Texts.push([0, name])
        }
        else if (element.substring(0, 2) == "##") {
            var name = element.substring(2);

            parsed.Texts.push([1, name])
        }
        else if (element.substring(0, 1) == "#") {
            var name = element.substring(1);

            parsed.Texts.push([2, name])
        }
    }

    return parsed
}

var loadedCurrent = 0

var elements = []
var elements2 = []

function loadFile(f) {
    readBrowserFile(f, function(txt) {
        var parsed = parse(txt)

        var el = []

        totalLoaded += parsed.Functions.length;

        totalLoaded += parsed.Texts.length;

        for (let i = 0; i < parsed.Functions.length; i++) {
            const element = parsed.Functions[i];
            elements.push([0, element]);
            checkFinish();
        }

        for (let i = 0; i < parsed.Texts.length; i++) {
            const element = parsed.Texts[i];
            elements.push(element);
            checkFinish();
        }

        elements2.push([f, el])

        loadedCurrent += 1;
    })
}

function wait1(after) {
    if (loadedCount >= totalLoaded - 1 && loadedCurrent > 0) {
        after()
    }
    else {
        setTimeout(function() { wait1(after); }, 10)
    }
}

function load() {
    readBrowserFile("data/files", function(txt) {
        var parsed = parse(txt)
        
        totalLoaded += parsed.Directories.length - 1;

        for (let i = 0; i < parsed.Directories.length; i++) {
            const element = parsed.Directories[i];
            if (element[0] == 0) {
                loadFile("data/" + element[1])
            }
        }

        wait1(function() {
            for (let i = 0; i < parsed.Directories.length; i++) {
                const element = parsed.Directories[i];

                var child = document.createElement("button")
                if (element[0] == 0) {
                    child.classList.add("collapsing")
                }
                child.innerHTML = element[2];

                document.getElementById("side").appendChild(child)
                document.getElementById("side").innerHTML += "<br/>"

                checkFinish();
            }
        })
    });
}

load()