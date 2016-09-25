#import "MrubyPlg.h"
#include "test.h"
#include "mruby.h"
#include "mruby/string.h"
//#include <mruby/compile.h>

static NSString* mystdin; 
static mrb_state* mrb;
static MrubyPlg* gPlugin;

static void
printstr(mrb_state *mrb, mrb_value obj)
{
  char *str;

  if (mrb_string_p(obj)) {
    str = mrb_str_to_cstr(mrb, obj);
    NSString *nsstr = [ [ NSString alloc ] initWithUTF8String:str ];
    NSString* msg = [NSString stringWithFormat: @"mruby.__jsPuts('%@')", nsstr];
    [gPlugin.commandDelegate evalJs:msg];
  }
}

static mrb_value
mrb_printstr(mrb_state *mrb, mrb_value self)
{
  mrb_value argv;

  mrb_get_args(mrb, "o", &argv);
  printstr(mrb, argv);

  return argv;
}

static mrb_value
mrb_gets(mrb_state *mrb, mrb_value self)
{
  mrb_value val;
  mystdin = nil;
  [gPlugin.commandDelegate evalJs:@"mruby.__jsGets()"];
        
  while(1) {
    [NSThread sleepForTimeInterval:0.3];
    if(mystdin != nil) {
            break;
    }
  }
  NSString* msg = [NSString stringWithFormat: @"%@", mystdin];
  mystdin = nil;
  
  return mrb_str_new_cstr(mrb, [msg UTF8String]);
}

@implementation MrubyPlg

- (void)pluginInitialize
{
    gPlugin = self;
    mrb = mrb_open();
    struct RClass *krn;
    krn = mrb->kernel_module;
    mrb_define_method(mrb, krn, "__printstr__", mrb_printstr, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, krn, "gets", mrb_gets, MRB_ARGS_REQ(1));
}

- (void)mrbLoadString:(CDVInvokedUrlCommand*)command
{
    [[[NSOperationQueue alloc] init] addOperationWithBlock:^{
        //c_hello();
        char *buf;
        NSString* script = [[command arguments] objectAtIndex:0];
        buf = c_hello(mrb, [script UTF8String]);

        NSString* msg = [NSString stringWithFormat: @"Hello, %@", script];
        //free(buf);
        CDVPluginResult* result = [CDVPluginResult
                                resultWithStatus:CDVCommandStatus_OK
                                messageAsString:msg];
        
        [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
    }];
}

- (void)_doneGets:(CDVInvokedUrlCommand*)command
{
    NSString* name = [[command arguments] objectAtIndex:0];
    
    mystdin = [NSString stringWithFormat: @"%@", name];
    //[self.commandDelegate evalJs:[NSString stringWithFormat: @"alert('d1%@')",mystdin]];

    NSString* msg = [NSString stringWithFormat: @"Hello, %@", name];
    CDVPluginResult* result = [CDVPluginResult
                               resultWithStatus:CDVCommandStatus_OK
                               messageAsString:msg];
                        
    [self.commandDelegate sendPluginResult:result callbackId:command.callbackId];
}

@end