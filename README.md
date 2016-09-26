# cordova-plugin-mruby


[![cordova-plugin-mruby](http://img.youtube.com/vi/Mz27HAByEE4/0.jpg)](http://www.youtube.com/watch?v=Mz27HAByEE4)

```html
<textarea id="rubyscript">
puts "Hello,mruby!"
i=gets
puts 123.to_s + " mruby i=#{i}"
</textarea>
<div id="stdout"></div>
```

```javascript
const success = function(message) {
  //alert(message);
}
const failure = function() {
  alert("Error calling Hello Plugin");
}
const script = document.getElementById("rubyscript").value;
const stdout = document.getElementById("stdout");
mruby.mrbLoadString(script, stdout, success, failure);
```
