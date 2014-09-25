#import "OGRechargeObserver.h"
#import "Network/OGRecharge.h"

#import <StoreKit/StoreKit.h>


@implementation OGRechargeObserver

@synthesize currentTransaction = currentTransaction_;

static OGRechargeObserver* sharedObserver = nil;

- (id)init
{
    self = [super init];
    if (self != nil)
    {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    }
    return self;
}

- (void)dealloc
{ 
    [currentTransaction_ release];
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    [super dealloc];
}

+ (OGRechargeObserver*)sharedRechargeObserver
{
    if (sharedObserver == nil)
    {
        sharedObserver = [[OGRechargeObserver alloc] init];
    }
    return sharedObserver;
}

// 验证产品列表
- (void)verifyProducts:(NSSet *)products
{
    SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:products];
	request.delegate = self;	
	[request start];
}

// 购买产品
- (void)buyProduct:(NSString *)productID
{
    SKPayment* payment = [SKPayment paymentWithProductIdentifier:productID];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

// 完成当前交易
- (void)finishCurrentTransaction
{
    if (currentTransaction_) 
    {
        [[SKPaymentQueue defaultQueue] finishTransaction:currentTransaction_];
        self.currentTransaction = nil;
    }
}

#pragma mark - SKProductsRequestDelegate

// 收到经苹果服务器验证后的产品列表
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    // TODO: add products to list
	NSLog(@"-----------收到产品反馈信息--------------");
    NSArray *myProduct = response.products;
    NSLog(@"产品Product ID:%@",response.invalidProductIdentifiers);
    NSLog(@"产品付费数量: %d", [myProduct count]);
    // populate UI
    for(SKProduct *product in myProduct){
        NSLog(@"product info");
        NSLog(@"SKProduct 描述信息%@", [product description]);
        NSLog(@"产品标题 %@" , product.localizedTitle);
        NSLog(@"产品描述信息: %@" , product.localizedDescription);
        NSLog(@"价格: %@" , product.price);
        NSLog(@"Product id: %@" , product.productIdentifier);
    }
//    SKPayment *payment = nil;
//    switch (buyType) {
//        case IAP0p99:
//            payment  = [SKPayment paymentWithProductIdentifier:ProductID_IAP0p99];    //支付$0.99
//            break;
//        case IAP1p99:
//            payment  = [SKPayment paymentWithProductIdentifier:ProductID_IAP1p99];    //支付$1.99
//            break;
//        case IAP4p99:
//            payment  = [SKPayment paymentWithProductIdentifier:ProductID_IAP4p99];    //支付$9.99
//            break;
//        case IAP9p99:
//            payment  = [SKPayment paymentWithProductIdentifier:ProductID_IAP9p99];    //支付$19.99
//            break;
//        case IAP24p99:
//            payment  = [SKPayment paymentWithProductIdentifier:ProductID_IAP24p99];    //支付$29.99
//            break;
//        default:
//            break;
//    }
//    NSLog(@"---------发送购买请求------------");
//    [[SKPaymentQueue defaultQueue] addPayment:payment];
    [request autorelease];
}

#pragma mark - Transaction result

- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    NSData* receipt = transaction.transactionReceipt;
    NSString* uid = transaction.transactionIdentifier;
    
    self.currentTransaction = transaction;
    
    NSLog(@"[IGP] Transaction completed!");
    
    OGRecharge::getInstance().sendAppleReceipt((const char*)[receipt bytes], [uid UTF8String]);
}

- (void)restoreTransaction:(SKPaymentTransaction*)transaction
{
    NSData* receipt = transaction.originalTransaction.transactionReceipt;
    NSString* uid = transaction.originalTransaction.transactionIdentifier;
    
    self.currentTransaction = transaction;
    
    NSLog(@"[IGP] Transaction restored!");
    
    OGRecharge::getInstance().sendAppleReceipt((const char*)[receipt bytes], [uid UTF8String]);
}

- (void)failedTransaction:(SKPaymentTransaction*)transaction
{
	if (transaction.error.code != SKErrorPaymentCancelled)
    { 
        NSError* err = transaction.error;
        NSLog(@"[IGP] Transaction failed! Domain: %@, Code: %d", err.domain, err.code);
	}
    
    int code = transaction.error.code;
    if(transaction.error && transaction.error.localizedDescription){
        std::string error([transaction.error.localizedDescription UTF8String]);
        OGRecharge::getInstance().transactionFailed(code, error);
    }else{
        std::string error;
        OGRecharge::getInstance().transactionFailed(code, error);
    }
    
	[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
    self.currentTransaction = nil;
}

#pragma mark - SKPaymentTransactionObserver

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {				
            case SKPaymentTransactionStatePurchased:				
                [self completeTransaction:transaction];
				NSLog(@"-----交易完成 --------");
                break;
				
            case SKPaymentTransactionStateFailed:				
                [self failedTransaction:transaction];
				NSLog(@"-----交易失败 --------");
                break;
				
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
				NSLog(@"-----已经购买过该商品 --------");
                break;
				
			case SKPaymentTransactionStatePurchasing://购买中
				NSLog(@"-----商品添加进列表 --------");
                break;
            
			default:
                break;
        }
    }
}

//处理未完成的交易
-(void)processPendingTransactions
{
	NSArray* transactions = [SKPaymentQueue defaultQueue].transactions;
	if (transactions.count > 0)//检测是否有未完成的交易
	{
		SKPaymentTransaction* transaction = [transactions firstObject];
		if (transaction.transactionState == SKPaymentTransactionStatePurchased)
		{
			[self completeTransaction:transaction];
//			[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
			return;
		}
	}
}

@end
