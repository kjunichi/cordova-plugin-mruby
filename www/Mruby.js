module.exports = {
  mrbLoadString: function (name, dom, successCallback, errorCallback) {
    this._dom = dom;
    cordova.exec(successCallback, errorCallback, "Mruby", "mrbLoadString", [name]);
  },
  __jsPuts: function(s) {
    //alert(s);
    const e = document.createElement("div");
    e.innerHTML = s;
    this._dom.appendChild(e);
  }
};
