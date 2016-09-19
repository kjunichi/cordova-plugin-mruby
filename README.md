# cordova-plugin-mruby

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
