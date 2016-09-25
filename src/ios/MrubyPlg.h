#import <Cordova/CDV.h>

@interface MrubyPlg : CDVPlugin

- (void)pluginInitialize;
- (void)mrbLoadString:(CDVInvokedUrlCommand*)command;
- (void)_doneGets:(CDVInvokedUrlCommand*)command;

@end