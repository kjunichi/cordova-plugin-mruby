require('cordova/channel').onCordovaReady.subscribe(function() {
     require('cordova/exec')(win, null, 'Mruby', 'method', []);

    function win(message) {
        //...process message from java here...
        console.log(message);
        return window.prompt("stdin");
    }
});

module.exports = {
    mrbLoadString: function(name, dom, successCallback, errorCallback) {
        this._dom = dom;
        cordova.exec(successCallback, errorCallback, "Mruby", "mrbLoadString", [name]);
    },
    __jsPuts: function(s) {
        //alert(s);
        const e = document.createElement("div");
        e.innerHTML = s;
        this._dom.appendChild(e);
    },
    __jsGets: function() {
      console.log("__jsGets");
        const stdin = window.prompt("stdin");
        cordova.exec(function(s){console.log(s);}, null, "Mruby", "_doneGets", [stdin]);
        return stdin;
    },
    __jsDoneGets: function(inputString, successCallback, errorCallback) {
      console.log("__jsDoneGets");
      cordova.exec(successCallback, errorCallback, "Mruby", "_doneGets", [inputString]);
    }

};
