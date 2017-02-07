#import "VideoCell.h"

@implementation VideoCell

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    if(self){
        self.layer.masksToBounds = NO;
        self.layer.contentsScale = [UIScreen mainScreen].scale;
        self.layer.borderColor = [[UIColor grayColor] CGColor];
        self.layer.borderWidth = 0.5;
        self.layer.shadowOpacity = 0.5;
        self.layer.shadowColor = [[UIColor grayColor] CGColor];
        self.layer.shadowRadius = 5;
        self.layer.shadowOffset  = CGSizeZero;
        self.layer.shadowPath = [UIBezierPath bezierPathWithRect:self.bounds].CGPath;

        CGRect imgRect = {CGPointZero,{self.contentView.frame.size.width,self.contentView.frame.size.height - VIDEO_TITLE_HEIGHT}};
        self.imageView = [[UIImageView alloc]initWithFrame:imgRect];
        self.imageView.image = [UIImage imageNamed:@"ImagePlaceholder"];
        [self.contentView addSubview: self.imageView];
        self.titleLabel = [[UILabel alloc]initWithFrame:CGRectMake(0,self.contentView.frame.size.height-VIDEO_TITLE_HEIGHT, self.contentView.frame.size.width, VIDEO_TITLE_HEIGHT)];
        self.titleLabel.font = [UIFont systemFontOfSize:14];
        self.titleLabel.textAlignment = NSTextAlignmentLeft;
        self.titleLabel.backgroundColor = COLOR(222,222,222,1);
        self.titleLabel.textColor = [UIColor darkGrayColor];
        self.titleLabel.alpha= 1;
        self.titleLabel.layer.borderColor = [[UIColor grayColor] CGColor];
        self.titleLabel.layer.borderWidth = 0.5;
        [self bringSubviewToFront:self.titleLabel];
        [self.contentView addSubview:self.titleLabel];
    }
    return self;
}

// Check if the "thing" pass'd is empty
static inline BOOL isEmpty(id thing) {
    return thing == nil
    || [thing isKindOfClass:[NSNull class]]
    || ([thing respondsToSelector:@selector(length)]
        && [(NSData *)thing length] == 0)
    || ([thing respondsToSelector:@selector(count)]
        && [(NSArray *)thing count] == 0);
}

@end
