#include <stdlib.h>
#include <string.h>
#include "mruby.h"
#include "mruby/value.h"
#include "mruby/compile.h"
#include "mruby/string.h"
#include "test.h"

char* c_hello(mrb_state *mrb, const char* script) {
    
    mrb_value val;
    char *result;
    int len;

    
    //char *script = "a=1+2;\"Hello, mruby 1+2 = #{a}\"";
    val = mrb_load_string(mrb, script);

    //len = RSTRING_LEN(val);
    // result = (char*)malloc(sizeof(char)*(len+1));
    // memcpy(result, RSTRING_PTR(val),len);
    // result[len]= '\0';
    result = (char*)malloc(sizeof(char)*(1));
    result[0]= '\0';
    //mrb_close(mrb);
    //char* hello= "Hello I'm pure c function";
    //return hello;
    return result;
}