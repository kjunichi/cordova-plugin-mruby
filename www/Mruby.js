module.exports = {
  mrbLoadString: function (name, successCallback, errorCallback) {
      cordova.exec(successCallback, errorCallback, "Mruby", "mrbLoadString", [name]);
  }
};
