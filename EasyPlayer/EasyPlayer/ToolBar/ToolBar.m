#import "ToolBar.h"

@implementation ToolBar

-(id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        //设置自定义的MyToolbar为透明背景
        self.backgroundColor = COLOR(30,30, 30, 0.5);
    }
    return self;
}
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}

@end
