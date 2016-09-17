module.exports = {
  coolMethod: function (name, successCallback, errorCallback) {
      cordova.exec(successCallback, errorCallback, "Mruby", "coolMethod", [name]);
  }
};
