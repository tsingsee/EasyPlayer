#import "RootViewController.h"
#import "PlayViewController.h"
#import "VideoCell.h"
#import <CommonCrypto/CommonDigest.h>

@interface RootViewController()<UICollectionViewDelegate,UICollectionViewDataSource,UIActionSheetDelegate>
{
    UIAlertView* _alertView;
    UIAlertView* _deleteAlertView;
    UIActionSheet* _actionSheet;
}
@end

@implementation RootViewController

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.collectionView reloadData];
    NSLog(@"view will appear");
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    [self navigationSetting];
    
    UICollectionViewFlowLayout *layout = [[UICollectionViewFlowLayout alloc]init];
    //定义每个UICollectionView 横向的间距
    layout.minimumLineSpacing = 10;
    //定义每个UICollectionView 纵向的间距
    layout.minimumInteritemSpacing = 10;
    //定义每个UICollectionView 的边距距
    layout.sectionInset = UIEdgeInsetsMake(10, 10, 10, 10);//上左下右
    self.collectionView = [[UICollectionView alloc]initWithFrame:CGRectMake(0, 0, ScreenWidth, ScreenHeight) collectionViewLayout:layout];
    self.collectionView.delegate = self;
    self.collectionView.dataSource = self;
    self.collectionView.showsHorizontalScrollIndicator = NO;
    self.collectionView.showsVerticalScrollIndicator = NO;
    self.collectionView.backgroundColor = [UIColor whiteColor];
    
    
    [self.collectionView registerClass:[VideoCell class] forCellWithReuseIdentifier:@"VideoCell"];
    [self.view addSubview:self.collectionView];
    NSMutableArray *urls = [[NSUserDefaults standardUserDefaults] objectForKey:@"videoUrls"];
    if(urls){
        _dataArray = [urls mutableCopy];
    }else{
        _dataArray = [NSMutableArray array];
    }
    
    _alertView = [[UIAlertView alloc] initWithTitle:@"请输入播放地址" message: nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    [_alertView setAlertViewStyle:UIAlertViewStylePlainTextInput];
    [_alertView textFieldAtIndex:0].keyboardType = UIKeyboardTypeURL;
    
    _deleteAlertView = [[UIAlertView alloc] initWithTitle:@"确定删除?" message:nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
    
    _actionSheet = [[UIActionSheet alloc] initWithTitle:nil delegate:self cancelButtonTitle:@"取消" destructiveButtonTitle:@"删除" otherButtonTitles:@"修改", nil];
    
    self.collectionView.mj_header = [MJRefreshNormalHeader headerWithRefreshingTarget:self refreshingAction:@selector(doReload)];
}

- (void)doReload
{
    [self.collectionView reloadData];
    [self.collectionView.mj_header endRefreshing];
}

//导航栏设置
- (void)navigationSetting
{
    [self.navigationController.navigationBar setTitleTextAttributes:
     @{NSFontAttributeName:[UIFont systemFontOfSize:17],
       NSForegroundColorAttributeName:[UIColor whiteColor]}];
    
    UIView* logo = [[UIView alloc]init];
    logo.frame = CGRectMake(0, 0, 160, 30);
    UIImageView* img = [[UIImageView alloc]init];
    img.layer.masksToBounds = YES;
    img.layer.cornerRadius = 5;
    img.image = [UIImage imageNamed:@"EasyPlayer_60"];
    img.frame = CGRectMake(0, 0, 30, 30);
    [logo addSubview:img];
    
    UILabel* label = [[UILabel alloc] initWithFrame:CGRectMake(40, 0, 130, 30)];
    [label setText:@"EasyPlayer"];
    [label setTextColor:[UIColor whiteColor]];
    [logo addSubview:label];
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithCustomView:logo];
    
    UIButton *addBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [addBtn setFrame:CGRectMake(0, 0, 30, 30)];
    [addBtn setBackgroundImage:[UIImage imageNamed:@"ic_action_add"] forState:UIControlStateNormal];
    [addBtn addTarget:self action:@selector(clickAddBtn) forControlEvents:UIControlEventTouchUpInside];
    UIBarButtonItem *addItem = [[UIBarButtonItem alloc]initWithCustomView:addBtn];
    self.navigationItem.rightBarButtonItem = addItem;
}

- (void)clickAddBtn
{
    [_alertView textFieldAtIndex:0].text = @"rtsp://";
    _alertView.tag = -1;
    [_alertView show];
}

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(alertView == _alertView && buttonIndex == 1){
        UITextField *tf = [_alertView textFieldAtIndex:0];
        NSString* url = [tf.text stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        if(_alertView.tag < 0){//new
            [_dataArray insertObject:url atIndex:0];
        } else {//edit
            [_dataArray removeObjectAtIndex:_alertView.tag];
            [_dataArray insertObject:url atIndex:_alertView.tag];
        }
        [[NSUserDefaults standardUserDefaults] setObject:_dataArray forKey:@"videoUrls"];
        [[NSUserDefaults standardUserDefaults] synchronize];
        [self.collectionView reloadData];
    }
    if(alertView == _deleteAlertView && buttonIndex == 1){
        NSString* url = [_dataArray objectAtIndex:_deleteAlertView.tag];
        const char *cStr = [url UTF8String];
        unsigned char result[16];
        CC_MD5(cStr, (CC_LONG)strlen(cStr), result);
        NSString* strMd5 = [NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
                            result[0], result[1], result[2], result[3],
                            result[4], result[5], result[6], result[7],
                            result[8], result[9], result[10], result[11],
                            result[12], result[13], result[14], result[15]
                            ];
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* docDir = [paths objectAtIndex:0];
        NSString* path = [docDir stringByAppendingPathComponent:[NSString stringWithFormat:@"snapshot/%@.png", strMd5]];
        if([[NSFileManager defaultManager] fileExistsAtPath:path]){
            [[NSFileManager defaultManager] removeItemAtPath:path error:nil];
        }
        [_dataArray removeObjectAtIndex:_deleteAlertView.tag];
        [[NSUserDefaults standardUserDefaults] setObject:_dataArray forKey:@"videoUrls"];
        [[NSUserDefaults standardUserDefaults] synchronize];
        [self.collectionView reloadData];
    }

}

- (void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if(actionSheet == _actionSheet){
        switch (buttonIndex) {
            case 0:
            {
                //delete
                _deleteAlertView.tag = _actionSheet.tag;
                [_deleteAlertView show];
                break;
            }
            case 1:
            {
                //edit
                _alertView.tag = _actionSheet.tag;
                UITextField *tf = [_alertView textFieldAtIndex:0];
                tf.text = [_dataArray objectAtIndex:_alertView.tag];
                [_alertView show];
                break;
            }
            default:
                break;
        }
    }
}

-(NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section
{
    return _dataArray.count;
}

-(CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath
{
    int w = ScreenWidth - 20;
    int h = w * 9 /16;
    return CGSizeMake(w, h + VIDEO_TITLE_HEIGHT);//30 is the bottom title height
}

-(UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath
{
    VideoCell * cell = [collectionView dequeueReusableCellWithReuseIdentifier:@"VideoCell" forIndexPath:indexPath];
    NSString *url = _dataArray[indexPath.row];
    [cell.titleLabel setText:[NSString stringWithFormat:@"  %@",url]];
    
    const char *cStr = [url UTF8String];
    unsigned char result[16];
    CC_MD5(cStr, (CC_LONG)strlen(cStr), result);
    NSString* strMd5 = [NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            result[0], result[1], result[2], result[3],
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]
            ];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* docDir = [paths objectAtIndex:0];
    NSString* path = [docDir stringByAppendingPathComponent:[NSString stringWithFormat:@"snapshot/%@.png", strMd5]];
    if([[NSFileManager defaultManager] fileExistsAtPath:path]){
        cell.imageView.image = [UIImage imageWithContentsOfFile:path];
    }else {
        cell.imageView.image = [UIImage imageNamed:@"ImagePlaceholder"];
    }
    
    UILongPressGestureRecognizer* longgs=[[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(longpress:)];
    [cell addGestureRecognizer:longgs];
    longgs.minimumPressDuration=0.3;
    longgs.view.tag=indexPath.row;
    return cell;
}

-(void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath
{
    PlayViewController* pvc = [[PlayViewController alloc] init];
    pvc.urlStr = _dataArray[indexPath.row];
    const char *cStr = [pvc.urlStr UTF8String];
    unsigned char result[16];
    CC_MD5(cStr, (CC_LONG)strlen(cStr), result);
    NSString* strMd5 = [NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            result[0], result[1], result[2], result[3],
            result[4], result[5], result[6], result[7],
            result[8], result[9], result[10], result[11],
            result[12], result[13], result[14], result[15]
            ];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString* docDir = [paths objectAtIndex:0];
    NSString* _dir = [docDir stringByAppendingPathComponent:@"snapshot"];
    if(![[NSFileManager defaultManager] fileExistsAtPath:_dir]){
        [[NSFileManager defaultManager] createDirectoryAtPath:_dir withIntermediateDirectories:YES attributes:nil error:nil];
    }
    pvc.imagePath = [_dir stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.png",strMd5]];
    [self presentViewController:pvc animated:YES completion:nil];
}

-(void)longpress:(UILongPressGestureRecognizer*)ges
{
    CGPoint pointTouch = [ges locationInView:self.collectionView];
    if(ges.state == UIGestureRecognizerStateBegan){
        NSIndexPath *indexPath = [self.collectionView indexPathForItemAtPoint:pointTouch];
        _actionSheet.tag = indexPath.row;
        [_actionSheet showInView:self.view];
    }
}


@end
