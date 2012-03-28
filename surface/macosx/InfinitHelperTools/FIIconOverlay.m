//
//  FIIconOverlay.m
//  HelperTools
//
//  Created by Charles Guillot on 27/03/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "FIIconOverlay.h"
#import "FITableCellDictKey.h"

@implementation FIIconOverlay

@synthesize syncingIconRef;
@synthesize syncedIconRef;
@synthesize disconnectedIconRef;

+ (void) initialize
{
    [NSClassFromString(@"TListViewIconAndTextCell") 
     fi_swizzleMethod:@selector(drawIconWithFrame:) 
     withMethod:@selector(drawOverlayIconWithFrame:) 
     error:NULL];
    
    [NSClassFromString(@"TBrowserTableColumnViewController") 
     fi_swizzleMethod:@selector(tableView:willDisplayCell:forTableColumn:row:) 
     withMethod:@selector(tableViewWithNodeCaptcher:willDisplayCell:forTableColumn:row:) 
     error:NULL];
    
    [NSClassFromString(@"FINode") 
     fi_swizzleMethod:@selector(IKImageRepresentationWithType:) 
     withMethod:@selector(IKImageRepresentationWithTypeWithNodeCaptcher:) 
     error:NULL];
}

- (id) init
{
    self = [super init];
    
    if (!nodeStatusOperationQueue) {
        nodeStatusOperationQueue = [[NSOperationQueue alloc] init];
        [nodeStatusOperationQueue setMaxConcurrentOperationCount:2];
    }
    if(self) {
        NSString* imageName;
        
        imageName = [[NSBundle bundleWithPath:@"/Applications/Infinit.app"] pathForResource:@"ok" ofType:@"icns"];
        syncingIconRef = [self iconRefForURL:[NSURL fileURLWithPath:imageName]];
        
        imageName = [[NSBundle bundleWithPath:@"/Applications/Infinit.app"] pathForResource:@"ok" ofType:@"icns"];
        syncedIconRef = [self iconRefForURL:[NSURL fileURLWithPath:imageName]];
        
        imageName = [[NSBundle bundleWithPath:@"/Applications/Infinit.app"] pathForResource:@"ok" ofType:@"icns"];
        disconnectedIconRef = [self iconRefForURL:[NSURL fileURLWithPath:imageName]];
	}
    return self;
}

- (IconRef)iconRefForURL:(NSURL *)arg1
{
    FSRef   theFSRef;
    IconRef theIconRef;
    
    if( CFURLGetFSRef((CFURLRef)arg1, &theFSRef))
    {
        RegisterIconRefFromFSRef ('ANYT', 'HING', &theFSRef, &theIconRef );
    }
    
    return theIconRef;
}                                             
+ (FIIconOverlay*) instance
{
	static FIIconOverlay *item = nil;
	
	if(item == nil)
		item = [[FIIconOverlay alloc] init];
	
	return item;
}
- (void) addCellStatusWithCell:(id)arg1
{
    if (arg1 != nil && [nodesStatusDict objectForKey:arg1] == nil )
    {
        //CFDictionarySetValue((CFMutableDictionaryRef)nodeStatus,tIconAndTextCell, watingStatusImg);
        // Add task to operation queue
        [nodeStatusOperationQueue addOperationWithBlock:^{
            sleep(1);
            //CFDictionarySetValue((CFMutableDictionaryRef)nodeStatus,tIconAndTextCell, satusChecked);
            //[self UpdateViewLayout];
            [[arg1 controlView] updateCell:arg1];
        }];
    }
}

- (void) removeCellStatusWithCell:(id)arg1
{
    [[self CellStatusWithCell:arg1] release];
	[nodesStatusDict removeObjectForKey:arg1];
}

- (id) CellStatusWithCell:(id)arg1
{
	return [nodesStatusDict objectForKey:arg1];
}

@end

@implementation NSTextFieldCell (FIIconOverlayBadge)

- (void) setOverlayIcon:(NodeStatus)arg1
{
    if ([self respondsToSelector:@selector(node)]) {
        IconRef *badgedIconRef;
        TIconRef *iconRef;
        iconRef = (TIconRef *)[self icon];
        IconRef backgroundIconRef = iconRef->fRef;
        IconRef foregroundIconRef = [[FIIconOverlay instance] syncingIconRef];
        CompositeIconRef (  backgroundIconRef,
                          foregroundIconRef,
                          &badgedIconRef);
        const struct TFENode *tfeIcon = {badgedIconRef};
        objc_msgSend(self, @selector(setIcon:), &tfeIcon);
        TFENode *node = [self node];
    }
}

- (void) drawOverlayIconWithFrame:(struct CGRect)arg1
{
    id cellStatus = [[FIIconOverlay instance] CellStatusWithCell:self];
    if (cellStatus == nil)
    {
        [[FIIconOverlay instance] addCellStatusWithCell:self];
    }
    else
    {
        [self setOverlayIcon:cellStatus];
    }
    [self drawOverlayIconWithFrame:arg1];
}

@end

@implementation NSViewController (FIIconOverlayBadge)

- (void) tableViewWithNodeCaptcher:(id)arg1 willDisplayCell:(id)arg2 forTableColumn:(id)arg3 row:(int)arg4;
{
    if ([arg2 isKindOfClass:NSClassFromString(@"TColumnCell")])
    {
        id dictKey = [FITableCellDictKey tableCellDictKeyWithColumnIdentifer:arg3 rowIndex:arg4];
    
        id cellStatus = [[FIIconOverlay instance] CellStatusWithCell:dictKey];
    
        if (cellStatus == nil) {
            [[FIIconOverlay instance] addCellStatusWithCell:dictKey];
        }
        else
        {
            [arg2 setOverlayIcon:cellStatus];
        }
    }
    [self tableViewWithNodeCaptcher:arg1 willDisplayCell:arg2 forTableColumn:arg3 row:arg4];
}

@end

@implementation NSObject (FIIconOverlayBadge)

- (id)IKImageRepresentationWithTypeWithNodeCaptcher:(id)arg1
{
    if ([self isInIconView]) {
        //TODO
    }
    return [self IKImageRepresentationWithTypeWithNodeCaptcher:arg1];
}

@end