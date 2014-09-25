//
//  UISystemControlTextView.mm
//  Texas-iPhone2
//
//  Created by Chaim on 14-7-1.
//
//

#include "UISystemControlTextView.h"

//@interface UISystemControlTextView ()
//
//@property (nonatomic, strong) UIColor* textColor;
//
//- (void) beginEditing:(NSNotification*) notification;
//- (void) endEditing:(NSNotification*) notification;
//
//@end



@implementation UISystemControlTextView
//@synthesize placeholder;
//@synthesize textColor;

@synthesize nMaxLimit;


-(id) init
{
    if ((self = [super init]))
    {
        
    }
	
	self.nMaxLimit = -1;
	self.delegate = self;
//	placeholder = @"请在此输入信息";
	
    return self;
}

- (void) dealloc
{
    [self release];
    [super dealloc];
}

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    if ([text isEqualToString:@"\n"])
	{
		return NO;
	}	
    else if ([text isEqualToString:@""] && range.length > 0)
	{//删除字符肯定是安全的
        return YES;
    }
    else
	{
        if (self.nMaxLimit > 0 && textView.text.length - range.length + text.length > self.nMaxLimit)
		{
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"超出最大可输入长度" message:nil delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil];
            [alert show];
            return NO;
        }
        else
		{
            return YES;
        }
    }
}

//-(void)addObserver
//{
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(beginEditing:) name:UITextViewTextDidBeginEditingNotification object:self];
//    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(endEditing:) name:UITextViewTextDidEndEditingNotification object:self];
//}
//
//-(void)removeobserver
//{
//    [[NSNotificationCenter defaultCenter] removeObserver:self];
//}
//
//#pragma mark -
//#pragma mark Setter/Getters
//- (void) setPlaceholder:(NSString *)aPlaceholder {
//    placeholder = aPlaceholder;
//    [self endEditing:nil];
//}
//
//- (NSString *) text {
//    NSString* text = [super text];
//    if ([text isEqualToString:placeholder]) return @"";
//    return text;
//}
//
//- (void) beginEditing:(NSNotification*) notification {
//    if ([super.text isEqualToString:placeholder]) {
//        super.text = nil;
//        //字体颜色
//        [super setTextColor:textColor];
//    }
//	
//}
//
//- (void) endEditing:(NSNotification*) notification {
//    if ([super.text isEqualToString:@""] || self.text == nil) {
//        super.text = placeholder;
//        //注释颜色
//        [super setTextColor:[UIColor lightGrayColor]];
//    }
//}
//

@end

